//编译:
//		gcc posix_barrier.c -g3 -lpthread -o x 

//barrier栅栏的简介: 
/*
	好处:
		简单的线程同步方式, 连mutex互斥锁都省了, 十分好用;
		可重复利用, 也适合一次性使用并销毁;
	缺点:
		只能做简单等待(等人齐了自动唤醒),
		不能主动(手动)唤醒;
*/



//barrier栅栏操作API
/*
	int pthread_barrier_destroy(pthread_barrier_t *lock);
	int pthread_barrier_init(pthread_barrier_t *lockrestrict, const pthread_barrierattr_t *restrict, unsigned count);
	int pthread_barrier_wait(pthread_barrier_t *lock);
*/

//barrier栅栏属性修改API(略)
/*
	int pthread_barrierattr_destroy(pthread_barrierattr_t *);
	int pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict, int *restrict);
	int pthread_barrierattr_init(pthread_barrierattr_t *);
	int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int);
*/



#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t G_barrier;

void* pth_func(void* args){
	printf("pth_func() start\n");
	sleep(1);
	//子线程到达barrier栅栏
	pthread_barrier_wait(&G_barrier);
	printf("pth_func() exit\n");
	return NULL;
}

int main(void){
	pthread_t thid;

	//初始化barrier栅栏(主线程+子线程=2)
	if(pthread_barrier_init(&G_barrier, NULL, 2) != 0){
		perror("pthread_barrier_init()");
		return -1;
	}

	if(pthread_create(&thid,NULL,pth_func,NULL) != 0){
		perror("pthread_create()");
		return -1;
	}

	//主线程到达barrier栅栏
	pthread_barrier_wait(&G_barrier);

	//销毁barrier栅栏(谁初始化, 谁负责销毁)
	pthread_barrier_destroy(&G_barrier);

	if(pthread_join(thid,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}

	printf("main() exit\n");
	return 0;
}
