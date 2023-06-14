//编译:
//		gcc posix_rwlock.c -g3 -lpthread -o x 



//rwlock读写锁操作API
/*
	int pthread_rwlock_destroy(pthread_rwlock_t *lock);
	int pthread_rwlock_init(pthread_rwlock_t *lockrestrict,const pthread_rwlockattr_t *attrrestrict);
	int pthread_rwlock_timedrdlock(pthread_rwlock_t *lockrestrict,const struct timespec *restrict);
	int pthread_rwlock_timedwrlock(pthread_rwlock_t *lockrestrict,const struct timespec *restrict);
	int pthread_rwlock_tryrdlock(pthread_rwlock_t *lock);
	int pthread_rwlock_trywrlock(pthread_rwlock_t *lock);
	int pthread_rwlock_unlock(pthread_rwlock_t *lock);
	int pthread_rwlock_rdlock(pthread_rwlock_t *lock);
	int pthread_rwlock_wrlock(pthread_rwlock_t *lock);
*/

//rwlock读写锁属性修改API(略)
/*
	int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
	int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
	int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attrrestrict, int *restrict);
	int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int);
*/



#include <pthread.h>
#include <stdio.h>



//rwlock读写锁的所有操作
void rwlock_all_api(void){
	int G_tmp = 0;
	struct timespec timeout;
	pthread_rwlock_t rwlock;

	pthread_rwlock_init(&rwlock,0);			//初始化rwlock锁



	//加锁方法1:
	pthread_rwlock_rdlock(&rwlock);			//死等读锁(若阻塞触发线程切换)
	if(G_tmp != 0) printf("1-G_tmp != 0\n");
	pthread_rwlock_unlock(&rwlock);			//解锁

	pthread_rwlock_wrlock(&rwlock);			//死等写锁(若阻塞触发线程切换)
	G_tmp += 1;
	pthread_rwlock_unlock(&rwlock);


	//加锁方法2:
	if(pthread_rwlock_tryrdlock(&rwlock) == 0){
		if(G_tmp != 0) printf("2-G_tmp != 0\n");
		pthread_rwlock_unlock(&rwlock);		//非阻塞加读锁(单次询问)
	}
	else{
		printf("pthread_rwlock_tryrdlock() failed, next move??\n");
	}

	if(pthread_rwlock_trywrlock(&rwlock) == 0){
		G_tmp += 1;												//非阻塞加写锁(单次询问)
		pthread_rwlock_unlock(&rwlock);
	}
	else{
		printf("pthread_rwlock_trywrlock() failed, next move??\n");
	}


	//加锁方法3:
	while(pthread_rwlock_tryrdlock(&rwlock) == 16)
		;																	//非阻塞加读锁(死等,但不切换线程,一直等到有锁为止)
	if(G_tmp != 0) printf("3-G_tmp != 0\n");
	pthread_rwlock_unlock(&rwlock);

	printf("%d\n",pthread_rwlock_tryrdlock(&rwlock));
	printf("%d\n",pthread_rwlock_tryrdlock(&rwlock));
	while(pthread_rwlock_trywrlock(&rwlock) == 16)
		;																	//非阻塞加写锁(死等,但不切换线程,一直等到有锁为止)
	G_tmp += 1;
	pthread_rwlock_unlock(&rwlock);



	pthread_rwlock_destroy(&rwlock);		//销毁rwlock锁
	return;
}



int main(void){
	rwlock_all_api();
	return 0;
}
