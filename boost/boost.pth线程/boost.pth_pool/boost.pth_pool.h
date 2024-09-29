#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <memory>

#include <boost/asio.hpp>



class asioPthPool
{
public:
	inline asioPthPool (size_t pth_pool_max);
	inline ~ asioPthPool ()
	{
		stop ();
	}

	inline void joinPthPool ();

	template < typename Handler > inline void post (Handler && handler)
	{
		asio_io_service.post (handler);
	}

private:
	inline void start ();
	inline void stop ();
	inline void pth_func_worker ();//设置线程池执行的线程函数

	boost::asio::io_service asio_io_service;
	std::unique_ptr < boost::asio::io_service::work > asio_io_service_work;

	std::vector < std::thread > pth_pool;
};

inline asioPthPool::asioPthPool (size_t pth_pool_max):pth_pool (pth_pool_max)
{
	start ();
}

inline void asioPthPool::start ()
{
	asio_io_service_work.reset (new boost::asio::io_service::work (asio_io_service));

	//从线程池中取出每个std::thread, 逐个启动
	for (std::thread & pth_tmp:pth_pool)
	{
		pth_tmp = std::thread (&asioPthPool::pth_func_worker, this);
	}

}

inline void asioPthPool::stop ()
{
	asio_io_service_work.reset ();
	asio_io_service.stop ();

	//从线程池中取出每个std::thread, 逐个停止(阻塞等待)
	for (std::thread & pth_tmp:pth_pool)
	{
		if (pth_tmp.joinable ())
		{
			pth_tmp.join ();
		}
	}
}

inline void asioPthPool::joinPthPool ()
{
	asio_io_service.run ();
}

inline void asioPthPool::pth_func_worker ()
{
	joinPthPool ();
}



//Heavy info 统计结构体(带debugging 函数, 实现变量+代码的自我管理, c 语言的struct 竟然可以被用到这个份上, 重新捏造一个c 语言类)
struct Heavy
{
	bool verbose;
	std::vector < char >resource;

	//伪构造函数
	Heavy (bool verbose = false):verbose (verbose), resource (100 * 1024 * 1024)
	{
		if (verbose)
		{
			std::cout << "heavy default constructor" << std::endl;
		}
	}

	Heavy (const Heavy & o):verbose (o.verbose), resource (o.resource)
	{
		if (verbose)
		{
			std::cout << "heavy copy constructor" << std::endl;
		}
	}

	Heavy (Heavy && o):verbose (o.verbose), resource (std::move (o.resource))
	{
		if (verbose)
		{
			std::cout << "heavy move constructor" << std::endl;
		}
	}

	Heavy & operator== (const Heavy & o)
	{
		verbose = o.verbose;
		resource = o.resource;
		if (verbose)
		{
			std::cout << "heavy copy operator" << std::endl;
		}
		return *this;
	}

	Heavy & operator== (const Heavy && o)
	{
		verbose = o.verbose;
		resource = std::move (o.resource);
		if (verbose)
		{
			std::cout << "heavy move operator" << std::endl;
		}
		return *this;
	}

	~Heavy ()
	{
		if (verbose)
		{
			std::cout << "heavy destructor. " << (resource.size ()? "Owns resource" : "Doesn't own resource") << std::endl;
		}
	}
};



#define REPOST_COUNT (1000000)
struct RepostJob
{
	//Heavy heavy;

	asioPthPool *asio_thread_pool;

	volatile size_t counter;
	long long int begin_count;
	std::promise < void >*waiter;

	RepostJob (asioPthPool * asio_thread_pool, std::promise < void >*waiter): asio_thread_pool (asio_thread_pool), counter (0), waiter (waiter)
	{
		begin_count = std::chrono::high_resolution_clock::now ().time_since_epoch ().count ();
	}

	void operator () ()
	{
		if (++counter < REPOST_COUNT)
		{
			if (asio_thread_pool)
			{
				asio_thread_pool->post (*this);
				return;
			}
		}
		else
		{
			long long int end_count = std::chrono::high_resolution_clock::now ().time_since_epoch ().count ();
			std::cout << "reposted " << counter << " in " << (double) (end_count - begin_count) / (double) 1000000 << " ms" << std::endl;
			waiter->set_value ();
		}
	}
};
