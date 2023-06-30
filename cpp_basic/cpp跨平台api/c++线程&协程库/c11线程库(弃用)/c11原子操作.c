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



