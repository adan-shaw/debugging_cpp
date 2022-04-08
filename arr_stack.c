/*
	-nostartfiles = 没有开始函数的.c 源文件file, 
	-e 表示自定义开始函数(需要用exit(0) 结束自定义开始函数, 否则必然会Segmentation fault溢出)
	局部编译(方便局部调试):
		gcc -g3 -nostartfiles arr_stack.h arr_stack.c -e test_as -o x
*/



#include "arr_stack.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>



//初始化栈
inline void as_init(as_t* pas){
	pas->errno = 0;
	pas->cur_size = 0;
}

//压一个元素入栈
inline void as_push(as_t* pas, as_type val){
	pas->errno=0;
	if(pas->cur_size>=as_buf_max)
		pas->errno=-1;
	else
		pas->buf[pas->cur_size++] = val;
}

//弹一个元素出栈
inline as_type as_pop(as_t* pas){
	pas->errno=0;
	if(pas->cur_size<=as_buf_min)
		pas->errno=-1;
	else
		return pas->buf[--pas->cur_size];
}



//压栈n 个(需要先将N 个元素放进pas->cache 缓冲区, 然后传入n 元素个数)
bool as_pushN(as_t* pas, unsigned int N){
	unsigned int m = 0;
	as_type *ptmp;
	pas->errno=0;

	//N 不能大于as_cache_max
	N = (N>as_cache_max ? as_cache_max : N);

	//完美压入N 个元素
	if(pas->cur_size+N <= as_buf_max){
		ptmp = pas->cur_size!=0 ? &pas->buf[pas->cur_size - 1] : &pas->buf[pas->cur_size];
		//if(pas->cur_size != 0)
			//ptmp = &pas->buf[pas->cur_size - 1];//相位
		//else
			//ptmp = &pas->buf[pas->cur_size]
		memcpy(ptmp, pas->cache, N*sizeof(as_type));
		pas->cur_size+=N;
		return true;
	}

	//部分压入, 压入个数=N-(pas->cur_size+N-as_buf_max)=as_buf_max - pas->cur_size = m
	m = as_buf_max - pas->cur_size;
	if(m > 0){
		ptmp = pas->cur_size!=0 ? &pas->buf[pas->cur_size - 1] : &pas->buf[pas->cur_size];
		memcpy(ptmp, pas->cache, m*sizeof(as_type));
		pas->errno=m;//部分压入, 压入后, pas->errno 显示压入个数
		pas->cur_size+=m;
		return false;
	}
	else{//m <= 0 栈满
		pas->errno=-1;
		return false;
	}
}



//弹栈n 个(传入n 元素个数, 将N 个元素弹入pas->cache 缓冲区, 然后自己取走)
bool as_popN(as_t* pas, unsigned int N){
	unsigned int m = 0;
	as_type *ptmp;
	pas->errno=0;

	//N 不能大于as_cache_max
	N = (N>as_cache_max ? as_cache_max : N);

	//完美弹出N 个元素
	if(pas->cur_size >= N){
		m = pas->cur_size - N;
		m = (pas->cur_size - 1) - m;//相位
		ptmp = &pas->buf[m];
		memcpy(pas->cache, ptmp, N*sizeof(as_type));
		pas->cur_size-=N;
		return true;
	}

	//部分弹出, 弹出个数=pas->cur_size
	if(pas->cur_size > 0){
		memcpy(pas->cache, pas->buf, pas->cur_size*sizeof(as_type));
		pas->errno=pas->cur_size;//部分弹出, 弹出后, pas->errno 显示弹出个数
		pas->cur_size=0;
		return false;
	}
	else{//m <= 0 栈空
		pas->errno=-1;
		return false;
	}
}



void test_as(void){
	unsigned int tmp, count, err_count=0;
	as_t as;



	as_init(&as);

	//填满栈
	for(tmp=0; tmp<as_buf_max; tmp++){
		as_push(&as, tmp);
		if(as.errno != -1)
			break;
	}
	printf("填满栈: cur_size=%d\n", as.cur_size);

	//弹空栈
	for(tmp=as_buf_max; tmp>0; tmp--){
		printf("%d, ", as_pop(&as));
		if(as.errno != -1)
			break;
	}
	printf("\n弹空栈: cur_size=%d\n", as.cur_size);



	//测试批量压入
	for(tmp=0; tmp<as_cache_max; tmp++)
		as.cache[tmp]=tmp;
	count = as_buf_max/as_cache_max;
	tmp = as_cache_max;
	do{
		if(as_pushN(&as, tmp)){
			printf("测试批量压入: cur_size=%d\n", as.cur_size);
			count--;
		}
		else{
			printf("as_pushN() test failed\n");
			break;
		}
	}while(count > 0);
	for(tmp=0; tmp<as_buf_max; tmp++)
		printf("%d, ", as.buf[tmp]);
	printf("\n测试批量压入: cur_size=%d\n", as.cur_size);



	//测试批量弹出
	count = as_buf_max/as_cache_max;
	tmp = as_cache_max;
	do{
		memset(as.cache, '\0', as_cache_max);
		if(as_popN(&as, tmp)){
			printf("测试批量弹出: cur_size=%d\n", as.cur_size);
			for(tmp=0; tmp<as_cache_max; tmp++)
				printf("%d, ", as.cache[tmp]);
			printf("\n");
			if(as.cache[0] != 0){
				printf("*** as.cache[0] != 0: tmp=%d, count=%d\n", tmp, count);
				err_count++;
			}
			count--;
		}
		else{
			printf("as_popN() test failed\n");
			break;
		}
	}while(count > 0);
	printf("测试批量弹出: cur_size=%d\n", as.cur_size);



	//批量'压入1, 弹出1'测试
	memset(as.cache, '\0', as_cache_max);
	as.cache[0] = 9999;
	if(!as_pushN(&as, 1))
		printf("批量'压入1' failed\n");

	as.cache[0] = 0;
	if(!as_popN(&as, 1))
		printf("批量'弹出1' failed\n");

	if(as.cache[0] != 9999)
		printf("批量'压入1, 弹出1'测试 failed, as.cache[0]=%d\n", as.cache[0]);
	else
		printf("批量'压入1, 弹出1'测试 okay, as.cache[0]=%d\n", as.cache[0]);


	printf("test_as() finished, err_count=%d\n",err_count);
	exit(0);
}
