//编译:
//		gcc ./sig32异步信号.c -g3 -o x

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>



//屏蔽29个信号(9,19除外)
//[0是测试进程是否存活的信号,不能被signal()重置,signal()传入0参数会报错:Invalid argument]
void ignore_sig32(void){
	int i = 1;
	for(;i <= 31;i++){
		if((i==9) || (i==19))
			continue;
		if(signal(i, SIG_IGN) == SIG_ERR){
			perror("signal()");
			break;
		}
	}
}

//恢复29个信号
void reset_sig32(void){
	int i = 1;
	for(;i <= 31;i++){
		if((i==9) || (i==19))
			continue;
		if(signal(i, SIG_DFL) == SIG_ERR){
			perror("signal()");
			break;
		}
	}
}

//信号处理函数
void sig_func(int sig){
	printf("signal number: %d\n",sig);
	return;
}

#define alarm_wait_time (1)
//alarm循环闹钟函数(单位:秒)
void alarm_func(int sig){
	printf("alarm signal number: %d\n",sig);
	alarm(alarm_wait_time);	//递归调用自身, 实现循环闹钟
	return;
}


int main(void){
	ignore_sig32();
	reset_sig32();
	ignore_sig32();

	if(kill(getpid(),1)){		//给自己发信号(一般来说不需要if, 不会出错的)
		perror("kill()");
		return -1;
	}

	raise(1);								//给自己发信号(原子操作版)

	if(signal(1, sig_func) == SIG_ERR){
		perror("signal()");		//改写信号0 的处理方式(改成: 收到信号后, 执行信号处理函数)
		return -1;
	}

	raise(0);
	raise(1);

	if(signal(SIGALRM, alarm_func) == SIG_ERR){
		perror("signal()");		//改变SIGALRM 闹钟信号的处理方式
		return -1;
	}
	alarm(alarm_wait_time);	//启动第一个alarm(), 后续会自动递归alarm()

	printf("alarm begin: %d\n",time(NULL));

	raise(SIGALRM);					//手动触发一个闹钟

	for(;;)
	sleep(10);							//主进程陷入阻塞(但会被alarm()闹钟唤醒, 实际只会阻塞1 秒)
	printf("alarm end: %d\n",time(NULL));
	return 0;
}
