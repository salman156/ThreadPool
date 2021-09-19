#include <iostream>
#include <thread>
#include "ThreadPool.h"

void stat(std::unique_ptr<int> asd) {
	
}
int main()
{
	thread_pool pool;

	std::unique_ptr<int> it(new int(12));

	stat(std::move(it));


	//auto it = pool.submit(foo);

	//it.wait();

	//std::chrono::milliseconds time = it.get();

	return 0;
}
