//编译:
//		gcc posix_mutex.c -g3 -lpthread -o x 



//互斥锁操作API
/*
	int pthread_mutex_consistent(pthread_mutex_t *);
	int pthread_mutex_getprioceiling(const pthread_mutex_t *restrict,int *restrict);
	int pthread_mutex_setprioceiling(pthread_mutex_t *restrict, int,int *restrict);
	int pthread_mutex_init(pthread_mutex_t *restrict,const pthread_mutexattr_t *restrict);
	int pthread_mutex_destroy(pthread_mutex_t *);
	int pthread_mutex_timedlock(pthread_mutex_t *restrict,const struct timespec *restrict);
	int pthread_mutex_lock(pthread_mutex_t *);
	int pthread_mutex_trylock(pthread_mutex_t *);
	int pthread_mutex_unlock(pthread_mutex_t *);

	pthread_mutex_init()							//初始化互斥锁(一定会成功的, 永远不会失败, 总是返回0)
	pthread_mutex_destroy()						//销毁互斥锁
	pthread_mutex_timedlock()					//阻塞+超时, 若阻塞触发线程切换, 超时后cpu会自动唤醒线程继续运行
	pthread_mutex_lock()							//阻塞+死等, 若阻塞触发线程切换[mutex空闲返回0, mutex被占用则返回16]
	pthread_mutex_trylock()						//非阻塞加锁
	pthread_mutex_unlock()						//解锁
*/



//互斥体属性修改API
/*
	int pthread_mutexattr_destroy(pthread_mutexattr_t *);
	int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *restrict, int *restrict);
	int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict,int *restrict);
	int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict,int *restrict);
	int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict,int *restrict);
	int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict,int *restrict);
	int pthread_mutexattr_init(pthread_mutexattr_t *);
	int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
	int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
	int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
	int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
	int pthread_mutexattr_settype(pthread_mutexattr_t *, int);


	互斥锁的属性, 可以在创建锁的时候, pthread_mutex_init()的第二参数指定;
	如果pthread_mutex_init()的第二参数为NULL, 则创建默认的PTHREAD_MUTEX_TIMED_NP普通锁;
	在LinuxThreads实现中仅有一个锁类型属性(linux 不允许创建其它类型的互斥锁??)

	当前(glibc2.2.3,linuxthreads0.9)有四个值可供选择:
		PTHREAD_MUTEX_TIMED_NP(缺省值, 普通锁):						[linux默认只有这种fast互斥锁]
			当一个线程加锁后, 其余请求锁的线程将形成一个等待队列, 
			解锁后, 等待队列中的线程, 按优先级重新抢锁(解锁的线程, 一般会重新安排到等待队列的尾部);
			这种锁策略保证了资源分配的公平性;

		PTHREAD_MUTEX_RECURSIVE_NP(嵌套锁):
			当一个线程加锁后, 其余请求锁的线程将形成一个等待队列, 
			解锁后, 如再次发生多线程竞争, 上一次解锁的线程具有重新获取锁的优先权;

		PTHREAD_MUTEX_ERRORCHECK_NP(检错锁):							[error checking互斥体的意义]
			当一个线程加锁后, 其余请求锁的线程访问资源, 则返回EDEADLK;
			获取资源成功的话, 与PTHREAD_MUTEX_TIMED_NP类型动作相同;
			跟pthread_mutex_trylock() 操作类似;

		PTHREAD_MUTEX_ADAPTIVE_NP(适应锁):
			动作最简单的锁类型, 仅等待解锁后重新竞争;


	PTHREAD_MUTEX_TIMED_NP = fast互斥体
	PTHREAD_MUTEX_RECURSIVE_NP = recursive互斥体
	PTHREAD_MUTEX_ERRORCHECK_NP = error checking互斥体

	fast互斥体:
		当线程访问临界区, 但没有抢到锁的时候, 简单地陷入阻塞, 并将线程挂起;

	error checking互斥体:
		当线程访问临界区, 但没有抢到锁的时候, 立即返回一个错误码EDEADLK;



	带有NP后缀的pthread 线程锁, 这是POSIX标准的'不可移植的扩展', 不应该出现在可移植的程序中;


	pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t recmutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutex_t errchkmutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
*/



#include <pthread.h>
#include <stdio.h>



//测试创建不同类型的互斥锁
void test_mutex_all_type(void){
	pthread_mutex_t fastmutex;
	//pthread_mutex_t recmutex;
	//pthread_mutex_t errchkmutex;

	//PTHREAD_MUTEX_TIMED_NP = NULL = 普通互斥锁
	pthread_mutex_init(&fastmutex,PTHREAD_MUTEX_TIMED_NP);
	//linux 不允许创建其它类型的互斥锁, 否则pthread_mutex_init() 调用会崩溃: Segmentation fault
	//pthread_mutex_init(&recmutex,PTHREAD_MUTEX_RECURSIVE_NP);
	//pthread_mutex_init(&errchkmutex,PTHREAD_MUTEX_ERRORCHECK_NP);

	pthread_mutex_destroy(&fastmutex);
	//pthread_mutex_destroy(&recmutex);
	//pthread_mutex_destroy(&errchkmutex);
	return;
}



//mutex互斥锁的所有操作
void mutex_all_api(void){
	int G_tmp = 0;
	struct timespec timeout;
	pthread_mutex_t mutex;

	//pthread_mutex_init(&mutex,PTHREAD_MUTEX_TIMED_NP);
	pthread_mutex_init(&mutex,0);			//初始化mutex锁



	//加锁方法1:
	pthread_mutex_lock(&mutex);				//死等(若阻塞触发线程切换)
	G_tmp += 1;
	pthread_mutex_unlock(&mutex);			//解锁

	//加锁方法2(弃用, 你不应该用这种方式操作mutex互斥量, 不安全):
	/*
	pthread_mutex_lock(&mutex);				//加锁1次, pthread_mutex_timedlock()错误返回110
	//pthread_mutex_lock(&mutex);			//加锁2次, pthread_mutex_timedlock()陷入死锁, 程序卡死
	timeout.tv_sec = 1;								//1秒
	timeout.tv_nsec = 0;							//0纳秒(1秒=10^9纳秒)
	if(pthread_mutex_timedlock(&mutex,&timeout) == 0){
		G_tmp += 1;											//超时, 若阻塞触发线程切换, 超时后cpu会自动唤醒线程继续运行
		pthread_mutex_unlock(&mutex);
	}
	else{															//(需要编写抢锁超时的后续操作)
		printf("pthread_mutex_timedlock() failed, next move??\n");
	}
	*/

	//加锁方法3:
	if(pthread_mutex_trylock(&mutex) == 0){
		G_tmp += 1;
		pthread_mutex_unlock(&mutex);		//非阻塞加锁(单次询问)
	}
	else{															//(需要编写抢锁失败的后续操作)
		printf("pthread_mutex_trylock() failed, next move??\n");
	}

	//加锁方法4:
	printf("%d\n",pthread_mutex_trylock(&mutex));
	printf("%d\n",pthread_mutex_trylock(&mutex));
	while(pthread_mutex_trylock(&mutex) == 16)
		;																//非阻塞加锁(死等,但不切换线程,一直等到有锁为止; 也可视作为自旋锁,CAS锁,只是消耗更大)
	G_tmp += 1;
	pthread_mutex_unlock(&mutex);



	pthread_mutex_destroy(&mutex);		//销毁mutex锁
	return;
}



int main(void){
	test_mutex_all_type();
	mutex_all_api();
	return 0;
}





