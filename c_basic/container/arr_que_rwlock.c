/*
	-nostartfiles = 没有开始函数的.c 源文件file, 
	-e 表示自定义开始函数(需要用exit(0) 结束自定义开始函数, 否则必然会Segmentation fault溢出)
	局部编译(方便局部调试):
		gcc -g3 -lpthread -nostartfiles arr_que_rwlock.h arr_que_rwlock.c -e test_aq -o x
*/



#include "arr_que_rwlock.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>



//从队列front压入一个元素(写锁互斥)
inline bool aq_push_front(aq_t* paq, aq_type val){
	if(paq->cur_size>=aq_size_max)
		return false;
	else{
		pthread_rwlock_wrlock(&paq->rwlock);
		paq->cur_size++;
		if(paq->pos_front > paq->pos_rear){				//相对位置, pos_front 在pos_rear 前面的情况
			if((aq_size_max - 1) == paq->pos_front){//如果pos 到达了aq_size_max-1数组极限, 即从头开始again
				paq->buf[paq->pos_front] = val;
				paq->pos_front = 0;
			}
			else
				paq->buf[paq->pos_front++] = val;
		}
		else																			//相对位置, pos_front 在pos_rear 后面的情况
			paq->buf[paq->pos_front++] = val;
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}
}

//从队列rear压入一个元素
inline bool aq_push_rear(aq_t* paq, aq_type val){
	if(paq->cur_size>=aq_size_max)
		return false;
	else{
		pthread_rwlock_wrlock(&paq->rwlock);
		paq->cur_size++;
		if(paq->pos_rear > paq->pos_front)
			paq->buf[paq->pos_rear--] = val;
		else{
			if(aq_size_min == paq->pos_rear){
				paq->buf[paq->pos_rear] = val;
				paq->pos_rear = aq_size_max - 1;
			}
			else
				paq->buf[paq->pos_rear--] = val;
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}
}

//从队列front弹出一个元素
inline bool aq_pop_front(aq_t* paq, aq_type *val){
	if(paq->cur_size<=aq_size_min)
		return false;
	else{
		pthread_rwlock_wrlock(&paq->rwlock);
		paq->cur_size--;
		if(paq->pos_front > paq->pos_rear)
			*val = paq->buf[--paq->pos_front];
		else{
			if(aq_size_min == paq->pos_front){
				paq->pos_front = aq_size_max - 1;
				*val = paq->buf[paq->pos_front];
			}
			else
				*val = paq->buf[--paq->pos_front];
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}
}

//从队列rear弹出一个元素
inline bool aq_pop_rear(aq_t* paq, aq_type *val){
	if(paq->cur_size<=aq_size_min)
		return false;
	else{
		pthread_rwlock_wrlock(&paq->rwlock);
		paq->cur_size--;
		if(paq->pos_rear > paq->pos_front){
			if((aq_size_max - 1) == paq->pos_rear){
				paq->pos_rear = aq_size_min;
				*val = paq->buf[paq->pos_rear];
			}
			else
				*val = paq->buf[++paq->pos_rear];
		}
		else
			*val = paq->buf[++paq->pos_rear];
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}
}



//从队列rear压入n 个元素(需要先将N 个元素放进paq->cache 缓冲区, 然后传入n 元素个数)
inline bool aq_pushN_rear(aq_t* paq, unsigned int N){
	unsigned int pos_cache, count, m;
	aq_type *ptmp;
	paq->errno=0;

	//N 不能大于aq_cache_max
	N = (N>aq_cache_max ? aq_cache_max : N);

	//完美压入N 个元素
	if(paq->cur_size+N <= aq_size_max){
		pthread_rwlock_wrlock(&paq->rwlock);
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
				paq->pos_rear = aq_size_max;
				for(m=0;m < count;m++)								//剩余的太麻烦, 用for 循环逐个插入
					paq->buf[--paq->pos_rear] = paq->cache[pos_cache++];
			}
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}

	//部分压入, 压入个数=N-(paq->cur_size+N-aq_size_max)=aq_size_max - paq->cur_size = m
	m = aq_size_max - paq->cur_size;
	if(m > 0){
		pthread_rwlock_wrlock(&paq->rwlock);
		paq->errno=m;															//部分压入, 压入后, paq->errno 显示压入个数
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
				paq->pos_rear = aq_size_max;
				for(m=0;m < count;m++)
					paq->buf[--paq->pos_rear] = paq->cache[pos_cache++];
			}
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return false;
	}
	else{//m <= 0 队列满
		paq->errno=-1;
		return false;
	}
}



