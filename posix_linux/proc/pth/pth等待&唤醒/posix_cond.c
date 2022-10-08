//编译:
//		gcc posix_cond.c -g3 -lpthread -o x



//cond 条件变量操作API list
/*
	int pthread_cond_destroy(pthread_cond_t *);
	int pthread_cond_init(pthread_cond_t *restrict,const pthread_condattr_t *restrict);
	int pthread_cond_signal(pthread_cond_t *);
	int pthread_cond_broadcast(pthread_cond_t *);
	int pthread_cond_timedwait(pthread_cond_t *restrict, pthread_mutex_t *restrict, const struct timespec *restrict);
	int pthread_cond_wait(pthread_cond_t *restrict,pthread_mutex_t *restrict);

返回值:
	pthread_cond_init, pthread_cond_signal, 
	pthread_cond_broadcast, pthread_cond_wait never return an error code(用不返回);

	pthread_cond_timedwait返回值:
		ETIMEDOUT 超时
		EINTR     被信号唤醒而返回

	pthread_cond_destroy返回值:
		EBUSY     仍然有线程waiting on it
*/

//cond 条件变量属性修改API
/*
	int pthread_condattr_destroy(pthread_condattr_t *);
	int pthread_condattr_getclock(const pthread_condattr_t *restrict,clockid_t *restrict);
	int pthread_condattr_getpshared(const pthread_condattr_t *restrict,int *restrict);
	int pthread_condattr_init(pthread_condattr_t *);
	int pthread_condattr_setclock(pthread_condattr_t *, clockid_t);
	int pthread_condattr_setpshared(pthread_condattr_t *, int);
*/



#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>



int G_exit = 0;
pthread_cond_t G_cond;
pthread_mutex_t G_mutex;



void *thread1(void *arg){
	int count=0;
	while(!G_exit){
		pthread_mutex_lock(&G_mutex);
		pthread_cond_wait(&G_cond,&G_mutex);										//死等
		pthread_mutex_unlock(&G_mutex);
		printf("pthread_cond_wait() wake up count=%d\n",++count);
	}
	printf("thread1 exit\n");
	return NULL;
}

void *thread2(void *arg){
	int tmp,count=0;
	struct timespec timeout;
	timeout.tv_sec = 2;	//0秒timeout[不知道为什么超时功能不能用,1秒超时不会阻塞1秒,而是直接返回了
	timeout.tv_nsec = 0;//					 linux禁用了超时的cond条件变量? 证明: pthread_cond_timedwait()不会切换线程,
											//					 多运行几次, 可以看到pthread_cond_timedwait() 会被调用]
	//return NULL;//for test only
	while(!G_exit){
		pthread_mutex_lock(&G_mutex);
		tmp = pthread_cond_timedwait(&G_cond,&G_mutex,&timeout);//超时等待(异步查询用法,不切换线程)
		pthread_mutex_unlock(&G_mutex);
		switch(tmp){
			case 0:
				printf("pthread_cond_timedwait() okay, count=%d\n",++count);
				break;
			case EINTR:			//4
				printf("pthread_cond_timedwait() wake up by singal, count=%d\n",++count);
				break;
			case ETIMEDOUT:	//110
				//printf("pthread_cond_timedwait() timeout\n");
				break;
			default:				//EINVAL=22
				printf("pthread_cond_timedwait() unknow error, tmp=%d\n", tmp);
				break;
		}
	}
	printf("thread2 exit\n");
	return NULL;
}

int main(void){
	pthread_t thid1,thid2;

	pthread_mutex_init(&G_mutex,NULL);				//初始化mutex互斥锁
	pthread_cond_init(&G_cond,NULL);					//初始化cond条件变量

	if(pthread_create(&thid1,NULL,thread1,NULL) != 0){
		perror("pthread_create()");
		return -1;
	}
	if(pthread_create(&thid2,NULL,thread2,NULL) != 0){
		perror("pthread_create()");
		return -1;
	}

	sleep(1);

	pthread_mutex_unlock(&G_mutex);
	printf("main pthread_mutex_unlock()\n");	//直接解mutex锁, cond也不会松开(无影响)
	sleep(1);

	pthread_mutex_lock(&G_mutex);
	pthread_cond_signal(&G_cond);
	pthread_mutex_unlock(&G_mutex);
	printf("main pthread_cond_signal()\n");		//触发这个cond条件变量的waiting等待队列中的第一个等待者
	sleep(1);

	pthread_mutex_lock(&G_mutex);
	pthread_cond_broadcast(&G_cond);
	pthread_mutex_unlock(&G_mutex);
	printf("main pthread_cond_broadcast()\n");//触发这个cond条件变量的waiting等待队列中的所有等待者
	sleep(1);

	G_exit = 1;
	pthread_mutex_lock(&G_mutex);
	pthread_cond_broadcast(&G_cond);					//销毁前必须先调用broadcast,防止被卡死,
	pthread_cond_destroy(&G_cond);						//一旦销毁失败,不会返回EBUSY,而是直接卡死,
	pthread_mutex_unlock(&G_mutex);						//等待资源松绑了,才解开返回EBUSY(这样做就没意义了)
	pthread_mutex_destroy(&G_mutex);

	if(pthread_join(thid1,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}
	if(pthread_join(thid2,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}
	return 0;
}





