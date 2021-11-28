#include "ThreadPool.h"

void thread_pool::worker_thread() {
	while (!done) {
		std::shared_ptr<function_wrapper> task = work_queue.try_pop();
		if (task) {
			task.get()->operator()();
		}
		else
			std::this_thread::yield();
	}
}

thread_pool::thread_pool() : work_queue(), done(false) {

	auto thread_count = std::thread::hardware_concurrency();

	try {
		for (auto i = 0u; i < thread_count; i++) {
			threads.push_back(std::thread(&thread_pool::worker_thread,this));
		}
	}
	catch (...) {
		done = true;
		throw;
	}
}

thread_pool::~thread_pool() {
	done = true;
	for (auto i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}