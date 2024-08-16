//编译:
//		g++ -g3 -std=c++2a -pthread ./c++11线程同步mutex.cpp -o x
//		cl.exe ./c++11线程同步mutex.cpp -EHsc -w -Zi -Ox -link -out:x.exe



//判断编译器是否支持c++11 线程库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 <thread> ]
#else
	#pragma message("Support C11 <thread>")
	#include <thread>
	#include <mutex>
	#include <shared_mutex>
	//std::chrono::system_clock 头文件
	#include <chrono>
	//std::put_time 头文件
	#include <iomanip>
#endif

//判断编译器是否支持c++11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 <atomic> ]
#else
	#pragma message("Support C11 <atomic>")
	#include <atomic>
#endif

#include <cstdio>



//c++ 互斥锁库简介
/*
	c++ 线程库支持多种互斥锁, 
	而且都是类封装的互斥锁, 操作API 都是通用抽象API, 每一种锁的操作API 都是一样的, 只是锁的属性不一样;
	不同类型的互斥锁list:
	<mutex>(类C++11)							(条件变量需要最普通的mutex 互斥锁即可, 不用太复杂的)
		mutex											基础互斥锁 [抢占者阻塞死等, 使用者无限期使用]
		timed_mutex								基础互斥锁+超时拓展 [抢占者阻塞死等+超时放弃等待, 使用者无限期使用]
		recursive_mutex						提供能被同一线程递归锁定的互斥设施 [抢占者阻塞死等, 使用者有限期使用]
		recursive_timed_mutex			提供能被同一线程递归锁定的互斥设施, 并实现有时限锁定 [抢占者阻塞死等+超时放弃等待, 使用者有限期使用]

	<shared_mutex>							适合一个线程写, 其他线程都是读的情况, 提高效率;
		shared_mutex							提供共享互斥设施(类C++17)
		shared_timed_mutex				提供共享互斥设施并实现有时限锁定(类C++14)


	锁包装器(锁策略包装器: 使用锁包装器, 即默认使用自动解锁):
		c++ 标准锁, 默认是没有锁策略, 也没有包装器的, 非常接近于posix mutex 锁, 需要自己上锁, 自己解锁;
		使用锁包装器类模板, 进行二次包装, 可以做到:
				只需加锁即可, 解锁会自动解锁, 跟智能指针一样;
		具体锁包装器如下(智能解锁):
		lock_guard								实现严格基于作用域的互斥体所有权包装器(C++11)
		scoped_lock								用于多个互斥体的免死锁RAII 封装器(C++17)
		unique_lock								实现可移动的互斥体所有权包装器, 条件变量专用(C++11)
		shared_lock								实现可移动的共享互斥体所有权封装器, <shared_mutex>系列专用(C++14)
		使用时(注意):
			锁包装器, 是不能多线程共享, 而是每个线程必须独享一个包装器类!!
			锁倒是可以多线程共享, 锁也必须是多线程共享, 才能正常工作的;
			但锁包装器是不能多线程共享的, 详情请看: c++11线程同步cnd.cpp


	锁策略(通用C++11)
		defer_lock_t							通用锁策略类实体(如果你要保存锁实体, 方便传递, 则使用这个)
		try_to_lock_t
		adopt_lock_t

															通用锁策略常量(如果你不需要保存锁实体, 默认不更改锁策略, 就用这个)
		defer_lock								不获得互斥的所有权[用处不明]
		try_to_lock								尝试获得互斥的所有权而不阻塞(不阻塞策略)
		adopt_lock								假设调用方线程已拥有互斥的所有权(阻塞策略,死等)

	注意:
		锁策略类实体/锁策略常量, 功能都一样, 只是看你有没有保存/传递锁策略的需求, 
		有就用锁策略类实体, 没有就用锁策略常量;
		推荐使用: 锁策略常量
*/



//c++ 互斥锁库本质探讨
/*
	本质上, c++ 互斥锁库都是逻辑锁?
	即c++ 互斥锁库是不会切换线程的, 跟CAS 逻辑锁/自旋锁一样, 死等, cpu 消耗巨大?

	还是c++ 互斥锁库是会切换线程的, 跟posix mutex 一样, 一旦陷入资源抢夺, 就会挂起线程, 节约cpu 开销?
	网络上面没有描述, 希望是cpu 挂起, 因为这样其实性能更好;

	c++ 标准线程库, 现在也有通用线程挂起操作, 
	因此, 说不准c++ 互斥锁库, 具体支撑实现方式是那种;

	极度有可能是通过调节: 锁包装器+锁策略, 来实现CAS 逻辑锁(cpu死等), or posix mutex 等待挂起锁(节约cpu 资源);
	具体看业务需求:
		实时业务, 肯定希望CAS 逻辑锁(cpu死等);
		分时业务, 肯定希望posix mutex 等待挂起锁(节约cpu 资源);
*/



