//编译:
//		gcc -g3 -pthread ./c11原子操作.c -o x
//		[win不支持]cl.exe ./c11原子操作.c -EHsc -w -Zi -Ox -link -out:x.exe



//判断编译器是否支持c11 原子操作库[win不支持]
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
	#include <stdatomic.h>
#endif

//判断编译器是否支持c11 线程库[win不支持]
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 threads.h ]
#else
	#pragma message("Support C11 threads.h")
	#include <threads.h>
#endif

#include <stdio.h>

//c11 线程库, 只能用在纯c 语言里面, 估计插入c++ 里面需要使用extern "C" {}, 不能单纯插入*.cpp 代码中, 否则会有很多报错;

//c11 原子操作
/*
若编译器定义宏常量__STDC_NO_ATOMICS__(C11), 则不提供头文件<stdatomic.h>、关键词_Atomic以及所有列于此的名称;
定义于头文件 <stdatomic.h>

类型
	memory_order											定义内存顺序制约(枚举)
	atomic_flag												免锁布尔标志(结构体)

宏
	ATOMIC_BOOL_LOCK_FREE							指示给定的原子类型为免锁(宏常量)
	ATOMIC_CHAR_LOCK_FREE
	ATOMIC_CHAR16_T_LOCK_FREE
	ATOMIC_CHAR32_T_LOCK_FREE
	ATOMIC_WCHAR_T_LOCK_FREE
	ATOMIC_SHORT_LOCK_FREE
	ATOMIC_INT_LOCK_FREE
	ATOMIC_LONG_LOCK_FREE
	ATOMIC_LLONG_LOCK_FREE
	ATOMIC_POINTER_LOCK_FREE
	ATOMIC_FLAG_INIT									初始化新的atomic_flag(宏常量)
	ATOMIC_VAR_INIT										初始化新的原子对象(宏函数)(C17 中弃用)
	kill_dependency										打破memory_order_consume 的依赖链(宏函数)

函数
	atomic_flag_test_and_set
	atomic_flag_test_and_set_explicit	设置atomic_flag 为true 并返回旧值(函数)
	atomic_flag_clear
	atomic_flag_clear_explicit				设置atomic_flag 为false(函数)
	atomic_init												初始化既存的原子对象(函数)
	atomic_is_lock_free								指示原子对象是否免锁(函数)
	atomic_store
	atomic_store_explicit							存储值到原子对象(函数)
	atomic_load
	atomic_load_explicit							从原子对象读取值(函数)
	atomic_exchange
	atomic_exchange_explicit					与原子对象的值交换值(函数)
	atomic_compare_exchange_strong
	atomic_compare_exchange_strong_explicit
	atomic_compare_exchange_weak
	atomic_compare_exchange_weak_explicit			若期待旧值则与原子对象交换值, 否则读取旧值(函数)
	atomic_fetch_add
	atomic_fetch_add_explicit					原子加法(函数)
	atomic_fetch_sub
	atomic_fetch_sub_explicit					原子减法(函数)
	atomic_fetch_or
	atomic_fetch_or_explicit					原子逐位或(函数)
	atomic_fetch_xor
	atomic_fetch_xor_explicit					原子逐位异或(函数)
	atomic_fetch_and
	atomic_fetch_and_explicit					原子逐位与(函数)
	atomic_thread_fence								通用的内存顺序依赖的栅栏同步原语(函数)
	atomic_signal_fence								线程与执行于同一线程的信号处理函数间的栅栏(函数)

类型
	typedef名													完整类型名(有两部分, 非常不好记)
	atomic_bool												_Atomic _Bool
	atomic_char												_Atomic char
	atomic_schar											_Atomic signed char
	atomic_uchar											_Atomic unsigned char
	atomic_short											_Atomic short
	atomic_ushort											_Atomic unsigned short
	atomic_int												_Atomic int
	atomic_uint												_Atomic unsigned int
	atomic_long												_Atomic long
	atomic_ulong											_Atomic unsigned long
	atomic_llong											_Atomic long long
	atomic_ullong											_Atomic unsigned long long
	atomic_char16_t										_Atomic char16_t
	atomic_char32_t										_Atomic char32_t
	atomic_wchar_t										_Atomic wchar_t
	atomic_int_least8_t								_Atomic int_least8_t
	atomic_uint_least8_t							_Atomic uint_least8_t
	atomic_int_least16_t							_Atomic int_least16_t
	atomic_uint_least16_t							_Atomic uint_least16_t
	atomic_int_least32_t							_Atomic int_least32_t
	atomic_uint_least32_t							_Atomic uint_least32_t
	atomic_int_least64_t							_Atomic int_least64_t
	atomic_uint_least64_t							_Atomic uint_least64_t
	atomic_int_fast8_t								_Atomic int_fast8_t
	atomic_uint_fast8_t								_Atomic uint_fast8_t
	atomic_int_fast16_t								_Atomic int_fast16_t
	atomic_uint_fast16_t							_Atomic uint_fast16_t
	atomic_int_fast32_t								_Atomic int_fast32_t
	atomic_uint_fast32_t							_Atomic uint_fast32_t
	atomic_int_fast64_t								_Atomic int_fast64_t
	atomic_uint_fast64_t							_Atomic uint_fast64_t
	atomic_intptr_t										_Atomic intptr_t
	atomic_uintptr_t									_Atomic uintptr_t
	atomic_size_t											_Atomic size_t
	atomic_ptrdiff_t									_Atomic ptrdiff_t
	atomic_intmax_t										_Atomic intmax_t
	atomic_uintmax_t									_Atomic uintmax_t

*/



