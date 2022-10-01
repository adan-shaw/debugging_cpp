//编译:
//		gcc -g3 -pthread CAS.c -o x

//代码讲解1:使用__sync_fetch_and_add()操作全局变量, 保证+1 操作的有序性, 有效性(但无法抢占);

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

//16个线程, 一共操作G_count+1, 32000000 次
#define pth_count (16)

int G_count = 0;

//简单平稳地让多线程对G_count 进行+1 操作, 可以保证每个线程都能准确+1, 不会出现: 
/*
	A线程取了旧值+1, B线程也取了旧值+1, C线程也取了旧值+1;
	这时候, A,B,C 线程同时更新的时候, G_count 不一定是操作了3个+1, 可能只有2个+1操作, 可能只有1个+1操作;

	当这个重叠操作, 数量很大的时候, 差异更大;
	__sync_fetch_and_add() 只能简单保证各个线程的+1 操作正确有效;
	不能保证+1 操作更新过快的问题;
*/
void *test_func(void *arg){
	int i=0;
	for(i=0;i<2000000;++i)
		__sync_fetch_and_add(&G_count,1);
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



//+1 操作更新过快的问题保留(证明被篡改的速度很快, 没办法控制)
void *test_func2(void *arg){
	int i=0,tmp,now;
	for(i=0;i<2000000;++i){
		tmp = __sync_fetch_and_add(&G_count,1);
		now = G_count;
		if(tmp != (--now)){
			printf("if(tmp != (--now)){}, i=%d\n",i);
			continue;
		}
	}
	return NULL;
}
