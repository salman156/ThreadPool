#pragma once
#include <atomic>
#include <array>
#include <memory>
#include <thread>
template<typename T, std::size_t capacity>
class threadsafe_queue{
	std::array<std::atomic<T*>,capacity> arr;
	std::atomic<std::size_t> offers;
	std::atomic<std::size_t> polls;
public:
	threadsafe_queue();

	std::shared_ptr<T> try_pop();
	std::shared_ptr<T> try_push(T& item);
	void push(T& item);
	void push(T&& item);
	T pop();

};