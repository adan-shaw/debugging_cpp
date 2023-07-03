//编译:
//		g++ -g3 -pthread ./c++11原子操作.cpp -o x



//判断编译器是否支持c++11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support c++11 <atomic> ]
#else
	#pragma message("Support c++11 <atomic>")
	#include <atomic>
#endif

#ifdef __STDC_NO_THREADS__
	#error [ Not Support c++11 <thread> ]
#else
	#pragma message("Support c++11 <thread>")
	#include <thread>
	#include <mutex>
#endif

#include <cstdio>

//c++11 原子操作
/*
原子库为细粒度的原子操作提供组件, 允许无锁并发编程;
涉及同一对象的每个原子操作, 相对于任何其他原子操作是不可分的;原子对象不具有数据竞争;
定义于头文件<atomic>

原子类型
	atomic												atomic类模板及其针对布尔、整型和指针类型的特化(类模板C++11)
	atomic_ref										提供非原子对象上的原子操作(类模板C++20)

原子类型上的操作
	atomic_is_lock_free						检查对该原子类型的操作是否是无锁的(函数模板C++11)
	atomic_store
	atomic_store_explicit					原子地以非原子实参替换原子对象的值(函数模板C++11)
	atomic_load
	atomic_load_explicit					原子地获得存储于原子对象的值(函数模板C++11)
	atomic_exchange
	atomic_exchange_explicit			原子地以非原子实参的值替换原子对象的值, 并返回该原子对象的旧值(函数模板C++11)
	atomic_compare_exchange_weak	原子地比较原子对象和非原子实参的值,若相等则进行atomic_exchange,若不相等则进行atomic_load(函数模板C++11)
	atomic_compare_exchange_weak_explicit
	atomic_compare_exchange_strong
	atomic_compare_exchange_strong_explicit
	atomic_fetch_add
	atomic_fetch_add_explicit			将非原子值加到原子对象, 并获得原子对象的先前值(函数模板C++11)
	atomic_fetch_sub
	atomic_fetch_sub_explicit			从原子对象减去非原子值, 并获得原子对象的先前值(函数模板C++11)
	atomic_fetch_and
	atomic_fetch_and_explicit			将原子对象替换为与非原子实参逻辑与的结果, 并获得原子对象的先前值(函数模板C++11)
	atomic_fetch_or
	atomic_fetch_or_explicit			将原子对象替换为与非原子实参逻辑或的结果, 并获得原子对象的先前值(函数模板C++11)
	atomic_fetch_xor
	atomic_fetch_xor_explicit			将原子对象替换为与非原子实参逻辑异或的结果, 并获得原子对象的先前值(函数模板C++11)
	atomic_wait
	atomic_wait_explicit					阻塞线程直至被提醒且原子值更改(函数模板C++20)
	atomic_notify_one							提醒一个在atomic_wait 中阻塞的线程(函数模板C++20)
	atomic_notify_all							提醒所有在atomic_wait 中阻塞的线程(函数模板C++20)

标志类型及操作
	atomic_flag										免锁的布尔原子类型(类C++11)
	atomic_flag_test_and_set			原子地设置标志为true 并返回其先前值(函数C++11)
	atomic_flag_test_and_set_explicit
	atomic_flag_clear
	atomic_flag_clear_explicit		原子地设置标志值为false(函数C++11)
	atomic_flag_test
	atomic_flag_test_explicit			原子地返回标志的值(函数C++20)
	atomic_flag_wait
	atomic_flag_wait_explicit			阻塞线程, 直至被提醒且标志更改(函数模板C++20)
	atomic_flag_notify_one				提醒一个在atomic_flag_wait 中阻塞的线程(函数模板C++20)
	atomic_flag_notify_all				提醒所有在atomic_flag_wait 中阻塞的线程(函数模板C++20)

初始化
	atomic_init										对默认构造的原子对象进行非原子初始化(函数模板C++11)
	ATOMIC_VAR_INIT								静态存储期的原子对象的常量初始化(宏函数C++11)
	ATOMIC_FLAG_INIT							将std::atomic_flag 初始化为false(宏常量C++11)

内存同步顺序
	memory_order									为给定的原子操作定义内存顺序制约(枚举C++11)
	kill_dependency								从std::memory_order_consume 依赖树移除指定对象(函数模板C++11)
	atomic_thread_fence						通用的依赖内存顺序的栅栏同步原语(函数C++11)
	atomic_signal_fence						线程与执行于同一线程的信号处理函数间的栅栏(函数C++11)
*/



//
// 1.call_once() 测试
//
std::atomic<int> call_once_sum;

void call_once_func(void){
	int tmp = 9999;
	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&call_once_sum,1);	//标准的多线程竞争加法运算
		//call_once_sum+=1;												//不标准的加法
		printf("call_once_func: call_once_sum = %d\n", (int)call_once_sum);
		//printf("std::atomic<int> call_once_sum is lock free? %s\n", atomic_is_lock_free(&call_once_sum) ? "true" : "false");
	}
	return;
}

//函数原型: void call_once( once_flag* flag, void (*func)(void) );
void call_once_test(void){
	call_once_sum = 0;													//初始化全局原子int 变量
	std::once_flag flag;												//一个函数, 一个once_flag; 调用一次后就会失效, 不能调用第二次;
	printf("call_once_func go\n");							//once_flag + call_once(), 可以保证多个线程对某个函数只调用一次;
	std::call_once(flag, call_once_func);
	printf("call_once_func go again(not gonna run again)\n");
	std::call_once(flag, call_once_func);
	return ;
}



//
// 2.多线程抢夺原子变量test
//
void many_thrd_killing_eachother(void){
	std::thread t1, t2, t3, t4;

	printf("t1.joinable(): %d\n", t1.joinable());//创建std::thread 类, 默认线程是false 不活跃状态的
	printf("t2.joinable(): %d\n", t2.joinable());
	printf("t3.joinable(): %d\n", t3.joinable());
	printf("t4.joinable(): %d\n", t4.joinable());

	t1 = std::thread(call_once_func);						//重新调用构造函数, 传入线程函数&参数, 激活线程
	t2 = std::thread(call_once_func);
	t3 = std::thread(call_once_func);
	t4 = std::thread(call_once_func);

	printf("t1.joinable(): %d\n", t1.joinable());//检查线程状态again
	printf("t2.joinable(): %d\n", t2.joinable());
	printf("t3.joinable(): %d\n", t3.joinable());
	printf("t4.joinable(): %d\n", t4.joinable());

	t1.join();																	//等待线程结束
	t2.join();
	t3.join();
	t4.join();

	printf("t1.joinable(): %d\n", t1.joinable());//检查线程状态again
	printf("t2.joinable(): %d\n", t2.joinable());
	printf("t3.joinable(): %d\n", t3.joinable());
	printf("t4.joinable(): %d\n", t4.joinable());

	return;
}



//
// 3.原子操作赋值exchange
//
void many_thrd_exchange(int arg){
	int tmp = 9999;

	//赋值(在原子操作中, 赋值需要用atomic_exchange()来操作)
	std::atomic_exchange(&call_once_sum, arg);

	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&call_once_sum,1);
		printf("many_thrd_exchange: call_once_sum = %d\n", (int)call_once_sum);
	}
	return;
}



int main(void){
	call_once_test();
	many_thrd_killing_eachother();
	many_thrd_exchange(800);
	return 0;
}



