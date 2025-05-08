//编译:
//		gcc ./clone最底层的进程创建方式.c -g3 -lpthread -o x 



#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>



//clone()与fork(),pthread_create()的区别:
/*
	clone与fork() 的区别:
		从linux 2.3.3开始,glibc的fork()封装作为NPTL(Native POSIX Threads Library)线程实现的一部分;
		调用fork()等效于调用clone(2)时, 仅指定flags为SIGCHLD(共享信号句柄表);

	clone与pthread_create() 的区别:
		pthread_create()内部使用的也是clone函数(在glibc的/sysdeps/unix/sysv/linux/createthread.c源码中可以看到);
		创建线程的函数create_thread中使用了clone函数, 并指定了相关的flags:
			const int clone_flags = (CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SYSVSEM
															 |CLONE_SIGHAND|CLONE_THREAD
															 |CLONE_SETTLS|CLONE_PARENT_SETTID
															 |CLONE_CHILD_CLEARTID|0);
			TLS_DEFINE_INIT_TP (tp, pd);
			if(__glibc_unlikely(ARCH_CLONE(&start_thread, STACK_VARIABLES_ARGS, clone_flags, pd, &pd->tid, tp, &pd->tid) == -1))
			...
*/



//clone()函数原型:
/*
	clone()头文件:
		#define _GNU_SOURCE
		#include <sched.h>

	clone()函数原型:
		int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...);
			...多参数一般填充数据为: pid_t *parent_tid, void *tls, pid_t *child_tid

		long clone3(struct clone_args *cl_args, size_t size);
			size参数为:            cl_args结构体的大小(size参数可允许未来对clone_args进行扩展)

	clone3()用到的结构体(其实就是将旧式的clone()多参数的部分,弄成一个结构体,比较直观好处理):
		struct clone_args {
			u64 flags;        // Flags bit mask 
			u64 pidfd;        // Where to store PID file descriptor (pid_t *) 
			u64 child_tid;    // Where to store child TID, in child's memory (pid_t *) 
			u64 parent_tid;   // Where to store child TID, in parent's memory (int *) 
			u64 exit_signal;  // Signal to deliver to parent on child termination 
			u64 stack;        // Pointer to lowest byte of stack 
			u64 stack_size;   // Size of stack 
			u64 tls;          // Location of new TLS 
			u64 set_tid;      // Pointer to a pid_t array (since Linux 5.5) 
			u64 set_tid_size; // Number of elements in set_tid (since Linux 5.5) 
			u64 cgroup;       // File descriptor for target cgroup of child (since Linux 5.7) 
		};

	clone()和clone3() 选用哪个:
		clone() 简单一点, clone3() 需要自己填充struct clone_args{} 结构体, 十分麻烦;
		clone() 只需指定好flags 参数即可, 相对简单


	详细参数解释(内容超级多):
		flags 参数:
			CLONE_PARENT  创建的子进程的父进程是调用者的父进程, 新进程与创建它的进程成了"兄弟"而不是"父子"
			CLONE_FS      子进程与父进程共享相同的文件系统, 包括root、当前目录、umask
			CLONE_FILES   子进程与父进程共享相同的文件描述符(file descriptor)表
			CLONE_NEWNS   在新的namespace启动子进程, namespace描述了进程的文件hierarchy
			CLONE_SIGHAND 子进程与父进程共享相同的信号处理(signal handler)表
			CLONE_PTRACE  若父进程被trace, 子进程也被trace
			CLONE_VFORK   父进程被挂起, 直至子进程释放虚拟内存资源
			CLONE_VM      子进程与父进程运行于相同的内存空间
			CLONE_PID     子进程在创建时PID与父进程一致
			CLONE_THREAD  Linux 2.4中增加以支持POSIX线程标准, 子进程与父进程共享相同的线程群
			CLONE_SYSVSEM 如果设置了该标志, 则子进程和调用进程会共享一组System V semaphore adjustment(semadj)值
			CLONE_SETTLS  将TLS(Thread Local Storage)保存到tls字段中

			CLONE_CHILD_CLEARTID 当子线程存在时, 清除(置零)子线程内存的child_tid
			CLONE_CHILD_SETTID   在child_tid[(clone()]或cl_args.child_tid[(clone3()]的位置上保存线程ID
			CLONE_PARENT_SETTID  在父进程的parent_tid[(clone()]或cl_args.parent_tid[(clone3()]中保存子线程ID
			
	从Linux2.5.35开始, 如果指定了CLONE_THREAD, 则必须同时指定CLONE_SIGHAND;
	注意: 从Linux2.6.0开始, 指定CLONE_SIGHAND 的同时也必须指定CLONE_VM;
	也就是: CLONE_THREAD|CLONE_SIGHAND|CLONE_VM 必须一起用;
	(所以, 一旦启用线程, 就必须共享信号, 没办法剔除)
*/



