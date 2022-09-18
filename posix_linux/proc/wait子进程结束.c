//编译:
//		gcc ./wait子进程结束.c -g3 -o x 


//子进程的结束情况分析:
/*
	若子进程先结束时:
		* 如果父进程没有调用wait()/waitpid()函数, 子进程就会进入僵死状态, 成为僵尸进程;
		* 如果父进程调用了wait()/waitpid()函数, 就不会使子进程变为僵尸进程;

	若父进程先结束时:
		子进程被init 进程收养, 直至子进程结束;
		如果子进程一直不结束, 则会无限占用系统资源;
		如果子进程被收养后结束, 则由init进程自动释放其僵尸状态;
		孤儿进程getppid()永远为1;

	僵尸进程的坏处:
		在每个进程退出时, 内核释放所有资源, 但扔然保留一定的信息, 如进程号, 退出信息, 运行时间等等;
		进程对SIGCHLD信号的默认处理方式为忽略, 这样子进程的保留的那部分信息可能就一直得不到释放;
		僵尸进程会保留进程号, 而我们的系统能够使用的进程号是有限的, 
		如果大量的僵尸进程存在, 就可能因为没有可用的进程号而导致系统不能产生新的进程;
		(僵尸进程只在数量多的情况下, 才会有问题)

	debian-64 系统的僵尸进程极限: 1.1W 左右
*/



//wait()和waitpid()的比较:
/*
	wait()和waitpid() 函数的区别:
		* wait()阻塞等待子进程结束, waitpid()可以不阻塞;
		* waitpid()可指定等待哪个子进程结束才返回, wait()一有子进程结束就马上返回, 不能区分是哪一个子进程;
		* 父进程没有子进程, 则wait()报错; 指定的子进程不存在, waitpid()报错;

	waitpid()提供了wait()没有的三个功能: 
		* waitpid()可等待一个特定的进程;
		* waitpid()提供了一个wait()的非阻塞版本(WNOHANG 可使父进程不陷入阻塞);
		* waitpid()支持作业控制;

	wait(&status) 等价于 waitpid(-1, &status, 0);


	函数原型:
		pid_t wait(int *wstatus);
			返回子进程pid;
			引用返回子进程退出status;
			失败返回-1(没有子进程才会报错);

		pid_t waitpid(pid_t pid, int *wstatus, int options);
			返回子进程pid;
			引用返回子进程退出status;
			失败返回-1(没有子进程才会报错);
			参数:
				pid = 指定pid;
				wstatus = 引用返回子进程status
				options:
					options = 0, 阻塞等待
					options = 1, 非阻塞等待(1=WNOHANG)
					options = 2, 返回被'暂停/挂起'的子进程的状态(2=WUNTRACED)
		注意: 
			waitpid() 不一定是等待子进程, 也可以是其它进程;
			如果目标pid指向的进程不存在, 则失败返回-1;
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg);



void test_wait(void){
	int son_ret;
	int tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){
		sleep(1);
		exit(99);
	}
	tmp = wait(&son_ret);
	if(tmp == -1)
		perror("wait()");
	else{
		printf("son %d exit(%d)\n", tmp, son_ret);
		print_wait_stat(son_ret,NULL);
	}

	tmp = wait(&son_ret);
	if(tmp == -1)
		perror("wait()");									//没有子进程了, 所以报错

	return;
}



void test_waitpid(void){
	int son_pid, son_ret;
	int tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){
		sleep(1);
		exit(99);
	}

	son_pid = tmp;											//保存子进程pid, 用作传入waitpid()

	tmp = waitpid(son_pid,&son_ret,1);	//非阻塞
	if(tmp == -1)
		perror("waitpid()");
	else
		printf("son %d exit(%d), waitpid()=%d\n", son_pid, son_ret, tmp);

	tmp = waitpid(son_pid,&son_ret,0);	//阻塞
	if(tmp == -1)
		perror("waitpid()");
	else{
		printf("son %d exit(%d), waitpid()=%d\n", son_pid, son_ret, tmp);
		print_wait_stat(son_ret,NULL);
	}

	if(waitpid(1,&son_ret,1) == -1)
		perror("waitpid()");							//非阻塞(等待init 进程结束)
	if(waitpid(9999,&son_ret,1) == -1)
		perror("waitpid()");							//非阻塞(等待9999 进程结束)

	return;
}



int main(void){
	printf("pid=%d\n",getpid());
	test_wait();
	test_waitpid();
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
