//编译:
//		gcc pid线程\&进程.c -g3 -lpthread -o x 



//进程pid:
/*
	pid_t 变量类型, 实际上是int;
		typedef __pid_t pid_t;
		__STD_TYPE __PID_T_TYPE __pid_t;
		#define __PID_T_TYPE __S32_TYPE
		#define __S32_TYPE int

	因此, 你可以用pid_t / int 来装载进程pid;

	进程pid API:
		pid_t getpid(void);		//获取自身pid(永远不会失败)
		pid_t getppid(void);	//获取父进程pid(永远不会失败)
*/



//线程pid:
/*
	pthread_t 变量类型, 实际上差异很大, 在linux 上面是:
		typedef unsigned long int pthread_t;

	linux下,   pthread_t是unsigned long类型;
	solaris下, pthread_t是unsigned int类型;
	FreeBSD下, pthread_t是一个结构体指针;

	如果你要比较两个pthread_t 是否相等, 不能直接使用if(==)判读, 而应该使用pthread_equal来判断!!


	pthread_t pthread_self(void);										//返回自身的pthread_t(永远不会失败)

	int pthread_equal(pthread_t t1, pthread_t t2);	//判断两个pthread_t是否相等(不想等返回0,相等返回非0值)
*/



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void test_pid_t(void){
	pid_t pid1 = getpid();
	pid_t pid2 = getppid();
	if(pid1 != pid2)
		printf("pid1 != pid2\n");
	return;
}

void test_pthread_t(void){
	pthread_t thid = pthread_self();
	pthread_t thid2 = pthread_self();

	if(pthread_equal(thid,thid2) != 0)
		printf("thid == thid2\n");
	else
		printf("thid != thid2\n");
	return;
}

int main(void){
	test_pid_t();
	test_pthread_t();
	return 0;
}



