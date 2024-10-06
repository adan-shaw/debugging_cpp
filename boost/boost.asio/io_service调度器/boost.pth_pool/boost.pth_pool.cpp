//编译:
//		g++ -g3 ./boost.pth_pool.cpp -o x -pthread -l boost_system -l boost_thread -l boost_chrono



#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <future>
#include "boost.pth_pool.h"



static const size_t CONCURRENCY = 8;//任务数量尽量比线程数量多



int main (void)
{
	std::cout << "***boost::asio io_service 调度器 + std::thread = pth_pool***\n" << "Benchmark job reposting of " << std::endl;

	//获取硬件的cpu 个数
	size_t workers_count = std::thread::hardware_concurrency ();
	if (0 == workers_count){
		workers_count = 1;
	}

	//初始化线城池的max 数量(固定线城池, 减少线程切换) [workers_count != CONCURRENCY]
	pthPool thPool (workers_count);

	//初始化pth 任务数量(产生一定的线程抢占, 访问冲突, 最终由io_service 调度器解决访问冲突问题)
	std::promise<void> waiters[CONCURRENCY];

	//逐个任务启动
	for (std::promise<void> & waiter:waiters){
		thPool.post (RepostJob (&thPool, &waiter));
	}

	//逐个等待任务结束
	for (std::promise<void> & waiter:waiters){
		waiter.get_future ().wait ();
	}

	return 0;
}