//从队列front弹出n 个元素(传入n 元素个数, 将N 个元素弹入paq->cache 缓冲区, 然后自己取走)
inline bool aq_popN_front(aq_t* paq, unsigned int N){
	unsigned int pos_cache, count, m;
	aq_type *ptmp;
	paq->errno=0;

	//N 不能大于aq_cache_max
	N = (N>aq_cache_max ? aq_cache_max : N);

	//完美弹出N 个元素
	if(paq->cur_size >= N){
		pthread_rwlock_wrlock(&paq->rwlock);
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
				paq->pos_front = aq_size_max;
				for(m=0;m < count;m++)
					paq->cache[pos_cache++] = paq->buf[--paq->pos_front];
			}
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return true;
	}

	//部分弹出, 弹出个数=paq->cur_size
	if(paq->cur_size > 0){
		pthread_rwlock_wrlock(&paq->rwlock);
		m = paq->cur_size;
		paq->cur_size=0;
		paq->errno=paq->cur_size;								//部分弹出, 弹出后, paq->errno 显示弹出个数
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
				paq->pos_front = aq_size_max;
				for(m=0;m < count;m++)
					paq->cache[pos_cache++] = paq->buf[--paq->pos_front];
			}
		}
		pthread_rwlock_unlock(&paq->rwlock);
		return false;
	}
	else{																			//m <= 0 队列空
		paq->errno=-1;
		return false;
	}
}



void test_aq(void){
	unsigned int tmp, count, err_count=0;
	aq_t* paq=malloc(sizeof(aq_t));

	aq_init(paq);

	for(tmp=0; tmp<aq_size_max; tmp++)				//填满队列(front)
		if(!aq_push_front(paq, tmp))
			break;
	printf("填满队列front: cur_size=%d\n", paq->cur_size);

	for(tmp=aq_cur_size(paq); tmp>0; tmp--){	//弹空队列(front)
		if(!aq_pop_front(paq, &count))
			break;
		printf("%d, ", count);
	}
	printf("\n弹空队列front: cur_size=%d\n", paq->cur_size);


	for(tmp=0; tmp<aq_size_max; tmp++){				//填满队列(rear)
		if(!aq_push_rear(paq, tmp))
			break;
	}
	printf("填满队列rear: cur_size=%d\n", paq->cur_size);

	for(tmp=aq_cur_size(paq); tmp>0; tmp--){	//弹空队列(rear)
		if(!aq_pop_rear(paq, &count))
			break;
		printf("%d, ", count);
	}
	printf("\n弹空队列rear: cur_size=%d\n", paq->cur_size);



	for(tmp=0; tmp<aq_cache_max; tmp++)				//将数据放进paq->cache
		paq->cache[tmp]=tmp;

	count=aq_size_max/aq_cache_max;						//批量压入(填满)
	tmp=aq_cache_max;													//单次批量压入量(填满)
	do{
		if(aq_pushN_rear(paq, tmp))
			count--;
		else{
			printf("aq_pushN_rear() test failed\n");
			break;
		}
	}while(count > 0);

	for(tmp=0; tmp<aq_size_max; tmp++)				//打印批量压入的测试结果
		printf("%d, ", paq->buf[tmp]);
	printf("\n测试批量压入: cur_size=%d\n", paq->cur_size);



	count=aq_size_max/aq_cache_max;						//批量弹出
	tmp=aq_cache_max;
	do{
		//memset(paq->cache, '\0', aq_cache_max);
		if(aq_popN_front(paq, tmp)){
			for(tmp=0; tmp<aq_cache_max; tmp++)		//打印弹出的数据
				printf("%d, ", paq->cache[tmp]);
			printf("\n");													//打印换行
			if(paq->cache[0] != 0){								//抽查单个元素, 是否数据匹配, 准确弹出
				printf("*** paq->cache[0] != 0: tmp=%d, count=%d\n", tmp, count);
				err_count++;
			}
			count--;
		}
		else{
			printf("aq_popN_front() test failed\n");
			break;
		}
	}while(count > 0);
	printf("测试批量弹出: cur_size=%d\n", paq->cur_size);



	memset(paq->cache, '\0', aq_cache_max);		//批量'压入1, 弹出1', 单个测试
	paq->cache[0]=9999;
	if(!aq_pushN_rear(paq, 1))
		printf("批量'压入1' failed\n");

	paq->cache[0]=0;
	if(!aq_popN_front(paq, 1))
		printf("批量'弹出1' failed\n");

	if(paq->cache[0] != 9999)
		printf("批量'压入1, 弹出1', 单个测试 failed, paq->cache[0]=%d\n", paq->cache[0]);
	else
		printf("批量'压入1, 弹出1', 单个测试 okay, paq->cache[0]=%d\n", paq->cache[0]);


	printf("test_aq() finished, err_count=%d\n",err_count);

	aq_destroy(paq);
	free(paq);
	exit(0);
}
