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
	call_once			准确地一次调用函数, 该调用是互斥的(函数c11)
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

