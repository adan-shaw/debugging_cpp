//编译:
//		g++ -g3 ./boost.pth_pool.cpp -o x -pthread -l boost_system -l boost_thread -l boost_chrono



#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <future>
#include "boost.pth_pool.h"


static const size_t CONCURRENCY = 16;



int main (void)
{
	std::cout << "Benchmark job reposting of ***asio thread pool***" << std::endl;

	size_t workers_count = std::thread::hardware_concurrency ();//获取硬件的cpu 个数
	if (0 == workers_count)
	{
		workers_count = 1;
	}

	asioPthPool asio_thread_pool (workers_count);

	std::promise < void >waiters[CONCURRENCY];

	for (auto & waiter:waiters)
	{
		asio_thread_pool.post (RepostJob (&asio_thread_pool, &waiter));
	}

	for (auto & waiter:waiters)
	{
		waiter.get_future ().wait ();
	}

	return 0;
}
