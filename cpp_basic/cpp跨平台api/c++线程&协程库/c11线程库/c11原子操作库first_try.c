//编译:
//		gcc -g3 -pthread ./c11原子操作库first_try.c -o x 



//判断编译器是否支持c11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
#endif

//c11 线程库, 只能用在纯c 语言里面, 估计插入c++ 里面需要使用extern "C" {}, 不能单纯插入*.cpp 代码中, 否则会有很多报错;

#include <stdio.h>

//'c11线程库'
#include <stdatomic.h>
#include <threads.h>



void c11(void){
	atomic_int tmp;
	tmp = 10;
	printf("tmp = %d\n", (int)tmp);
	printf("atomic_int tmp is lock free? %s\n", atomic_is_lock_free(&tmp) ? "true" : "false");
	return;
}

int main(void){
	c11();
	return 0;
}

//问题解析:
/*
	* 只要将*.c 源文件, 改成*.cpp, 就会触发编译器认为这是c++ 文件, 从而导致编译错误;

	* #include <cstdio>
		这种写法同样会让编译器认为这是c++ 文件, 从而导致编译错误;

	* 不能用g++, 用gcc 编译这个文件, 才能正确编译;
		(#include <stdatomic.h> 可能要求很苛刻)
*/
