//编译:
//		gcc ./sig64实时信号.c -g3 -o x

#include <unistd.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>



//屏蔽64个信号(9,19除外)
void ignore_all_signal(void){
	sigset_t sig_set;
	sigfillset(&sig_set);
	sigdelset(&sig_set,9);
	sigdelset(&sig_set,19);
	//if(sigismember(&sig_set,0) == 0)//信号存在, 返回1; 信号不存在, 返回0
	//	printf("sigismember(): 0信号不在sigset_t 信号集中\n");
	if(sigprocmask(SIG_BLOCK,&sig_set,NULL) == -1)
		perror("sigprocmask()");
}

//恢复64个信号(9,19除外)
void reset_all_signal(void){
	sigset_t sig_set;
	sigfillset(&sig_set);
	sigdelset(&sig_set,9);
	sigdelset(&sig_set,19);
	if(sigprocmask(SIG_UNBLOCK,&sig_set,NULL) == -1)
		perror("sigprocmask()");
}



//参数信号处理函数(指针): 同一个进程内有效
void handler_point(int sig,siginfo_t *info,void *myact){
	int i;
	for(i=0;i<10;i++)
		printf("%c-",(*( (char*)((*info).si_ptr)+i)));
	printf("handle signal %d over;\n",sig);
	return;
}

//参数信号处理函数(整形)
void handler_int(int sig,siginfo_t *info,void *myact){
	printf("sig=%d, siginfo_t* info->si_int=%d, void *myact=%d\n",sig, info->si_int, myact);
	return;
}


//实时信号测试(占用64信号, 不能是已知的1-31, 否则会注册失败)
void sigaction_test(void){
	struct sigaction sig_act;
	union sigval sig_val;
	sigset_t sig_set;
	int tmp;


	sigfillset(&sig_set);
	sigdelset(&sig_set,9);
	sigdelset(&sig_set,19);
	sigdelset(&sig_set,64);
	if(sigismember(&sig_set,64) == 1)
		printf("sigismember(): sig-64 okay\n");

	//sig_act.sa_sigaction = handler_point;		//参数信号处理函数(指针)
	sig_act.sa_sigaction = handler_int;				//参数信号处理函数(整形)
	sig_act.sa_flags=SA_SIGINFO|SA_RESTART;
	//sig_act.sa_flags=SA_SIGINFO|SA_RESTART|SA_NODEFER;
	sig_act.sa_mask = sig_set;

	//激活信号
	if(sigaction(64, &sig_act, NULL) == -1){
		perror("sigaction()");
		return;
	}


	//4.常规发送和接收信号操作.
	tmp=fork();
	if(tmp==-1){
		perror("fork()");
		return;
	}
	if(tmp==0){//子进程, 让父进程先跑, 方便完成测试
		usleep(500);

		//信号附加值赋值
		//sig_val.sival_ptr = data;							//指针--只能在本进程内有效
		sig_val.sival_int = 12345678;						//整形

		//发送一个信号给父进程
		sigqueue(getppid(), 64, sig_val);

		exit(EXIT_SUCCESS);
	}
	else{
		sigsuspend(&sig_set);										//挂起等待一个有效信号到来(阻塞)
	}
	return;
}



int main(void){
	printf("main pthread pid = %d\n",getpid());
	ignore_all_signal();
	reset_all_signal();
	sigaction_test();
	setitimer_test();
	return 0;
}




//实时信号定时器测试
void setitimer_test(void){
	struct sigaction act;
	sigset_t sig_set;
	struct itimerval tick;
	int test_count;

	tick.it_value.tv_sec = 1;									//启动延时(单位:秒)
	tick.it_value.tv_usec = 0;								//(单位:微秒,6位数=1秒)
	tick.it_interval.tv_sec = 1;							//触发间隔
	tick.it_interval.tv_usec = 0;

	sigfillset(&sig_set);
	sigdelset(&sig_set,9);
	sigdelset(&sig_set,19);
	sigdelset(&sig_set,SIGALRM);

	//act.sa_sigaction = handler_point;				//参数信号处理函数(指针)
	act.sa_sigaction = handler_int;						//参数信号处理函数(整形)
	act.sa_flags = ITIMER_REAL;								//实时定时器类型
	act.sa_mask = sig_set;

	if(sigaction(SIGALRM,&act,NULL) == -1){		//激活信号
		perror("sigaction()");
		return;
	}

	//设置并激活实时定时器
	if(setitimer(ITIMER_REAL,&tick,NULL) == -1){
		perror("setitimer()");
		return;
	}

	//等待定时信号到来for test
	test_count = 6;														//等待6 个信号即退出
	while(test_count > 0){
		sigsuspend(&sig_set);
		test_count--;
	}
	return ;
}


