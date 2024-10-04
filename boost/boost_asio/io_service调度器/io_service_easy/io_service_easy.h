#include <string>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <deque>
#include <mutex>



class taskPool
{
public:
	taskPool() : taskTimer(ioService), taskTimeout(5) {}
	~taskPool() {}

	void init(void)
	{
		taskTimer.expires_from_now(boost::posix_time::seconds(taskTimeout));
		taskTimer.async_wait(boost::bind(&taskPool::handle_task, this, boost::asio::placeholders::error));
	}

	void poll(void)
	{
		ioService.poll();
	}

	void run(void)
	{
		ioService.run();
	}

	void post(std::string str)
	{
		std::lock_guard<std::mutex> mut(mtx);
		deq.push_back(str);
	}

	void handle_task(const boost::system::error_code& error)
	{
		if (!error){
			while (!deq.empty()) {
				std::string str = pop_deq_task();
				printf("handle_task(): %s\n", str.c_str());
			}
			taskTimer.expires_from_now(boost::posix_time::seconds(taskTimeout));
			taskTimer.async_wait(boost::bind(&taskPool::handle_task, this, boost::asio::placeholders::error));
		}
	}

protected:
	boost::asio::io_service ioService;
	std::deque<std::string> deq;
	boost::asio::deadline_timer taskTimer;
	int taskTimeout;
	std::mutex mtx;

private:
	std::string pop_deq_task(void)
	{
		std::lock_guard<std::mutex> tmp(mtx);
		std::string str = deq.front();
		deq.pop_front();//先进先出
		return str;
	}
};





class task {
public:
	task() {};
	~task() {};

	void setTask(void)
	{
		ptaskPool = std::shared_ptr<taskPool>(new taskPool());
		ptaskPool->init();
		boost::thread thrd(&task::run, this);
	}

	void run(void)//同步阻塞方式
	{
		ptaskPool->run();
	}

	void runEx(void){//异步非阻塞方式(未完善)
		while (1){
			ptaskPool->poll();
		}
	}

	void post_task(std::string str)
	{
		ptaskPool->post(str);
	}

protected:
	std::shared_ptr<taskPool> ptaskPool;
};
