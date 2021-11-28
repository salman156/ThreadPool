#pragma once
#include <atomic>
#include <array>
#include <memory>
#include <thread>

//TODO: Requires fixing

template<typename T, std::size_t capacity>
class threadsafe_queue{
	std::array<std::atomic<T*>,capacity> arr;
	std::atomic<std::size_t> offers;
	std::atomic<std::size_t> polls;
public:
	threadsafe_queue();

	std::unique_ptr<T> try_pop();
	bool try_push(const T& item);
	void push(const T& item);
	T pop();

};

template<class T, std::size_t capacity>
threadsafe_queue<T, capacity>::threadsafe_queue() {

	static_assert(check_single_bit(capacity), "Capacity of threadsafe_queue must be a power of 2");

	offers = 0;
	polls = 0;

}

template<typename T, std::size_t capacity>
bool threadsafe_queue<T, capacity>::try_push(const T& item) {

	T* ptr_item = new T();

	std::size_t o = offers.load();

	std::size_t p = polls.load();

	if (o != offers)
		return false;

	if (o == p + arr.size())
		return false;

	std::size_t i = o % arr.size();
	T* round = (T*)(o / arr.size());
	bool success = arr[i].compare_exchange_strong(round, ptr_item);

	offers.compare_exchange_strong(o, o + 1);

	if (success)
	{
		memcpy(ptr_item, &item, sizeof(T));
		return true;
	}
	else
		delete ptr_item;

	return false;
}

template<typename T, std::size_t capacity>
std::unique_ptr<T> threadsafe_queue<T, capacity>::try_pop() {

	std::size_t o = offers.load();

	std::size_t p = polls.load();

	std::size_t i = p % arr.size();

	T* e = arr[i];

	if (p != polls)
		return false;

	if (p == o)
		return false;

	T* newround = (T*)(p / arr.size() + 1);

	if (e == newround) {
		polls.compare_exchange_strong(p, p + 1);
		return false;
	}

	bool success = arr[i].compare_exchange_strong(e, newround);

	polls.compare_exchange_strong(p, p + 1);

	if (success)
	{
		return std::make_unique(e);
	}
	else
		return nullptr;
}

template<typename T, std::size_t capacity>
void threadsafe_queue<T, capacity>::push(const T& item) {

	while (!try_push(item))
		std::this_thread::yield();

}

template<typename T, std::size_t capacity>
T threadsafe_queue<T, capacity>::pop() {
	T item;

	while (!try_pop())
		std::this_thread::yield();

	return item;
}