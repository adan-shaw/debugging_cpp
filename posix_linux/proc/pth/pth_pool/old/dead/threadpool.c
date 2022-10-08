#include "threadpool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>


//线程函数(主要就是询问链队列有没有任务, 没有就休眠)
void *thread_routime(void *arg){
	printf("thread %d is starting\n", pthread_self());
	threadpool_t *pool = (threadpool_t *)arg;

	while(1){
		bool timeout = false;
		pthread_mutex_lock(&pool->ready.pmutex);
		pool->idle++;

		//等待队列有任务到来 or 等待线程池销毁通知
		struct timespec abstime;
		while(pool->first == NULL && !pool->quit){
			//printf("thread %d is waiting\n", pthread_self());

			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec += wait_mission_timeout;


			int status = pthread_cond_timedwait(&pool->ready.pcond, &pool->ready.pmutex, &abstime);
			if(status == ETIMEDOUT){
				printf("thread %d wait timed out\n", pthread_self());
				timeout = true;
				break;
			}
		}

		pool->idle--;

		//检查任务链队列
		if(pool->first != NULL){
			task_t *t = pool->first;
			pool->first = t->next;

			pthread_mutex_unlock(&pool->ready.pmutex);

			t->run(t->arg);
			free(t);

			continue;
			//pthread_mutex_lock(&pool->ready.pmutex);//重新加锁
		}

		//退出线程池 && 任务队列已经为空
		if(pool->quit && pool->first == NULL){
			pool->counter--;

			if(pool->counter == 0){
				pthread_cond_signal(&pool->ready.pcond);
			}

			pthread_mutex_unlock(&pool->ready.pmutex);
			break;
		}

		//超时, 跳出销毁线程
		if(timeout == true){
			pool->counter--;
			pthread_mutex_unlock(&pool->ready.pmutex);
			break;
		}

		//pthread_mutex_unlock(&pool->ready.pmutex);
	}

	printf("thread %d already quit\n", pthread_self());
	return NULL;
}


//线程池初始化
bool threadpool_init(threadpool_t *pool, int threads){
	if(pthread_mutex_init(&pool->ready.pmutex, NULL) != 0){
		perror("pthread_mutex_init()");
		return false;
	}
	if(pthread_cond_init(&pool->ready.pcond, NULL) != 0){
		perror("pthread_cond_init()");
		return false;
	}
	pool->first = NULL;
	pool->last =NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->max_threads = threads;
	pool->quit = false;
	return true;
}


//增加一个任务到线程池
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg){
	task_t *newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;

	pthread_mutex_lock(&pool->ready.pmutex);

	if(pool->first == NULL)
		pool->first = newtask;
	else
		pool->last->next = newtask;

	pool->last = newtask;

	if(pool->idle > 0)
		pthread_cond_signal(&pool->ready.pcond);
	else if(pool->counter < pool->max_threads){
		pthread_t tid;
		if(pthread_create(&tid, NULL, thread_routime, pool) == 0)
			pool->counter++;
	}

	pthread_mutex_unlock(&pool->ready.pmutex);
}

//线程池销毁
bool threadpool_destroy(threadpool_t *pool){
	if(pool->quit){
		printf("threadpool_destroy() already quit\n");
		return false;
	}

	pthread_mutex_lock(&pool->ready.pmutex);
	pool->quit = true;

	if(pool->counter > 0){
		if(pool->idle > 0)
			pthread_cond_broadcast(&pool->ready.pcond);

		while(pool->counter){
			pthread_cond_wait(&pool->ready.pcond, &pool->ready.pmutex);
		}
	}
	pthread_mutex_unlock(&pool->ready.pmutex);

	//清除资源
	if(pthread_mutex_destroy(&pool->ready.pmutex) != 0){
		perror("pthread_mutex_destroy()");
		return false;
	}

	if(pthread_cond_destroy(&pool->ready.pcond) != 0){
		perror("pthread_cond_destroy()");
		return false;
	}
	return true;
}