//常用见的clone()用法:
/*
	在线程原版的基础上, 剔除信号共享, 提高安全性, 崩溃互不影响, 但又能最大限度共享内存
	(你不就想这样嘛, 只有这样做, 多进程才会比多线程好用;
	 但这样做也不能让多进程共享互斥锁? 没准多进程互斥锁, 只能在CLONE_THREAD 多线程属性中使用;
	 但也有可能可以用, 试试)

				|CLONE_SIGHAND|CLONE_THREAD //直接在线程的基础上, 去除CLONE_THREAD & CLONE_SIGHAND信号共享;
				|CLONE_CHILD_CLEARTID       //去掉子线程pid变成: CLONE_CHILD_SETTID创建子进程pid, 这样clone() 会返回新子进程的pid;
				|CLONE_SETTLS               //去掉TLS(Thread Local Storage) tls字段;
				|CLONE_VM                   //附带CLONE_VM选项,中断会导致主线程被牵连(必然牵连),
																		//也就是不能共享内存,否则指针崩溃会受牵连,子进程回收空间时,会把父进程的内存空间也给回收了;
				|SIGCHLD                    //新增SIGCHLD选项,那么父进程就可以使用wait()/waitpid()操作子进程了,且崩溃不互相影响;
*/

//常见的clone()组合用法1: (子进程崩溃互不影响, 但不能共享CLONE_VM, 还是得用shm/mmap + 信号量做进程同步)
const int clone_flags1 = (CLONE_FS|CLONE_FILES|CLONE_SYSVSEM|SIGCHLD
												 |CLONE_PARENT_SETTID|CLONE_CHILD_SETTID|0);

//常见的clone()组合用法2: (子进程崩溃互相影响, 但能共享CLONE_VM, 可以用pthread_mutex 互斥锁)
const int clone_flags2 = (CLONE_FS|CLONE_FILES|CLONE_SYSVSEM|SIGCHLD|CLONE_VM
												 |CLONE_PARENT_SETTID|CLONE_CHILD_SETTID|0);

static pthread_mutex_t mutex;				//互斥锁
static pthread_cond_t cond;					//条件变量
static int val = 0;



//(1048576 = 1024*1024)
#define child_stack_max (1048576)

//新建的子进程的main线程栈(也可以用mmap 创建的内存块来做子进程的main线程栈)
static char child_stack[child_stack_max];



//断言中断:互不影响, 但不能共享CLONE_VM
static int child_func1(void){
	printf("child process PID: %ld\n", (long)getpid());
	printf("child process parent PID: %ld\n", (long)getppid());
	sleep(1);
	assert(0);												//子进程断言中断, 看父进程是否会受影响
	return 0;
}

void test_child_func1(void){
	pid_t child_pid;
	int child_ret, tmp;

	printf("main proecess PID: %ld\n", (long)getpid());
	child_pid = clone(child_func1, child_stack+child_stack_max, clone_flags1, NULL);
	printf("clone() return = %ld\n", (long)child_pid);

	tmp = waitpid(child_pid, &child_ret, 0);
	if(tmp == -1)
		perror("waitpid()");
	else
		printf("child %d exit(%d), waitpid()=%d\n", child_pid, child_ret, tmp);

	if(fork() == 0)										//fork() 可以扛assert() 中断不退出
		assert(0);

	sleep(2);
	printf("assert() test okay?\n");	//打印两个assert()断言之后,进程没有退出,可以看到这个打印,则测试成功
	return;
}



//断言中断:互相影响, 但共享CLONE_VM(可以用互斥锁&条件变量)
static int child_func2(void){
	struct timespec timeout;

	printf("child process PID: %ld\n", (long)getpid());
	printf("child process parent PID: %ld\n", (long)getppid());

	val = 999;												//证明CLONE_VM共享不是写时拷贝

	timeout.tv_sec = time(0)+2;				//2秒timeout
	timeout.tv_nsec = 0;
	//条件变量:无论成功or超时or失败,都不需要解锁(会自动松开互斥锁的)
	if(pthread_cond_timedwait(&cond,&mutex,&timeout) == 0)
		val = 1;												//子进程死等父进程唤醒条件变量, 如唤醒, 则val=1
	else{
		if(errno == 0)
			printf("pthread_cond_timedwait() timeout\n");
		else
			perror("pthread_cond_timedwait()");
	}
	return 0;
}

void test_child_func2(void){
	pid_t child_pid;
	int child_ret, tmp;
	pthread_mutex_init(&mutex,NULL);	//初始化互斥锁
	pthread_cond_init(&cond,NULL);		//初始化条件变量


	printf("main proecess PID: %ld\n", (long)getpid());
	child_pid = clone(child_func2, child_stack+child_stack_max, clone_flags2, NULL);
	printf("clone() return = %ld\n", (long)child_pid);

	sleep(1);													//等子进程先陷入条件变量等待
	if(pthread_cond_signal(&cond) != 0)//唤醒子进程的条件变量
		perror("pthread_cond_signal()");
	sleep(1);													//等待子进程结束, 才销毁条件变量&互斥量

	printf("val = %d\n(互斥锁,条件变量可用,则val=1;\n 子进程CLONE_VM共享不是写时拷贝,但互斥锁,条件变量不可用,则val=999)\n",val);
	if(val == 1)
		printf("child process 共享内存页CLONE_VM okay, 互斥锁, 条件变量共享okay!!\n");

	pthread_cond_destroy(&cond);			//销毁条件变量
	pthread_mutex_destroy(&mutex);		//销毁互斥锁
	return;
}



int main(void){
	test_child_func1();
	test_child_func2();
	return 0;
}