//应对多个c++ 互斥锁同时上锁的业务需要:
/*
	实际上这种情况, 极其变态, 一般很少应用需要这么复杂的加锁方式的, 
	除非锁区内的资源访问非常慢, 导致你要加两把锁(如果慢资源, 根本就不应该放入互斥锁临界区内, 这是怎么想的?)

	所以, 原则上, 应该自己裁减临界区内的资源, 尽量做到一把锁解决问题;
	如果实在是业务逼得你没办法, 你可以采取两把锁的加锁方式, 一把快锁, 一把慢锁的方式(尽量少干这种事, 非常烧脑)

	c++ 标准编码手册上面有2 把锁的加锁demo 和策略, 实在迫不得已, 你可以这么干, 但实在是不推荐你这么做;
*/



//关于使用智能解锁, 还是手动解锁
/*
	实际上还是看业务需求, 如果加锁场景比较复杂, 解锁场景比较复杂, 自动解锁有优势;
	尤其是多把锁的情况下, 只管理加锁即可, 解锁让它自己解锁, 这是智能解锁的好处, c++ 的好处;
	普通一把锁, 自己加锁, 解锁, 其实语义上更明确;

	使用锁包装器, 即默认使用自动解锁, 复杂-多把锁业务, 推荐使用;
*/



//
// 定义锁
//
//<mutex>(类C++11)
std::mutex m_mtx;
std::timed_mutex m_mtx_t;
std::recursive_mutex m_mtx_rec;
std::recursive_timed_mutex m_mtx_rec_t;

//<shared_mutex>
std::shared_mutex m_mtx_shared;
std::shared_timed_mutex m_mtx_shared_t;



std::atomic<int> call_once_sum;

void call_once_func(void){
	//
	// 定义锁包装器
	//
	//使用锁包装器, 包装锁策略(锁策略demo 只使用死等锁做测试, 简单)
	std::lock_guard<std::mutex> m_mtx_lock(m_mtx, std::adopt_lock);
	std::lock_guard<std::timed_mutex> m_mtx_t_lock(m_mtx_t, std::adopt_lock);
	std::lock_guard<std::recursive_mutex> m_mtx_rec_lock(m_mtx_rec, std::adopt_lock);
	std::lock_guard<std::recursive_timed_mutex> m_mtx_rec_t_lock(m_mtx_rec_t, std::adopt_lock);

	//共享锁+普通互斥锁策略
	std::lock_guard<std::shared_mutex> m_mtx_shared_lock(m_mtx_shared, std::adopt_lock);
	std::lock_guard<std::shared_timed_mutex> m_mtx_shared_t_lock(m_mtx_shared_t, std::adopt_lock);
	//共享锁+专用共享锁策略
	std::shared_lock<std::shared_mutex> m_mtx_shared_lock2(m_mtx_shared, std::adopt_lock);
	std::shared_lock<std::shared_timed_mutex> m_mtx_shared_t_lock2(m_mtx_shared_t, std::adopt_lock);

	std::chrono::high_resolution_clock::time_point now;
	int tmp = 9999;
	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&call_once_sum,1);	//标准的多线程竞争加法运算
		printf("call_once_func: call_once_sum = %d\n", (int)call_once_sum);
	}

	//c++ 标准库的锁, 不需要init() 初始化锁, 类构造函数会自动完成init()

	m_mtx.lock();																//简单死等互斥锁(使用: std::mutex)
	m_mtx.unlock();

	if(m_mtx.try_lock()){												//简单询问性非阻塞互斥锁(使用: std::mutex)
		printf("try_lock() ok\n");
		m_mtx.unlock();
	}
	else
		printf("cant try_lock()\n");

	if(m_mtx_t.try_lock_for(std::chrono::milliseconds(1000))){
		printf("try_lock_for() ok\n");						//简单低精度超时死等互斥锁(使用: std::timed_mutex)
		m_mtx_t.unlock();
	}
	else
		printf("cant try_lock_for()\n");

	now = std::chrono::high_resolution_clock::now();
	if(m_mtx_t.try_lock_until(now + std::chrono::milliseconds(10))){
		printf("try_lock_until() ok\n");					//简单高精度超时死等互斥锁(使用: std::timed_mutex) -- 少用
		m_mtx_t.unlock();
	}
	else
		printf("cant try_lock_until()\n");

	//c++ 标准库的锁, 不需要destory() 销毁锁, 类析构函数会自动完成destory()

	return;
}

void mutex_test(void){
	std::thread t1,t2;

	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());
	t1 = std::thread(call_once_func);
	t2 = std::thread(call_once_func);
	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());
	t1.join();
	t2.join();
	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());
	return;
}


int main(void){
	mutex_test();
	return 0;
}



