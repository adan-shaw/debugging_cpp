//编译:
//		gcc -g3 -pthread mutex.c -o x

//代码讲解2:使用互斥锁mutex操作全局变量
/*
	互斥锁的详细使用就不分析了, 最简单的, 肯定可靠, 无敌的;
	只是性能不怎么样, 必然触发线程切换;
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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *test_func(void *arg){
	int i=0;
	for(i=0;i<2000000;++i){
		pthread_mutex_lock(&mutex);
		++G_count;
		pthread_mutex_unlock(&mutex);
	}
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
