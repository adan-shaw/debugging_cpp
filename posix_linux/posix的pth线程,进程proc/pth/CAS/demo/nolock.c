//编译:
//		gcc -g3 -pthread nolock.c -o x

//代码讲解3: 完全不加锁
/*
	完全不加锁, 看看最终G_count 是否等于32000000, 
	不等于32000000, 即表示G_count+1 不安全, 有重叠!!

	结果很明显: 
		运算结果, 每一次都不相同!! 
		证明多线程抢夺G_count+1 不安全, 必须使用__sync_fetch_and_add(), 保证+1 操作的有序性;
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

//16个线程, 一共操作G_count+1, 32000000 次
#define pth_count (16)

int G_count = 0;

void *test_func(void *arg){
	int i=0;
	for(i=0;i<2000000;++i)
		++G_count;
	return NULL;
}

int main(void){
	pthread_t id[pth_count];
	int i = 0;

	uint64_t usetime;
	struct timeval start;
	struct timeval end;

	gettimeofday(&start,NULL);

	for(i=0;i<pth_count;++i)
		pthread_create(&id[i],NULL,test_func,NULL);

	for(i=0;i<pth_count;++i)
		pthread_join(id[i],NULL);

	gettimeofday(&end,NULL);

	usetime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	printf("G_count = %d, usetime = %lu usecs\n", G_count, usetime);
	return 0;
}
