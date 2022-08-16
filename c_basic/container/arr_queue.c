/*
	-nostartfiles = 没有开始函数的.c 源文件file, 
	-e 表示自定义开始函数(需要用exit(0) 结束自定义开始函数, 否则必然会Segmentation fault溢出)
	局部编译(方便局部调试):
		gcc -g3 -lpthread -nostartfiles arr_queue.h arr_queue.c -e test_aq -o x
*/



#include "arr_queue.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>



//初始化队列
inline void aq_init(aq_t* paq){
	paq->errno = 0;
	paq->cur_size = 0;
	paq->pos_front = 0;
	paq->pos_rear = 0;
}

//销毁队列
//inline void aq_destroy(aq_t* paq){}

//从队列front压入一个元素(写锁互斥)
inline bool aq_push_front(aq_t* paq, aq_type val){
	if(paq->cur_size>=aq_buf_max)
		return false;
	else{
		paq->cur_size++;
		if(paq->pos_front > paq->pos_rear){//相对位置, pos_front 在pos_rear 前面的情况
			if((aq_buf_max - 1) == paq->pos_front){//如果pos 到达了aq_buf_max-1数组极限, 即从头开始again
				paq->buf[paq->pos_front] = val;
				paq->pos_front = 0;
			}
			else
				paq->buf[paq->pos_front++] = val;
		}
		else//相对位置, pos_front 在pos_rear 后面的情况
			paq->buf[paq->pos_front++] = val;
		return true;
	}
}

//从队列rear压入一个元素
inline bool aq_push_rear(aq_t* paq, aq_type val){
	if(paq->cur_size>=aq_buf_max)
		return false;
	else{
		paq->cur_size++;
		if(paq->pos_rear > paq->pos_front)
			paq->buf[paq->pos_rear--] = val;
		else{
			if(aq_buf_min == paq->pos_rear){
				paq->buf[paq->pos_rear] = val;
				paq->pos_rear = aq_buf_max - 1;
			}
			else
				paq->buf[paq->pos_rear--] = val;
		}
		return true;
	}
}

//从队列front弹出一个元素
inline bool aq_pop_front(aq_t* paq, aq_type *val){
	if(paq->cur_size<=aq_buf_min)
		return false;
	else{
		paq->cur_size--;
		if(paq->pos_front > paq->pos_rear)
			*val = paq->buf[--paq->pos_front];
		else{
			if(aq_buf_min == paq->pos_front){
				paq->pos_front = aq_buf_max - 1;
				*val = paq->buf[paq->pos_front];
			}
			else
				*val = paq->buf[--paq->pos_front];
		}
		return true;
	}
}

//从队列rear弹出一个元素
inline bool aq_pop_rear(aq_t* paq, aq_type *val){
	if(paq->cur_size<=aq_buf_min)
		return false;
	else{
		paq->cur_size--;
		if(paq->pos_rear > paq->pos_front){
			if((aq_buf_max - 1) == paq->pos_rear){
				paq->pos_rear = aq_buf_min;
				*val = paq->buf[paq->pos_rear];
			}
			else
				*val = paq->buf[++paq->pos_rear];
		}
		else
			*val = paq->buf[++paq->pos_rear];
		return true;
	}
}



