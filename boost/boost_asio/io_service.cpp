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



#include <string>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <deque>
#include <mutex>



class task
{
public:
	task();
	~task() {};
	void init();
	void poll();
	void run();
	void post(std::string str);
	void handle_task(const boost::system::error_code& error);
protected:
	boost::asio::io_service m_ioServer;
	std::deque<std::string> m_deque;
	boost::asio::deadline_timer m_taskTimer;
	int m_taskTimeOut;
	std::mutex m_lock;
};

task::task():m_taskTimer(m_ioServer),
	m_taskTimeOut(5)
{
}

void task::init()
{
	m_taskTimer.expires_from_now(boost::posix_time::seconds(m_taskTimeOut));
	m_taskTimer.async_wait(boost::bind(&task::handle_task, this, boost::asio::placeholders::error));
}

void task::poll()
{
	m_ioServer.poll();
}

void task::run()
{
	m_ioServer.run();
}

void task::handle_task(const boost::system::error_code& error)
{
	if (!error){
		while (!m_deque.empty()) {
			std::lock_guard<std::mutex> mut(m_lock);
			std::string str = m_deque.front();
			printf("task work %s\n", str.c_str());
		}
		m_taskTimer.expires_from_now(boost::posix_time::seconds(m_taskTimeOut));
		m_taskTimer.async_wait(boost::bind(&task::handle_task, this, boost::asio::placeholders::error));
	}
}

void task::post(std::string str)
{
	std::lock_guard<std::mutex> mut(m_lock);
	m_deque.push_back(str);
}



class TaskApp {
typedef std::shared_ptr<task> taskRef;

public:
	TaskApp() {};
	~TaskApp() {};

	void setTask() {
		mTaskRef = taskRef(new task());
		mTaskRef->init();
		boost::thread thrd(&TaskApp::run, this);
	}

	void run() {
		//同步阻塞方式
		mTaskRef->run();
		//异步方式
		/*
		while (1)
		{
			mTaskRef->poll();
		}*/
	}

	void post_task(std::string str)
	{
		mTaskRef->post(str);
	}

protected:
	taskRef mTaskRef;
};



int main(void)
{
	TaskApp taskApp;
	taskApp.setTask();
	while (1){
		taskApp.post_task("hello");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}



