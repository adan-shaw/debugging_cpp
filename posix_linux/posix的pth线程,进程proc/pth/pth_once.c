//编译:
//		gcc pth_once.c -g3 -lpthread -o x 



//pthread_once()函数:
/*
	Linux Threads使用互斥锁和条件变量[pthread_once()实际也是由pthread线程+互斥锁+条件变量实现的], 
	保证由pthread_once()指定的函数执行且仅执行一次, 而once_control则表征是否执行过;
*/

//函数原型:
/*
	int pthread_once(pthread_once_t *once_control, void (*init_routine) (void));

	once_control参数:
		必须有signed long int实体, 否则内存溢出Segmentation fault;

		初值为0, PTHREAD_ONCE_INIT, 表示这个pthread_once()未被执行过. 调用的进程可以马上抢执行权.
		初值为1, 则由于所有pthread_once()都必须等待其中一个激发"已执行一次"信号,因此所有pthread_once()都会陷入永久的等待中;
		初值为2, 则表示该函数已执行过一次, 从而所有pthread_once()都会立即返回0.


	void (*init_routine) (void) 运行函数指针:
		被执行的函数不能传递参数, 不能返回参数;


	总结:
		简单地用来执行一次性任务, 好用;
		其他用法就别想了, 别浪费时间, 其他用法就用标准pthread_create() 更好;
*/



#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


//pthread_t 的变量类型的本质
//typedef unsigned long int pthread_t;


//这个值必须有pthread_once_t载体, 否则内存溢出Segmentation fault
pthread_once_t once = PTHREAD_ONCE_INIT;



//func_once() 不能传递参数, 也不能返回;(只被执行一次的函数)
void func_once(void){
	printf("func_once() run in pth: %u\n", pthread_self());
	return;
}

void* task1(void* arg){
	unsigned long int thid = pthread_self();
	printf("begin: task1 run in pth: %u\n", thid);
	if(pthread_once(&once, func_once) != 0){
		perror("pthread_once()");
		return NULL;
	}
	printf("end: task1 run in pth: %u\n", thid);
	return NULL;
}

void* task2(void* arg){
	unsigned long int thid = pthread_self();
	printf("begin: task2 run in pth: %u\n", thid);
	if(pthread_once(&once, func_once) != 0){
		perror("pthread_once()");
		return NULL;
	}
	printf("end: task2 run in pth: %u\n", thid);
	return NULL;
}

int main(void){
	pthread_t thid1, thid2;

	if(pthread_create(&thid1, NULL, (void*)task1, NULL) != 0){
		perror("pthread_create()");
		return -1;
	}
	if(pthread_create(&thid2, NULL, (void*)task2, NULL) != 0){
		perror("pthread_create()");
		return -1;
	}

	sleep(1);

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
