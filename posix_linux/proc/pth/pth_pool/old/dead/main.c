//编译:
//    gcc threadpool.h threadpool.c main.c -lpthread -g3 -o x

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "threadpool.h"

#define pth_pool_count (8)
#define test_count (10000)


threadpool_t pool;


//任务函数
void* mytask(void *arg){
	printf("thread %d is working on task %d\n", pthread_self(), *(int*)arg);
	//sleep(1);
	free(arg);
	return NULL;
}



int main(void){
	int *arg, i;
	
	if(!threadpool_init(&pool, pth_pool_count))
		printf("threadpool_init() failed\n");

	for(i = 0; i < test_count; i++){
		arg = (int*)malloc(sizeof(int));
		*arg = i;
		threadpool_add_task(&pool, mytask, arg);
	}

	if(!threadpool_destroy(&pool))
		printf("threadpool_destroy() failed\n");
	return 0;
}




