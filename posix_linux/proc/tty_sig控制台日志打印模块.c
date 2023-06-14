//编译:
//		gcc ./tty_sig控制台日志打印模块.c -g3 -o x

//测试:
//		kill -s 28 <pid>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>



//选定的tty(配置daemon server 的时候自己选定, 需要root 权限执行)
#define PATH_TTY "/dev/tty2"
//每5 秒报告一次daemon server 的存活信息
#define alarm_wait_time (5)
//FILE* 文件流buf 缓冲区max
#define FS_BUF_LEN (4096)



//本模块不需要多进程共享, 禁止多进程共享(一个进程只需有一个即可)
static FILE* pfs;
static char fs_buf[FS_BUF_LEN];



//屏蔽29个信号[除外: 9, 19, SIGALRM=14定时器信号, SIGWINCH=28自定义信号(占用终端窗口尺寸发生变化信号)]
void ignore_sig32(void){
	int i = 1;
	for(;i <= 31;i++){
		if((i==9) || (i==19) || (i==14) || (i==28))
			continue;
		if(signal(i, SIG_IGN) == SIG_ERR){
			perror("signal()");
			break;
		}
	}
}

//信号处理函数(用户手动传来信号, 不能附带参数, 一般打印daemon server 的大部分运行状态信息)
void sig28_func(int sig){
	char buf[32];
	time_t t = time(NULL);
	fprintf(pfs,"signal: %d, %s\n",sig,ctime_r(&t,&buf));
	return;
}

//alarm循环闹钟函数(单位:秒)[定期打印到tty, 证明daemon server 仍然存活]
void alarm_func(int sig){
	char buf[32];
	time_t t = time(NULL);
	fprintf(pfs,"alarm signal: %d, daemon server is still alive: %s\n",sig,ctime_r(&t,&buf));

	//递归调用自身, 实现循环闹钟
	alarm(alarm_wait_time);
	return;
}

//初始化本模块(成功返回0, 失败返回-1)
int init_tty_sig(void){
	//屏蔽其他无关信号
	ignore_sig32();

	pfs = fopen(PATH_TTY,"w");
	if(!pfs){
		perror("fopen()");
		return -1;
	}
	setvbuf(pfs, fs_buf, _IOFBF, FS_BUF_LEN);

	if(signal(28, sig28_func) == SIG_ERR){
		perror("signal()");		//改写信号0 的处理方式(改成: 收到信号后, 执行信号处理函数)
		fclose(pfs);
		return -1;
	}

	if(signal(SIGALRM, alarm_func) == SIG_ERR){
		perror("signal()");		//改变SIGALRM 闹钟信号的处理方式
		fclose(pfs);
		return -1;
	}
	alarm(alarm_wait_time);	//启动第一个alarm(), 后续会自动递归alarm()
	return 0;
}

//释放本模块
//#define free_tty_sig(pfs) (fclose(pfs))
#define free_tty_sig() (fclose(pfs))



int main(void){
	if(init_tty_sig() == -1)
		return -1;

	printf("alarm begin: %d(pid=%d)\n",time(NULL),getpid());
	sleep(10);							//主进程陷入阻塞(但会被alarm()闹钟唤醒, 实际只会阻塞1 秒)
	sleep(10);							//测试循环alarm()
	sleep(10);
	printf("alarm end: %d\n",time(NULL));

	free_tty_sig();
	return 0;
}
