//编译:
//		g++ -g3 -std=c++2a -pthread ./c++11线程同步mutex简单.cpp -o x
//		cl.exe ./c++11线程同步mutex简单.cpp -EHsc -w -Zi -Ox -link -out:x.exe



//本demo 简介: 把c++ 线程库的mutex, 当成c11 线程库的mutex一样使用, 不考虑'锁包装器+包装锁策略'(手动加锁/解锁, 怎么简单怎么用)



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



//
// 定义锁
//
//<mutex>(类C++11)
std::mutex mtx;
std::timed_mutex mtx_t;
std::recursive_mutex mtx_rec;
std::recursive_timed_mutex mtx_rec_t;

//<shared_mutex>
std::shared_mutex mtx_shared;
std::shared_timed_mutex mtx_shared_t;



std::atomic<int> call_once_sum;

void call_once_func(void){
	std::chrono::high_resolution_clock::time_point now;
	int tmp = 9999;
	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&call_once_sum,1);	//标准的多线程竞争加法运算
		printf("call_once_func: call_once_sum = %d\n", (int)call_once_sum);
	}

	//c++ 标准库的锁, 不需要init() 初始化锁, 锁包装器的类构造函数会自动完成init()

	mtx.lock();																	//简单死等互斥锁(使用: std::mutex)
	mtx.unlock();

	if(mtx.try_lock()){													//简单询问性非阻塞互斥锁(使用: std::mutex)
		printf("try_lock() ok\n");
		mtx.unlock();
	}
	else
		printf("cant try_lock()\n");

	if(mtx_t.try_lock_for(std::chrono::milliseconds(1000))){
		printf("try_lock_for() ok\n");						//简单低精度超时死等互斥锁(使用: std::timed_mutex)
		mtx_t.unlock();
	}
	else
		printf("cant try_lock_for()\n");

	now = std::chrono::high_resolution_clock::now();
	if(mtx_t.try_lock_until(now + std::chrono::milliseconds(10))){
		printf("try_lock_until() ok\n");					//简单高精度超时死等互斥锁(使用: std::timed_mutex) -- 少用
		mtx_t.unlock();
	}
	else
		printf("cant try_lock_until()\n");

	//c++ 标准库的锁, 不需要destory() 销毁锁, 锁包装器的类析构函数会自动完成destory()

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



