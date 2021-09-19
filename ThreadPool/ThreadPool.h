// ThreadPool.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once
#include "threadsafe_queue.h"
#include "functionwrapper.h"
#include <memory>
#include <future>


class thread_pool {
	std::atomic<bool> done;
	threadsafe_queue<function_wrapper, 128> work_queue;
	std::vector<std::thread> threads;
	void worker_thread();
public:
	thread_pool();
	~thread_pool();
	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
		using result_type = std::result_of<FunctionType()>::type;
		using ptr_result_type = std::unique_ptr<result_type>;

		std::packaged_task<ptr_result_type()> task(std::move(f));
		std::future<ptr_result_type> res(task.get_future());
		
		work_queue.push(std::move(task));
		auto* resp2 = &res;
		return res;
	}
};
// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
