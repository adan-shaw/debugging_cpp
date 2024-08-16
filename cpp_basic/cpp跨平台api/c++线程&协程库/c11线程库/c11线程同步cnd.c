//编译:
//		gcc -g3 -pthread ./c11线程同步cnd.c -o x
//		[win不支持]cl.exe ./c11线程同步cnd.c -EHsc -w -Zi -Ox -link -out:x.exe



//判断编译器是否支持c11 线程库[win不支持]
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 threads.h ]
#else
	#pragma message("Support C11 threads.h")
	#include <threads.h>
#endif

//判断编译器是否支持c11 原子操作库[win不支持]
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
	#include <stdatomic.h>
#endif

#include <stdio.h>



atomic_int sum;

mtx_t m_mtx;

cnd_t m_cnd;



int thrd_func_cnd(void* parg){
	struct timespec tt;
	int tmp = 9999;
	atomic_exchange(&sum, (int)parg);										//赋值(原子操作)
	for(;tmp > 0;tmp--){
		atomic_fetch_add(&sum,1);
		printf("thrd_func_cnd: sum = %d\n", (int)sum);
	}

	if(cnd_wait(&m_mtx,&m_cnd) != thrd_success){				//阻塞, 死等条件变量
		perror("cnd_wait()");
		return -1;
	}

	tt.tv_sec = 1;																			//秒
	tt.tv_nsec = 0;																			//纳秒[1s=1000ms(毫秒)=1000*1000us(微妙)=1000*1000*1000ns(纳秒)]
	if(cnd_timedwait(&m_mtx,&m_cnd,&tt) == thrd_error){	//阻塞等待条件变量, 超时1 秒
		perror("cnd_timedwait()");
		return -1;
	}

	return 6666;
}



//条件变量test(如果需要协调线程的启动/停止, 就需要用到条件变量)
void thrd_cnd_test(void){
	thrd_t t1;
	int thrd_stat;


	if(mtx_init(&m_mtx, mtx_plain) != thrd_success){		//初始化互斥锁
		perror("mtx_init()");
		return;
	}

	if(cnd_init(&m_cnd) != thrd_success){								//初始化条件变量
		perror("cnd_init()");
		return;
	}


	if(thrd_create(&t1, thrd_func_cnd, (void*)600) != thrd_success){
		perror("thrd_create()");
		return;
	}


	if(cnd_signal(&m_cnd) != thrd_success){							//唤醒一个等待条件变量的线程
		perror("cnd_signal()");
		return;
	}

	if(cnd_broadcast(&m_cnd) != thrd_success){					//唤醒所有等待条件变量的线程
		perror("cnd_broadcast()");
		return;
	}


	if(thrd_join(t1, &thrd_stat) != thrd_success){
		perror("thrd_join()");
		return;
	}
	printf("thrd_join(): thrd_stat=%d\n", thrd_stat);


	cnd_destroy(&m_cnd);																//销毁条件变量
	mtx_destroy(&m_mtx);																//销毁互斥锁
	return;
}

int main(void){
	thrd_cnd_test();
	return 0;
}








