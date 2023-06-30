//编译:
//		gcc -g3 -pthread ./c11线程库.c -o x



//判断编译器是否支持c11 原子操作库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 threads.h ]
#else
	#pragma message("Support C11 threads.h")
	#include <threads.h>
#endif

#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
	#include <stdatomic.h>
#endif

#include <stdio.h>

//c11 线程库, 只能用在纯c 语言里面, 估计插入c++ 里面需要使用extern "C" {}, 不能单纯插入*.cpp 代码中, 否则会有很多报错;

//c11 线程库
/*
若编译器定义宏常量__STDC_NO_THREADS__(C11), 则不提供头文件<threads.h>和所有列于此的名称;
定义于头文件 <threads.h>

创建线程
	thrd_t				实现定义的标识线程的完整对象类型
	thrd_create		创建线程(函数c11)
	thrd_equal		检查二个标识符是否表示同一线程(函数c11)
	thrd_current	获取当前线程标识符(函数c11)
	thrd_sleep		在给定的时间段内暂停调用方线程的执行(函数c11)
	thrd_yield		让出当前时间片段(函数c11)
	thrd_exit			终止调用方线程(函数c11)
	thrd_detach		分离线程(函数c11)
	thrd_join			阻塞到线程终止为止(函数c11)
	thrd_start_t	函数指针类型int(*)(void*)的typedef, 为thrd_create所用(typedef[c11])
	thrd_successthrd_timedoutthrd_busythrd_nomemthrd_error	指示线程错误状态(常量c11)

互斥锁
	call_once			准确地一次调用函数, 该调用是互斥的(函数c11) [纯c 语言版本, 不支持传递函数]
	mtx_t					互斥标识符
	mtx_init			创建互斥(函数c11)
	mtx_lock			阻塞到锁定互斥为止(函数c11)
	mtx_timedlock	阻塞到锁定互斥或时限经过为止(函数c11)
	mtx_trylock		锁定互斥, 若已锁定则返回而不阻塞(函数c11)
	mtx_unlock		解锁互斥(函数c11)
	mtx_destroy		销毁互斥(函数c11)
	mtx_plainmtx_recursivemtx_timed	定义互斥的类型(枚举c11)

条件变量
	cnd_t					条件变量标识符
	cnd_init			创建条件变量(函数c11)
	cnd_signal		除阻一个阻塞于条件变量上的线程(函数c11)
	cnd_broadcast	除阻所有阻塞于条件变量上的线程(函数c11)
	cnd_wait			在条件变量上阻塞(函数c11)
	cnd_timedwait	在条件变量上阻塞一段时长(函数c11)
	cnd_destroy		销毁条件变量(函数c11)

线程局域存储
	thread_local	线程局域存储类宏(关键字宏c11)
	tss_t					线程特定存储的指针
	tss_dtor_t		函数指针类型 void(*)(void*), 用作 TSS 析构器(typedef[c11])
	tss_create		以给定的析构器, 创建线程特定存储指针(函数c11)
	tss_get				从线程特定存储读取(函数c11)
	tss_set				写入线程特定存储(函数c11)
	tss_delete		释放给定的线程特定存储指针所保有的资源(函数c11)
	TSS_DTOR_ITERATIONS	析构器被调用的最大次数(宏常量c11)

*/



//打印当前线程的pid(哪个线程调用本函数, 就打印哪个线程的pid)
void print_pid(void){
	//thrd_t 变量实际上是: unsigned long int
	thrd_t thrd = thrd_current();
	printf("print_pid(): %d\n", (unsigned long int)thrd);

	//对比两个线程pid 是否相等
	if(thrd_equal(thrd, thrd_current()))
		printf("thrd_equal() checks out ok\n");

	return;
}



atomic_int sum;

int thrd_func(void* parg){
	struct timespec tt;
	int tmp = 9999;
	atomic_exchange(&sum, (int)parg);	//赋值(在原子操作中, 赋值需要用atomic_exchange()来操作)
	for(;tmp > 0;tmp--){
		atomic_fetch_add(&sum,1);
		printf("thrd_func: sum = %d\n", (int)sum);
	}

	thrd_yield();											//立即挂起线程(本函数无参数, 无返回值)

	tt.tv_sec = 1;										//秒
	tt.tv_nsec = 0;										//纳秒[1s=1000ms(毫秒)=1000*1000us(微妙)=1000*1000*1000ns(纳秒)]
	if(thrd_sleep(&tt,NULL) != 0)			//线程休眠
		perror("thrd_sleep()");

	thrd_exit(6666);									//等价于return 6666;
	return 6666;
}

//创建线程test
void thrd_create_test(void){
	thrd_t t1;
	int thrd_stat;

	if(thrd_create(&t1, thrd_func, (void*)600) != thrd_success){
		perror("thrd_create()");				//创建线程
		return;
	}

	if(thrd_join(t1, &thrd_stat) != thrd_success){
		perror("thrd_join()");					//join询问线程结束状态(阻塞等待线程结束) [一定会阻塞, 不能调节成非阻塞的]
		return;
	}
	printf("thrd_join(): thrd_stat=%d\n", thrd_stat);

	if(thrd_create(&t1, thrd_func, (void*)600) != thrd_success){
		perror("thrd_create()");
		return;
	}

	if(thrd_detach(t1) != thrd_success){
		perror("thrd_detach()");				//detach脱离线程, 不再询问线程结束状态
		return;
	}

	return;
}



int main(void){
	print_pid();
	thrd_create_test();
	return 0;
}

