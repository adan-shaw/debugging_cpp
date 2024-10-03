//编译:
//		g++ -g3 ./io_service.cpp -o x -pthread -l boost_system -l boost_thread -l boost_chrono



//1.io_service 的简介和常见用途
/*
	io_service 对象是asio框架中的调度器, 它的用途有很多, 例如:
		* socket io事件中, 可以令所有异步io事件, 都是通过io_service 来分发处理的:
			asio::io_service io_service;
			asio::ip::tcp::socket socket(io_service);

		* 在std::thread 线程pool 管理中, 可以用io_servie 实现了一个任务队列, 这里的任务就是: 函数指针void func(void)的任务

		...

	当然的, 无论是socket fd 异步io_servie 事件调度器, 还是std::thread 线程pool 管理中的io_servie 任务调度器,
	本质上, io_service 实现的功能, 就是一个'函数指针void func(void)的任务'调度器, 
	所以尤其适合有回调函数的场合, 或者有任务队列的场合;

	有io_service 帮你实现业务逻辑, 省得你自己手捏一个这么麻烦!!
*/



//2.io_service 常见的编程API
/*
	(略)
*/



#include "io_service.h"

#define pth_sleeptime_count (1)

int main(void)
{
	task taskApp;
	taskApp.setTask();
	while (1){
		taskApp.post_task("hello");
		std::this_thread::sleep_for(std::chrono::seconds(pth_sleeptime_count));
	}
}



