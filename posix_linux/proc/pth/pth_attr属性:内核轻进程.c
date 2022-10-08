//编译:
//		gcc pth_attr属性\:内核轻进程.c -g3 -lpthread -o x 



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



//pth_attr属性修改:创建内核轻进程
/*
	修改pth_attr属性, 只有3 大类:
		detach属性(可用API代替)
		'线程调度'相关的参数(可用API代替)
		'线程栈'相关的参数

	一旦你修改了'线程栈'相关的参数, 动了默认线程栈, 系统默认你已经会管理'线程栈'了, 
	那么栈末尾的警戒缓冲区参数会自动无效, 等同于把guardsize线程属性设为0;
	guardsize=0 的线程, 即轻进程(LWP=Light Weight Process), 即内核进程;
	(实时系统才会需要内核进程, 优先级高, 实时性强)

	总结:
		如果是分时系统, 基本没必要动线程属性;
		如果是实时系统, 很有必要改成LWP内核进程, 提高响应速度, 提高实时准确性;


	疑难杂证:
		* 无论怎么样, 都没办法修改'实时调度权值'(废弃)

		* 分时线程, 可以选择'继承/不继承'父进程的调度策略
			实时线程, 必须'继承'父进程的调度策略, 否则调度失败, why??
*/



//快速重建API(for debug only), 实体变量版本, attr必须为实体变量, 不能是指针;
//[如果不喜欢, 也可以快速替换字符串, 消灭这几个红定义]
#define pthread_attr_initEx(attr) { if(pthread_attr_init(attr) != 0) perror("pthread_attr_init()"); }
#define pthread_attr_destroyEx(attr) { if(pthread_attr_destroy(attr) != 0) perror("pthread_attr_destroy()"); }
#define pthread_attr_setdetachstateEx(attr, iVal) { if(pthread_attr_setdetachstate(attr, iVal) != 0) perror("pthread_attr_setdetachstate()"); }
#define pthread_attr_setstackEx(attr, pvoid, uiVal) { if(pthread_attr_setstack(attr, pvoid, uiVal) != 0) perror("pthread_attr_setstack()"); }
#define pthread_attr_setstacksizeEx(attr, uiVal) { if(pthread_attr_setstacksize(attr, uiVal) != 0) perror("pthread_attr_setstacksize()"); }
#define pthread_attr_setguardsizeEx(attr, uiVal) { if(pthread_attr_setguardsize(attr, uiVal) != 0) perror("pthread_attr_setguardsize()"); }
#define pthread_attr_setinheritschedEx(attr, iVal) { if(pthread_attr_setinheritsched(attr, iVal) != 0) perror("pthread_attr_setinheritsched()"); }
#define pthread_attr_setschedpolicyEx(attr, iVal) { if(pthread_attr_setschedpolicy(attr, iVal) != 0) perror("pthread_attr_setschedpolicy()"); }
#define pthread_attr_setscopeEx(attr, iVal) { if(pthread_attr_setscope(attr, iVal) != 0) perror("pthread_attr_setscope()"); }
#define pthread_attr_setschedparamEx(attr, psched_param) { if(pthread_attr_setschedparam(attr, psched_param) != 0) perror("pthread_attr_setschedparam()"); }



#define PTH_STACK_BUF_MAX (8*1024*1024)		//线程栈max
#define PTH_STACK_GUARD_MAX (0)						//线程栈警戒缓冲区max
#define PTH_SCHED_PRIO_VAL (20)						//线程实时调度权值(一般实时调度才改权值的)



void* pth_func(void* arg){
	int tmp = 3;
	while(tmp-- > 0){
		printf("pth_func() tmp: %d\n", tmp);
		sleep(1);
	}
	return NULL;
}

int main(void){
	pthread_t thid;
	pthread_attr_t attr;
	struct sched_param sched_prio;

	sched_prio.sched_priority = PTH_SCHED_PRIO_VAL;								//初始化'调度权值'

	void* pth_stack = malloc(PTH_STACK_BUF_MAX);
	if(pth_stack == NULL){//线程栈的缓冲区, 必须在'全局变量区/静态变量区/堆内存区'等CLONE_VM 共享的内存区中创建;
		perror("malloc()");
		return -1;
	}

	pthread_attr_initEx(&attr);																		//初始化attr 线程属性

	pthread_attr_setdetachstateEx(&attr, PTHREAD_CREATE_JOINABLE);//可获取pth退出状态记录

	pthread_attr_setinheritschedEx(&attr, PTHREAD_INHERIT_SCHED);	//继承父进程的调度策略(实时线程不能拒绝继承父进程的调度策略, 否则pthread_create失败)
	//pthread_attr_setschedparamEx(&attr, &sched_prio);							//设置的'实时调度权值'(范围:1-99, 权值高就是大爷, 优先级高)[总是出错, 不能修改权值?]
	//pthread_attr_setschedpolicyEx(&attr, SCHED_OTHER);					//设置新的调度策略: 分时
	pthread_attr_setschedpolicyEx(&attr, SCHED_RR);								//设置新的调度策略: 实时-时间片
	//pthread_attr_setschedpolicyEx(&attr, SCHED_FIFO);						//设置新的调度策略: 实时-抢占

	pthread_attr_setstacksizeEx(&attr, PTH_STACK_BUF_MAX);				//设置线程栈大小
	pthread_attr_setguardsizeEx(&attr, PTH_STACK_GUARD_MAX);			//设置线程栈警戒缓冲区大小
	pthread_attr_setstackEx(&attr, pth_stack, PTH_STACK_BUF_MAX);	//设置线程栈

	pthread_attr_setscopeEx(&attr, PTHREAD_SCOPE_SYSTEM);					//设置为LWP轻进程


	//第四参数, 传递void* arg, 为了简约主义, 这里忽略;
	if(pthread_create(&thid, &attr, (void*)pth_func, NULL) != 0){	//启动实时线程
		perror("pthread_create()");
		return -1;
	}
	else{
		sleep(4);

		if(pthread_join(thid,NULL) != 0){														//获取线程结束状态
			perror("pthread_join()");
			return -1;
		}

		free(pth_stack);																						//创建线程失败的话, 内核会自动释放pth_stack, 只有创建线程成功, 才需要手动释放pth_stack
	}

	pthread_attr_destroyEx(&attr);																//销毁attr 线程属性
	return 0;
}

