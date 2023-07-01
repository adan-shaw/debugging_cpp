//编译:
//		g++ -std=c++11 -g3 -pthread ./c++11线程库.cpp -o x



//判断编译器是否支持c++11 原子操作库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support c++11 <thread> ]
#else
	#pragma message("Support c++11 <thread>")
	#include <thread>
	//std::chrono::system_clock 头文件
	#include <chrono>
	//std::put_time 头文件
	#include <iomanip>
#endif

#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support c++11 <atomic> ]
#else
	#pragma message("Support c++11 <atomic>")
	#include <atomic>
#endif

#include <cstdio>
#include <iostream>

//c++11 线程库
/*
C++ 包含线程、互斥、条件变量和future的内建支持;

线程<thread>
	thread				管理单独的线程(类C++11)
	jthread				有自动合并和取消支持的std::thread(类C++20)

管理当前线程的函数<thread>
	yield					建议实现重新调度各执行线程(函数C++11)
	get_id				返回当前线程的线程id(函数C++11)
	sleep_for			使当前线程的执行停止指定的时间段(函数C++11)
	sleep_until		使当前线程的执行停止直到指定的时间点(函数C++11)

线程取消<stop_token>(C++20起)
	stop_token		查询是否已经做出std::jthread 取消请求的接口(类C++20)
	stop_source		表示请求停止一个或多个std::jthread 的类(类C++20)
	stop_callback	在std::jthread 取消上注册回调的接口(类模板C++20)

缓存大小访问<new>(C++17起)
	hardware_destructive_interference_size	避免假共享的最小偏移
	hardware_constructive_interference_size	促使真共享的最大偏移

互斥<mutex>
	mutex					提供基本互斥设施(类C++11)
	timed_mutex		提供互斥设施, 实现有时限锁定(类C++11)
	recursive_mutex						提供能被同一线程递归锁定的互斥设施(类C++11)
	recursive_timed_mutex			提供能被同一线程递归锁定的互斥设施, 并实现有时限锁定(类C++11)

共享互斥锁<shared_mutex>
	shared_mutex							提供共享互斥设施(类C++17)
	shared_timed_mutex				提供共享互斥设施并实现有时限锁定(类C++14)

通用互斥管理<mutex>
	lock_guard		实现严格基于作用域的互斥体所有权包装器(类模板C++11)
	scoped_lock		用于多个互斥体的免死锁RAII 封装器(类模板C++17)
	unique_lock		实现可移动的互斥体所有权包装器(类模板C++11)
	shared_lock		实现可移动的共享互斥体所有权封装器(类模板C++14)
	defer_lock_t
	try_to_lock_t
	adopt_lock_t	用于指定锁定策略的标签类型(类C++11)
	defer_lock
	try_to_lock
	adopt_lock		用于指定锁定策略的标签常量(常量C++11)

通用锁定算法<mutex>
	try_lock			试图通过重复调用try_lock 获得互斥体的所有权(函数模板C++11)
	lock					锁定指定的互斥体, 若任何一个不可用则阻塞(函数模板C++11)

单次调用<mutex>
	once_flag			确保call_once 只调用函数一次的帮助对象(类C++11)
	call_once			仅调用函数一次, 即使从多个线程调用(函数模板C++11)

条件变量<condition_variable>
	condition_variable				提供与std::unique_lock 关联的条件变量(类C++11)
	condition_variable_any		提供与任何锁类型关联的条件变量(类C++11)
	notify_all_at_thread_exit	安排到在此线程完全结束时对notify_all 的调用(函数C++11)
	cv_status									列出条件变量上定时等待的可能结果(枚举C++11)

信号量<semaphore>(C++20起)
	counting_semaphore				实现非负资源计数的信号量(类模板C++20)
	binary_semaphore					仅拥有二个状态的信号量(类C++20)

闩<latch>
	latch					单次使用的线程屏障(类C++20)

屏障<barrier>
	barrier				可复用的线程屏障(类模板C++20)

Future<future>
	标准库提供了一些工具来获取异步任务(即在单独的线程中启动的函数)的返回值, 并捕捉其所抛出的异常;
	这些值在共享状态中传递, 其中异步任务可以写入其返回值或存储异常, 
	而且可以由持有该引用该共享态的std::future或std::shared_future实例的线程检验、等待或是操作这个状态;

	promise				存储一个值以进行异步获取(类模板C++11)
	packaged_task	打包一个函数, 存储其返回值以进行异步获取(类模板C++11)
	future				等待被异步设置的值(类模板C++11)
	shared_future	等待被异步设置的值(可能为其他future 所引用)(类模板C++11)
	async					异步运行一个函数(有可能在新线程中执行), 并返回保有其结果的std::future(函数模板C++11)
	launch				指定std::async 所用的运行策略(枚举C++11)
	future_status	指定在std::future 和std::shared_future 上的定时等待的结果(枚举C++11)
Future错误<future>
	future_error							报告与future 或promise 有关的错误(类C++11)
	future_category						鉴别future 错误类别(函数C++11)
	future_errc								鉴别future 错误码(枚举C++11)
*/



