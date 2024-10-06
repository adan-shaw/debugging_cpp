#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <memory>

#include <boost/asio.hpp>

//简介
/*
	本boost.pth_pool 模块, 主要是利用boost::asio::io_service 调度器 + std::thread 标准线程来实现线城池demo;
	要阅读本demo 代码, 首先要会用:
		* boost::asio::io_service 调度器
		* std::thread 库(锁包装器也要会)
*/



class pthPool{
public:
	//初始化线城池的数量(也是线城池的最大值)
	inline pthPool (size_t pth_pool_max) : pth_pool (pth_pool_max){
		start ();
	}
	inline ~pthPool (){
		stop ();
	}
	inline void joinPthPool (void){
		ioService.run ();
	}
	template <typename Handler> inline void post (Handler && handler){
		ioService.post (handler);
	}

private:
	boost::asio::io_service ioService;
	std::unique_ptr <boost::asio::io_service::work> ioService_work;
	std::vector <std::thread> pth_pool;

	inline void start (void)
	{
		ioService_work.reset (new boost::asio::io_service::work (ioService));

		for (std::thread& pth_tmp:pth_pool){//从线程池中取出每个std::thread, 逐个启动
			pth_tmp = std::thread (&pthPool::pth_func_worker, this);
		}
	}

	inline void stop (void)
	{
		ioService_work.reset ();
		ioService.stop ();

		for (std::thread& pth_tmp:pth_pool){//从线程池中取出每个std::thread, 逐个停止(阻塞等待)
			if (pth_tmp.joinable ()){
				pth_tmp.join ();
			}
		}
	}

	inline void pth_func_worker (void){//线程池统一执行的线程函数(加入线程池=io_service->run())
		joinPthPool ();
	}
};





// 每个任务要执行的指令个数count (数量越大, 任务阻塞的时间越长)
#define REPOST_COUNT (100000)

struct RepostJob{
	pthPool *thPool;

	volatile size_t counter;
	long long int time_count_begin;
	std::promise<void> *waiter;

	RepostJob (pthPool * tmp_thPool, std::promise<void>* tmp_waiter) : 
		thPool (tmp_thPool), 
		counter (0), 
		waiter (tmp_waiter)
	{
		time_count_begin = std::chrono::high_resolution_clock::now ().time_since_epoch ().count ();
	}

	void operator () ()
	{
		long long int time_count_end;
		if (++counter < REPOST_COUNT){
			if (thPool){
				thPool->post (*this);//递归执行空任务, 消耗线城池的用量
				return;
			}
		}
		else{
			time_count_end = std::chrono::high_resolution_clock::now ().time_since_epoch ().count ();
			std::cout << "reposted " << counter << " in " << (double) (time_count_end - time_count_begin) / (double) 1000000 << " ms" << std::endl;
			waiter->set_value ();
		}
	}
};



