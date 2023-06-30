//编译:
//		gcc -g3 -pthread ./c11原子操作库bug.cpp -o x 


//判断编译器是否支持c11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
#endif



#include <cstdio>

//'c11线程库'
#include <stdatomic.h>//只要包含就出错, 不知道为何
#include <threads.h>

//借用c++ 的方法, 可以编译通过, 但还是c++, 不是纯c(没意义)
/*
#ifndef __cplusplus
# include <stdatomic.h>
#else
# include <atomic>
# define _Atomic(X) std::atomic< X >
#endif
*/



void c11(void){
	/*
	atomic_int tmp;
	tmp = 10;
	printf("tmp = %d\n", (int)tmp);
	printf("atomic_int tmp is lock free? %s\n", atomic_is_lock_free(&tmp) ? "true" : "false");
	*/
	return;
}

int main(void){
	c11();
	return 0;
}
