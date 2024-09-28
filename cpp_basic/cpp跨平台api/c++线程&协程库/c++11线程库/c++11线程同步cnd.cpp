//编译:
//		g++ -g3 -std=c++2a -pthread ./c++11线程同步cnd.cpp -o x
//		cl.exe ./c++11线程同步cnd.cpp -EHsc -w -Zi -Ox -link -out:x.exe



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

//判断操作系统类型, 非目标平台, 终止编译
#if defined(_WIN32) || defined(_WIN64)
	#pragma message("Support Windows")
	#include <windows.h>
	//Sleep() 函数改名为: sleep(), 对齐unix
	#define sleep Sleep
#elif defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__) || defined(__unix) || defined(__unix__)
	#pragma message("Support Unix")
	#include <unistd.h>
#else
	#error [ Unknow System Type !! ]
#endif

#include <cstdio>



//
// 定义锁(条件变量需要最普通的互斥锁即可, 不用太复杂的)
//
//<mutex>(类C++11)
std::mutex mtx;
//std::timed_mutex mtx_t;

std::condition_variable cnd;
bool ready = false;


std::atomic<int> call_once_sum;

void call_once_func(void){
	//
	// 定义锁包装器
	//
	//使用'锁包装器+包装锁策略', 条件变量默认必须指定为: 互斥锁 + unique_lock 锁包装器(ps: 锁策略demo 只是使用死等锁做测试, 图的就是简单)
	std::unique_lock<std::mutex> lock_mtx(mtx, std::adopt_lock);
	//std::unique_lock<std::timed_mutex> lock_mtx_t(mtx_t, std::adopt_lock);

	std::chrono::high_resolution_clock::time_point now;
	int tmp = 9999;
	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&call_once_sum,1);				//标准的多线程竞争加法运算
		//printf("call_once_func: call_once_sum = %d\n", (int)call_once_sum);
	}

	//c++ 标准库的锁, 不需要init() 初始化锁, 类构造函数会自动完成init()

	//c++ 条件变量, 不需要init(), 类构造函数会自动完成init()

	cnd.wait(lock_mtx, []{return ready;});						//死等ready = true(调用格式不能更改, c++风格)
	printf("cnd.wait() okay, ready=%d\n",ready);

	cnd.wait_for(lock_mtx, std::chrono::milliseconds(1000), []{return ready;});
	printf("cnd.wait_for() okay, ready=%d\n",ready);	//死等ready = true, 超时1s

	cnd.wait_for(lock_mtx, std::chrono::milliseconds(1000), []{return ready;});
	printf("cnd.wait_for() okay, ready=%d\n",ready);	//死等ready = true, 超时1s(低精度)

	now = std::chrono::high_resolution_clock::now();
	cnd.wait_until(lock_mtx, now + std::chrono::milliseconds(10), []{return ready;});
	printf("cnd.wait_until() okay, ready=%d\n",ready);//死等ready = true, 超时1s(高精度)

	//c++ 条件变量, 不需要destory(), 类析构函数会自动完成destory()

	//c++ 标准库的锁, 不需要destory() 销毁锁, 类析构函数会自动完成destory()

	return;
}

void cnd_test(void){
	std::thread t1,t2;

	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());
	t1 = std::thread(call_once_func);
	t2 = std::thread(call_once_func);
	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());

	sleep(1);

	ready = true;																			//使用前, 必须把条件ready 先设置为true(设置一次即可), 然后再唤醒;
	cnd.notify_one();																	//逐次, 逐个线程唤醒
	cnd.notify_one();

	cnd.notify_all();																	//一次唤醒全部等待线程
	cnd.notify_all();

	t1.join();
	t2.join();
	printf("t1.joinable(): %d\n", t1.joinable());
	printf("t2.joinable(): %d\n", t2.joinable());
	return;
}


int main(void){
	cnd_test();
	return 0;
}