//
// 1.call_once() 测试
//
atomic_int call_once_sum;

void call_once_func(void){
	int tmp = 9999;
	for(;tmp > 0;tmp--){
		atomic_fetch_add(&call_once_sum,1);	//标准的多线程竞争加法运算
		//call_once_sum+=1;									//不标准的加法
		printf("call_once_func: call_once_sum = %d\n", (int)call_once_sum);
		//printf("atomic_int call_once_sum is lock free? %s\n", atomic_is_lock_free(&call_once_sum) ? "true" : "false");
	}
	return;
}

//函数原型: void call_once( once_flag* flag, void (*func)(void) );
void call_once_test(void){
	call_once_sum = 0;										//初始化全局原子int 变量
	once_flag flag;												//一个函数, 一个once_flag; 调用一次后就会失效, 不能调用第二次;
	printf("call_once_func go\n");				//once_flag + call_once(), 可以保证多个线程对某个函数只调用一次;
	call_once(&flag, call_once_func);
	printf("call_once_func go again(not gonna run again)\n");
	call_once(&flag, call_once_func);
	return ;
}



//
// 2.多线程抢夺原子变量test
//
void many_thrd_killing_eachother(void){
	thrd_t t1, t2, t3, t4;
	thrd_create(&t1, call_once_func, NULL);
	thrd_create(&t2, call_once_func, NULL);
	thrd_create(&t3, call_once_func, NULL);
	thrd_create(&t4, call_once_func, NULL);

	thrd_join(t1, NULL);
	thrd_join(t2, NULL);
	thrd_join(t3, NULL);
	thrd_join(t4, NULL);
	return;
}



//
// 3.原子操作赋值exchange
//
void many_thrd_exchange(void* parg){
	int tmp = 9999;

	//赋值(在原子操作中, 赋值需要用atomic_exchange()来操作)
	atomic_exchange(&call_once_sum, (int)parg);

	for(;tmp > 0;tmp--){
		atomic_fetch_add(&call_once_sum,1);
		printf("many_thrd_exchange: call_once_sum = %d\n", (int)call_once_sum);
	}
	return;
}



int main(void){
	call_once_test();
	many_thrd_killing_eachother();
	many_thrd_exchange((void*)800);
	return 0;
}



