//编译:
//		gcc -g3 -pthread CASEx_mutex.c -o x

//代码讲解4:
/*
	使用__sync_fetch_and_add()操作全局变量, 保证+1 操作的有序性, 有效性;
	使用G_mlock 对全部变量, 进行抢占式互斥操作, 其它没有抢到的线程, 死等(类似自旋锁);

	注意:
		用来做锁的变量, 必须是全局变量, 或者malloc/new 创建出来的, 可供多线程共同访问的变量!!
		线程锁本质上, 也需要CLOSE_VM 进程属性, 这点毫无疑问, 必要的前置条件;

	本互斥锁的优势:
		基于CAS 的实现, 互斥的同时不切换线程, 但消耗一定CPU 资源, 产生较多预测分支;
		(实测性能比mutex 互斥锁还要慢很多, 而且cpu 消耗极度严重, 
		 在资源抢夺极其严重的情况下, 这种乐观锁其实就是鸡肋, 反而消耗更多资源)
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

//16个线程, 一共操作G_count+1, 32000000 次
#define pth_count (16)

int G_count = 0;											//全局变量
int G_count_mlock = 0;								//全局变量mutex锁

//简化宏
#define CASEx_mutex_entry(G_lock_mutex) { while(!__sync_bool_compare_and_swap(&G_lock_mutex,0,1)){;} }
#define CASEx_mutex_leave(G_lock_mutex) { G_lock_mutex=0; }

void *test_func(void *arg){
	int i=0;
	for(i=0;i<2000000;++i){
		CASEx_mutex_entry(G_count_mlock);	//死等抢占CAS互斥锁, 进入临界区
		G_count+=1;
		CASEx_mutex_leave(G_count_mlock);	//离开临界区
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



void *test_func_old(void *arg){
	int i=0;
	for(i=0;i<2000000;++i){
		while(!__sync_bool_compare_and_swap(&G_count_mlock,0,1))
			;																	//死等抢占CAS互斥锁, 进入临界区
		//__sync_fetch_and_add(&G_count,1);	//理论上不再需要__sync 函数进行操作了
		G_count+=1;
		G_count_mlock = 0;									//离开临界区
	}
	return NULL;
}