std::atomic<int> sum;

int thrd_func(int arg){
	struct timespec tt;
	int tmp = 9999;
	std::atomic_exchange(&sum, arg);		//赋值(在原子操作中, 赋值需要用atomic_exchange()来操作)
	for(;tmp > 0;tmp--){
		std::atomic_fetch_add(&sum,1);
		printf("thrd_func: sum = %d\n", (int)sum);
	}

	std::this_thread::yield();					//立即挂起线程(this_thread = 本线程自身的类, 每个线程都有一个)

	return 6666;												//线程结束
}

//创建线程test
void thrd_create_test(void){
	std::thread t1;											//线程实体是一个类(再次调用构造函数, 输入线程函数&参数, 则视为启动线程)
	std::thread::id t0_id,t1_id;				//线程id 是一个类, 可复用(新版c++ 线程库, 不能再简单打印一个uint 作为pid)
	std::chrono::high_resolution_clock::time_point start, end;		//chrono 高精度计时器
	std::chrono::duration<double, std::milli> elapsed;						//chrono 高精度的时差计算器
	std::time_t t_clock;																					//标准库时钟
	struct std::tm *ptm;																					//iomanip 时间容器


	t1 = std::thread(thrd_func, 600);		//创建线程
	if(!t1.joinable()){									//询问线程是否被激活
		perror("std::thread()");
		return;
	}

	t1.join();													//阻塞等待线程结束(不能获取线程结束状态, 只能清理线程尸体, 防止成为僵尸线程)

	t0_id = std::this_thread::get_id();	//获取main 线程自身的pid 描述类
	t1_id = t1.get_id();								//获取线程pid 描述类

	if(!t1.joinable())									//询问线程是否被激活
		printf("t1.joinable() = true, pthread is dead\n");


	t1 = std::thread(thrd_func, 600);
	if(!t1.joinable()){
		perror("std::thread()");
		return;
	}

	t1.detach();												//detach脱离线程, 不再询问线程结束状态


	std::this_thread::yield();					//立即挂起线程(this_thread = 本线程自身的类, 每个线程都有一个)

	start = std::chrono::high_resolution_clock::now();						//获取高精度计时器
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	//当前线程休眠1s
	end = std::chrono::high_resolution_clock::now();

	elapsed = end-start;																					//计算高精度时差
	printf("Waited %lf ms\n", elapsed.count());										//打印时差


	t_clock = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	ptm = std::localtime(&t_clock);

	//printf("Current time: %s\n", std::put_time(ptm, "%X"));			//必须大写X, 若小写x, 输出的为日期
	std::cout << "Current time: " << std::put_time(ptm, "%X") << std::endl;//(必须使用c++打印输出,c语言打印输出,需要做很多转换)
	ptm->tm_min = 0;
	ptm->tm_sec = 2;																							//从当前的时间开始, 向后休眠2 秒
	std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(mktime(ptm)));
	printf("%s reached!\n", std::put_time(ptm, "%X"));


	if(!t1.joinable())									//询问线程是否被激活
		printf("t1.joinable() = true, pthread is dead\n");

	return;
}



int main(void){
	thrd_create_test();
	return 0;
}

