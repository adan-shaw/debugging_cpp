#include <stdbool.h>

//单线程版, 不能用在多线程环境下;

#ifndef __arr_que
	#define __arr_que (1)

	#define aq_type unsigned int
	//#define aq_type int
	//#define aq_type double
	#define aq_buf_max (4096)
	#define aq_size_max aq_buf_max
	#define aq_size_min (0)

	//批量插入cache 缓冲区, 注意:
	/*
		批量插入操作有[ N=(N>aq_cache_max ? aq_cache_max : N); ]限制, 
		为了避免数据出错, 请直接设置as_buf_max(栈) = aq_cache_max(队列);
		即倒空as 栈buf, 刚好可以全部插入aq 队列, 这样就不会出错了;
	*/
	#define aq_cache_max (128)



	typedef struct aq_info{
		unsigned int cur_size;
		unsigned int errno;
		unsigned int pos_front;
		unsigned int pos_rear;
		aq_type buf[aq_buf_max];
		aq_type cache[aq_cache_max];
	}aq_t;


	//初始化队列
	#define aq_init(paq) { \
		paq->errno = 0; \
		paq->cur_size = 0; \
		paq->pos_front = 0; \
		paq->pos_rear = 0; }

	//销毁队列(带mutex锁才有, 无锁版arr_queue无destroy函数)
	//void aq_destroy(aq_t* paq);

	//从队列front压入一个元素(写锁互斥)
	bool aq_push_front(aq_t* paq, aq_type val);

	//从队列rear压入一个元素
	bool aq_push_rear(aq_t* paq, aq_type val);

	//从队列front弹出一个元素
	bool aq_pop_front(aq_t* paq, aq_type *val);

	//从队列rear弹出一个元素
	bool aq_pop_rear(aq_t* paq, aq_type *val);

	//返回队列当前的元素个数(读不加锁, 写才加锁)
	#define aq_cur_size(paq) (paq->cur_size)

	//从队列rear压入n个元素(需要先将N个元素拷贝进paq->cache缓冲区, 然后传入N)
	bool aq_pushN_rear(aq_t* paq, unsigned int N);

	//从队列front弹出n个元素(传入N, 则会自动弹出N个元素到paq->cache 缓冲区, 请自己取走)
	bool aq_popN_front(aq_t* paq, unsigned int N);

#endif
