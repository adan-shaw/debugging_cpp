//编译:
//		gcc posix_spin.c -g3 -lpthread -o x 



//spin lock自旋锁操作API
/*
	int pthread_spin_destroy(pthread_spinlock_t *lock);
	int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
	int pthread_spin_lock(pthread_spinlock_t *lock);
	int pthread_spin_trylock(pthread_spinlock_t *lock);
	int pthread_spin_unlock(pthread_spinlock_t *lock);
*/

//spin自旋锁属性修改API
/*
	(略)
*/



#include <pthread.h>
#include <stdio.h>



//互斥锁的所有操作
void spin_all_api(void){
	int G_tmp = 0;
	struct timespec timeout;
	pthread_spinlock_t spin;

	pthread_spin_init(&spin,0);			//初始化spin锁



	//加锁方法1:
	pthread_spin_lock(&spin);				//死等spin锁(若阻塞也不会触发线程切换)
	G_tmp += 1;
	pthread_spin_unlock(&spin);


	//加锁方法2:
	//pthread_spin_lock(&spin);
	if(pthread_spin_trylock(&spin) == 0){
		G_tmp += 1;										//非阻塞加spin锁(单次询问)
		pthread_spin_unlock(&spin);
	}
	else{
		printf("pthread_spin_trywrlock() failed, next move??\n");
	}



	pthread_spin_destroy(&spin);		//销毁spin锁
	return;
}



int main(void){
	spin_all_api();
	return 0;
}