//从队列rear压入n 个元素(需要先将N 个元素放进paq->cache 缓冲区, 然后传入n 元素个数)
bool aq_pushN_rear(aq_t* paq, unsigned int N){
	unsigned int tmp, pos_cache, count, m;
	aq_type *ptmp;
	paq->errno=0;

	//N 不能大于aq_cache_max
	N = (N>aq_cache_max ? aq_cache_max : N);

	//完美压入N 个元素
	if(paq->cur_size+N <= aq_buf_max){
		paq->cur_size+=N;
		if(paq->pos_rear > paq->pos_front){
			paq->pos_rear-=N;
			ptmp = &paq->buf[paq->pos_rear];
			memcpy(ptmp, paq->cache, N*sizeof(aq_type));
		}
		else{
			if(paq->pos_rear > N){
				paq->pos_rear-=N;
				ptmp = &paq->buf[paq->pos_rear];
				memcpy(ptmp, paq->cache, N*sizeof(aq_type));
			}
			else{
				memcpy(paq->buf, paq->cache, paq->pos_rear*sizeof(aq_type));
				pos_cache = paq->pos_rear;
				count = N - paq->pos_rear;
				paq->pos_rear = aq_buf_max;
				for(tmp=0;tmp < count;tmp++)//剩余的太麻烦, 用for 循环逐个插入
					paq->buf[--paq->pos_rear] = paq->cache[pos_cache++];
			}
		}
		return true;
	}

	//部分压入, 压入个数=N-(paq->cur_size+N-aq_buf_max)=aq_buf_max - paq->cur_size = m
	m = aq_buf_max - paq->cur_size;
	if(m > 0){
		paq->errno=m;//部分压入, 压入后, paq->errno 显示压入个数
		paq->cur_size+=m;
		if(paq->pos_rear > paq->pos_front){
			paq->pos_rear-=m;
			ptmp = &paq->buf[paq->pos_rear];
			memcpy(ptmp, paq->cache, m*sizeof(aq_type));
		}
		else{
			if(paq->pos_rear > m){
				paq->pos_rear-=m;
				ptmp = &paq->buf[paq->pos_rear];
				memcpy(ptmp, paq->cache, m*sizeof(aq_type));
			}
			else{
				memcpy(paq->buf, paq->cache, paq->pos_rear*sizeof(aq_type));
				pos_cache = paq->pos_rear;
				count = m - paq->pos_rear;
				paq->pos_rear = aq_buf_max;
				for(tmp=0;tmp < count;tmp++)//剩余的太麻烦, 用for 循环逐个插入
					paq->buf[--paq->pos_rear] = paq->cache[pos_cache++];
			}
		}
		return false;
	}
	else{//m <= 0 队列满
		paq->errno=-1;
		return false;
	}
}



//从队列front弹出n 个元素(传入n 元素个数, 将N 个元素弹入paq->cache 缓冲区, 然后自己取走)
bool aq_popN_front(aq_t* paq, unsigned int N){
	unsigned int tmp, pos_cache, count, m;
	aq_type *ptmp;
	paq->errno=0;

	//N 不能大于aq_cache_max
	N = (N>aq_cache_max ? aq_cache_max : N);

	//完美弹出N 个元素
	if(paq->cur_size >= N){
		paq->cur_size-=N;
		if(paq->pos_front > paq->pos_rear){
			paq->pos_front-=N;
			ptmp = &paq->buf[paq->pos_front];
			memcpy(paq->cache, ptmp, N*sizeof(aq_type));
		}
		else{
			if(paq->pos_front > N){
				paq->pos_front-=N;
				ptmp = &paq->buf[paq->pos_front];
				memcpy(paq->cache, ptmp, N*sizeof(aq_type));
			}
			else{
				memcpy(paq->buf, paq->cache, paq->pos_front*sizeof(aq_type));
				pos_cache = paq->pos_front;
				count = N - paq->pos_front;
				paq->pos_front = aq_buf_max;
				for(tmp=0;tmp < count;tmp++)//剩余的太麻烦, 用for 循环逐个插入
					paq->cache[pos_cache++] = paq->buf[--paq->pos_front];
			}
		}
		return true;
	}

	//部分弹出, 弹出个数=paq->cur_size
	if(paq->cur_size > 0){
		m = paq->cur_size;
		paq->cur_size=0;
		paq->errno=paq->cur_size;//部分弹出, 弹出后, paq->errno 显示弹出个数
		if(paq->pos_front > paq->pos_rear){
			paq->pos_front-=m;
			ptmp = &paq->buf[paq->pos_front];
			memcpy(paq->cache, ptmp, m*sizeof(aq_type));
		}
		else{
			if(paq->pos_front > m){
				paq->pos_front-=m;
				ptmp = &paq->buf[paq->pos_front];
				memcpy(paq->cache, ptmp, m*sizeof(aq_type));
			}
			else{
				memcpy(paq->buf, paq->cache, paq->pos_front*sizeof(aq_type));
				pos_cache = paq->pos_front;
				count = m - paq->pos_front;
				paq->pos_front = aq_buf_max;
				for(tmp=0;tmp < count;tmp++)//剩余的太麻烦, 用for 循环逐个插入
					paq->cache[pos_cache++] = paq->buf[--paq->pos_front];
			}
		}
		return false;
	}
	else{//m <= 0 队列空
		paq->errno=-1;
		return false;
	}
}



