//编译:
//		gcc -g3 -pthread ./c11线程同步mutex.c -o x



//判断编译器是否支持c11 原子操作库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 threads.h ]
#else
	#pragma message("Support C11 threads.h")
	#include <threads.h>
#endif

#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
	#include <stdatomic.h>
#endif

#include <stdio.h>



atomic_int sum;

mtx_t m_mtx;



int thrd_func_mutex(void* parg){
	struct timespec tt;
	int tmp = 9999;
	atomic_exchange(&sum, (int)parg);								//赋值(原子操作)
	for(;tmp > 0;tmp--){
		atomic_fetch_add(&sum,1);
		printf("thrd_func_mutex: sum = %d\n", (int)sum);
	}

	if(mtx_lock(&m_mtx) != thrd_success){						//阻塞, 死等锁
		perror("mtx_lock()");
		return -1;
	}

	if(mtx_unlock(&m_mtx) != thrd_success){					//解锁
		perror("mtx_unlock()");
		return -1;
	}

	tmp = mtx_trylock(&m_mtx);											//非阻塞, 尝试获取锁
	if(tmp == thrd_error){
		perror("mtx_trylock()");
		return -1;
	}
	if(tmp == thrd_busy)
		printf("mtx_trylock(): thrd_busy\n");
	else
		mtx_unlock(&m_mtx);														//解锁

	tt.tv_sec = 1;																	//秒
	tt.tv_nsec = 0;																	//纳秒[1s=1000ms(毫秒)=1000*1000us(微妙)=1000*1000*1000ns(纳秒)]
	if(mtx_timedlock(&m_mtx,&tt) == thrd_error){		//阻塞等待锁, 超时1 秒
		perror("mtx_timedlock()");
		return -1;
	}

	mtx_unlock(&m_mtx);															//解锁

	return 6666;
}

//互斥锁test(有些复杂资源, 不能用简单原子变量来同步的, 就需要用到互斥锁)
/*
	互斥锁type 必须拥有下列值之一:
		mtx_plain                 - 创建简单的, 非递归互斥
		mtx_timed                 - 创建非递归的, 支持时间限制的互斥
		mtx_plain | mtx_recursive - 创建递归互斥
		mtx_timed | mtx_recursive - 创建递归的支持时限的互斥
*/
void thrd_mutex_test(void){
	thrd_t t1;
	int thrd_stat;

	if(mtx_init(&m_mtx, mtx_plain) != thrd_success){//初始化互斥锁
		perror("mtx_init()");
		return;
	}

	if(thrd_create(&t1, thrd_func_mutex, (void*)600) != thrd_success){
		perror("thrd_create()");
		return;
	}

	if(thrd_join(t1, &thrd_stat) != thrd_success){
		perror("thrd_join()");
		return;
	}
	printf("thrd_join(): thrd_stat=%d\n", thrd_stat);

	mtx_destroy(&m_mtx);														//销毁互斥锁
	return;
}

int main(void){
	thrd_mutex_test();
	return 0;
}



