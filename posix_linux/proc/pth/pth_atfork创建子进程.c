//编译:
//		gcc pth_atfork创建子进程.c -g3 -lpthread -o x 



//函数api
/*
	int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
			prepare:
				一个函数指针, fork()创建子进程之前, 父进程, 子线程, 共享CLONE_VM的子进程, 需要做的准备工作;
			parent:
				一个函数指针, fork()创建子进程之后, 父进程'需要做的准备工作';
			child:
				一个函数指针, fork()创建子进程之后, 子线程, 共享CLONE_VM的子进程'需要做的准备工作';

			成功返回0, 失败返回非0;
*/

//pthread_atfork()的用途:
/*
	一般情况下, 子线程, 共享CLONE_VM属性的子进程, 都与父进程共享CLONE_VM,
	共享CLONE_VM属性的子进程, 子线程, 只有一份内存空间,
	所以即便创建多个子进程, 子线程, 都不会有任何问题, 也不会有任何差异;

	但当父进程想要fork()创建子线程的时候, 由于fork()不是clone(), 默认会拷贝出一份新的'CLONE_VM内存空间'(读时共享, 写时复制);
	这种情况下, 就有两份'CLONE_VM内存空间';
	如何保证fork() 的时候, 两份'CLONE_VM内存空间'保持一致, 这时候就需要pthread_atfork();

	pthread_atfork() 的一般做法是:
		fork()之前, 对'父进程, 子线程, 共享CLONE_VM的子进程'中'所有mutex/rwlock/spin锁'进行上锁操作, 防止'CLONE_VM内存空间'拷贝不一致;
		只有当'CLONE_VM内存空间'拷贝一致, fork() 操作才算是有意义的;

	ps:
		fork() 绝对不会拷贝子线程吗?
		答:
			因为fork() 也是基于clone() 实现的, pthread_create() 也是基于clone() 实现的;
			原则上, linux 没有线程, 都是进程;
			所以, 不用异想天开, fork() 绝对不会拷贝子线程!!

	最后:
		父进程, 子线程, 共享CLONE_VM属性的子进程, 都通过pthread_atfork() 实现了上锁, 解锁操作, 
		为什么fork() 新建的子进程, 不需要在fork() 进行解锁?
		答:
			pthread_atfork() 定义了解锁操作, 也包括最新创建的子进程, 
			最新创建的子进程, 也是子进程, 虽然不共享'CLONE_VM内存空间', 但也是子进程;
			所以不用刻意进行解锁操作!!

			所以, 从fork() 一刻开始, 不共享'CLONE_VM内存空间'的子进程, 第一时间就会拷贝出一份新的mutex/rwlock/spin锁;
			如果访问了'mutex/rwlock/spin锁'构建的临界区, 访问什么资源, 就拷贝一份出来(全局变量, 静态变量也照样拷贝),
			这种情况下, 不共享'CLONE_VM内存空间'的父子进程, 是完全独立'CLONE_VM内存空间'的两个进程, 完全符合fork()语义;
*/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int G_count = 0;

//互斥锁直接赋值初始化, 省掉pthread_mutex_init()
pthread_mutex_t G_mutex;



void pth_atfork_prepare(void){
	if(pthread_mutex_lock(&G_mutex) != 0)
		perror("pth_atfork_prepare: pthread_mutex_lock()");
}

void pth_atfork_parent(void){
	if(pthread_mutex_unlock(&G_mutex) != 0)
		perror("pth_atfork_parent: pthread_mutex_unlock()");
}

void pth_atfork_child(void){
	if(pthread_mutex_unlock(&G_mutex) != 0)
		perror("pth_atfork_child: pthread_mutex_unlock()");
}

void* pth_atfork_thread_proc(void* arg){
	while(G_count < 2){
		pthread_mutex_lock(&G_mutex);
		G_count++;
		pthread_mutex_unlock(&G_mutex);
		printf("pth_atfork_thread_proc: G_count:%d\n",G_count);
		sleep(1);
	}
	return NULL;
}

//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg);

int main(void){
	pid_t pid;
	int son_ret;
	pthread_t thid;

	pthread_mutex_init(&G_mutex,0);


	if(pthread_create(&thid, NULL, pth_atfork_thread_proc, NULL) != 0){
		perror("pthread_create()");
		return -1;
	}

	if(pthread_atfork(pth_atfork_prepare, pth_atfork_parent, pth_atfork_child) != 0){
		perror("pthread_create()");
		//强制退出,CLONE_VM共享内存空间被销毁,内核会自动终结子线程,不用担心(简单demo代码,不做深入探讨)
		return -1;
	}

	sleep(1);
	printf("parent is about to fork ...\n");
	pid = fork();
	if(pid == -1){
		perror("fork()");
		return -1;
	}
	if(pid == 0){
		printf("child running\n");		//子进程
		while(G_count < 4){
			pthread_mutex_lock(&G_mutex);
			G_count++;
			pthread_mutex_unlock(&G_mutex);
			printf("child: G_count:%d\n",G_count);
			sleep(1);
		}
		exit(0);											//子进程结束
	}

	printf("parent continue\n");		//父进程
	sleep(5);

	if(pthread_join(thid,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}

	pid = wait(&son_ret);
	if(pid == -1)
		perror("wait()");
	else{
		printf("son %d exit(%d)\n", pid, son_ret);
		print_wait_stat(son_ret,NULL);
	}

	pthread_mutex_destroy(&G_mutex);
	return 0;
}





//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg){
	if(attach_msg != NULL)						//打印附带msg
		printf("%s\n", attach_msg);

	if(WIFEXITED(status)){						//子进程正常退出, 退出statue码=WEXITSTATUS(status)
		printf("child exited, status=%d\n", WEXITSTATUS(status));
		return;
	}

	if(WIFSIGNALED(status)){					//子进程被信号终止, 终止信号=WTERMSIG(status)
		printf("child killed by signal %d (%s)",WTERMSIG(status), strsignal(WTERMSIG(status)));
		#ifdef WCOREDUMP
		if(WCOREDUMP(status))						//产生了核心转存
			printf(" (core dumped)\n");
		#endif
		return;
	}

	if(WIFSTOPPED(status)){						//子进程被信号暂停, 暂停信号=WSTOPSIG(status)
		printf("child stopped by signal %d (%s)\n",WSTOPSIG(status), strsignal(WSTOPSIG(status)));
		return;
	}

	#ifdef WIFCONTINUED								//子进程已从暂停状态转为继续执行continue
	if(WIFCONTINUED(status)){
		printf("child continued\n");
		return;
	}
	#endif

	//未知错误(不应该发生)
	printf("print_wait_stat() failed, child statue=%d\n",status);
	return;
}
