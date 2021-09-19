#include "threadsafe_queue.h"
#include "../misc.cpp"
#include "../../functionwrapper.h"
#include <future>

template<class T, std::size_t capacity>
threadsafe_queue<T, capacity>::threadsafe_queue() {

	static_assert(check_single_bit(capacity), "Capacity of threadsafe_queue must be a power of 2");

	offers = 0;
	polls = 0;

}
template<>
threadsafe_queue<function_wrapper, 128>::threadsafe_queue() {

	static_assert(check_single_bit(128), "Capacity of threadsafe_queue must be a power of 2");

	offers = 0;
	polls = 0;

}

template<typename T, std::size_t capacity>
std::shared_ptr<T> threadsafe_queue<T, capacity>::try_push(T& item) {

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
}

template<typename T, std::size_t capacity>
std::shared_ptr<T> threadsafe_queue<T, capacity>::try_pop() {

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
		memcpy(&item, e, sizeof(T));
		return true;
	}
	else
		return false;
}

template<>
std::shared_ptr<function_wrapper> threadsafe_queue<function_wrapper,128>::try_pop() {

	std::size_t o = offers.load();

	std::size_t p = polls.load();

	

	std::size_t i = p % arr.size();

	function_wrapper* e = arr[i];

	if (p != polls)
		return false;

	if (p == o)
		return false;

	function_wrapper* newround = (function_wrapper*)(p / arr.size() + 1);

	if (e == newround) {
		polls.compare_exchange_strong(p, p + 1);
		return false;
	}

	bool success = arr[i].compare_exchange_strong(e, newround);

	polls.compare_exchange_strong(p, p + 1);

	if (success)
	{
		memcpy(&item, e, sizeof(function_wrapper));
		return true;
	}
	else
		return false;
}

template<typename T, std::size_t capacity>
void threadsafe_queue<T, capacity>::push(T& item) {

	while (!try_push(item))
		std::this_thread::yield();

}

template<typename T, std::size_t capacity>
void threadsafe_queue<T, capacity>::push(T&& item) {

	while (true) {
		T* ptr_item = new T();

		std::size_t o = offers.load();

		std::size_t p = polls.load();

		if (o != offers)
			continue;

		if (o == p + arr.size())
			continue;

		std::size_t i = o % arr.size();
		T* round = (T*)(o / arr.size());
		bool success = arr[i].compare_exchange_strong(round, ptr_item);

		offers.compare_exchange_strong(o, o + 1);

		if (success)
		{
			memcpy(ptr_item, &item, sizeof(T));
			break;
		}
		else
			delete ptr_item;
	}

}

template<>
void threadsafe_queue<function_wrapper, 128>::push(function_wrapper&& item) {

	while (true) {

		

		std::size_t o = offers.load();

		std::size_t p = polls.load();

		if (o != offers)
			continue;

		if (o == p + arr.size())
			continue;

		std::size_t i = o % arr.size();
		function_wrapper* round = (function_wrapper*)(o / arr.size());
		bool success = arr[i].compare_exchange_strong(round, &item);

		offers.compare_exchange_strong(o, o + 1);

		if (success)
		{
			break;
		}

	}

}

template<typename T, std::size_t capacity>
T threadsafe_queue<T, capacity>::pop() {
	T item;

	while (!try_pop(item))
		std::this_thread::yield();

	return item;
}