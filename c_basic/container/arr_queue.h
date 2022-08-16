#include <stdbool.h>
#include <pthread.h>



#define aq_type unsigned int
#define aq_buf_max (96)
#define aq_buf_min (0)

//注意: 批量插入操作有[ N = (N>aq_cache_max ? aq_cache_max : N); ]限制, 
//			为了避免数据出错, 请直接设置as_buf_max = aq_cache_max
//			即倒空as 栈buf, 刚好可以全部插入aq 队列, 这样就不会出错了;
#define aq_cache_max (16)



typedef struct aq_info{
	unsigned int cur_size;
	unsigned int errno;
	unsigned int pos_front;
	unsigned int pos_rear;
	aq_type buf[aq_buf_max];
	aq_type cache[aq_cache_max];
}aq_t;


//初始化队列
void aq_init(aq_t* paq);

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



//从队列rear压入n 个元素(需要先将N 个元素放进paq->cache 缓冲区, 然后传入n 元素个数)
bool aq_pushN_rear(aq_t* paq, unsigned int N);


//从队列front弹出n 个元素(传入n 元素个数, 将N 个元素弹入paq->cache 缓冲区, 然后自己取走)
bool aq_popN_front(aq_t* paq, unsigned int N);
