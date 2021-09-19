#pragma once
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
template<typename T>
class threadsafe_queue {
	std::vector<T*> arr;
	std::mutex queue_mutex;
	std::size_t head;
	std::size_t polls;
public:
	threadsafe_queue(std::size_t capacity = 128);

	bool try_pop(T& item);
	bool try_push(T& item);
	void push(T& item);
	T pop();

};