void test_aq(void){
	unsigned int tmp, count, err_count=0;
	aq_t aq;



	aq_init(&aq);

	//填满队列
	for(tmp=0; tmp<aq_buf_max; tmp++){
		if(!aq_push_front(&aq, tmp))
			break;
	}
	printf("填满队列front: cur_size=%d\n", aq.cur_size);

	//弹空队列
	for(tmp=aq_buf_max; tmp>0; tmp--){
		if(!aq_pop_front(&aq, &count))
			break;
		printf("%d, ", count);
	}
	printf("\n弹空队列front: cur_size=%d\n", aq.cur_size);

	for(tmp=0; tmp<aq_buf_max; tmp++){
		if(!aq_push_rear(&aq, tmp))
			break;
	}
	printf("填满队列rear: cur_size=%d\n", aq.cur_size);

	for(tmp=aq_buf_max; tmp>0; tmp--){
		if(!aq_pop_rear(&aq, &count))
			break;
		printf("%d, ", count);
	}
	printf("\n弹空队列rear: cur_size=%d\n", aq.cur_size);



	//测试批量压入
	for(tmp=0; tmp<aq_cache_max; tmp++)
		aq.cache[tmp]=tmp;
	count = aq_buf_max/aq_cache_max;
	tmp = aq_cache_max;
	do{
		if(aq_pushN_rear(&aq, tmp)){
			//printf("测试批量压入: cur_size=%d\n", aq.cur_size);
			count--;
		}
		else{
			printf("aq_pushN_rear() test failed\n");
			break;
		}
	}while(count > 0);
	for(tmp=0; tmp<aq_buf_max; tmp++)
		printf("%d, ", aq.buf[tmp]);
	printf("\n测试批量压入: cur_size=%d\n", aq.cur_size);



	//测试批量弹出
	count = aq_buf_max/aq_cache_max;
	tmp = aq_cache_max;
	do{
		memset(aq.cache, '\0', aq_cache_max);
		if(aq_popN_front(&aq, tmp)){
			//printf("测试批量弹出: cur_size=%d\n", aq.cur_size);
			for(tmp=0; tmp<aq_cache_max; tmp++)
				printf("%d, ", aq.cache[tmp]);
			printf("\n");
			if(aq.cache[0] != 0){
				printf("*** aq.cache[0] != 0: tmp=%d, count=%d\n", tmp, count);
				err_count++;
			}
			count--;
		}
		else{
			printf("aq_popN_front() test failed\n");
			break;
		}
	}while(count > 0);
	printf("测试批量弹出: cur_size=%d\n", aq.cur_size);



	//批量'压入1, 弹出1'测试
	memset(aq.cache, '\0', aq_cache_max);
	aq.cache[0] = 9999;
	if(!aq_pushN_rear(&aq, 1))
		printf("批量'压入1' failed\n");

	aq.cache[0] = 0;
	if(!aq_popN_front(&aq, 1))
		printf("批量'弹出1' failed\n");

	if(aq.cache[0] != 9999)
		printf("批量'压入1, 弹出1'测试 failed, aq.cache[0]=%d\n", aq.cache[0]);
	else
		printf("批量'压入1, 弹出1'测试 okay, aq.cache[0]=%d\n", aq.cache[0]);


	printf("test_aq() finished, err_count=%d\n",err_count);

	//aq_destroy(&aq);
	exit(0);
}
