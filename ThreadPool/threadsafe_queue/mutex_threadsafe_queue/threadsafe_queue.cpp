#include "threadsafe_queue.h"
#include "../misc.cpp"

template<typename T>
threadsafe_queue<T>::threadsafe_queue(std::size_t capacity) {

	if (!check_single_bit(capacity)) {

		std::size_t newcapacity = 128;

		while (newcapacity < capacity)
			newcapacity *= 2;

		capacity = newcapacity;
	}

	arr = std::vector<T*>(capacity);
	head = 0;
	tail = 0;

}

template<typename T>
bool threadsafe_queue<T>::try_push(T& item) {

	std::unique_lock<std::mutex> lock(queue_mutex);
	if(lock.try_lock())
}

template<typename T>
bool threadsafe_queue<T>::try_pop(T& item) {

	size_t o = offers.load();

	size_t p = polls.load();

	size_t i = p % arr.size();

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

	bool success = std::atomic_compare_exchange_strong_explicit(arr[i], e, newround);

	polls.compare_exchange_strong(p, p + 1);

	if (success)
	{
		T return_item;
		memcpy(&return_item, e, sizeof(T));
		return true;
	}
	else
		return false;
}

template<typename T>
void threadsafe_queue<T>::push(T& item) {

	while (!try_push(item))
		std::this_thread::yield();

}

template<typename T>
T threadsafe_queue<T>::pop() {
	T item;

	while (!try_pop(item))
		std::this_thread::yield();

	return item;
}