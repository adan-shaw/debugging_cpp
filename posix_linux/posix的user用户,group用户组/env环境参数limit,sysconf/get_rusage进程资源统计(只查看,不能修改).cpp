//编译:
//		g++ ./get_rusage进程资源统计\(只查看,不能修改\).cpp -g3 -o x 



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>



#include "adan_clib/debug_locating_errors定位错误.h"
using namespace adan_clib;



namespace adan_clib{

	//tty path的字符串缓冲区, 不会超出16 个字符串的
	#define TTY_PATH_BUF (16)

	// class类: 父进程资源监视
	class proc_resage_monitor{
		public:
			proc_resage_monitor(){
				//get tty/pts id(真实tty/虚拟tty, 都可使用: ttyname()获取路径字符串, fd=0表示获取自身tty 路径)
				//ps: ttyname()也可以获取其它tty 的路径(少用)
				if(ttyname_r(0, &this->str_tty_path, sizeof(this->str_tty_path)) != 0){
					MY_DEBUG("ttyname_r() failed");
					return NULL;
				}

				//打开fd_tty 文件句柄
				this->fd_tty = open(str_tty_path, O_WRONLY | O_NONBLOCK);
				if(this->fd_tty == -1){
					MY_DEBUG("open() failed");
					return NULL;
				}
			};
			~proc_resage_monitor(){};


		private:
			//将结构体变量弄成共享, 减少进出栈, 但不线程共享(反正只有父进程适合调用这个模块)
			struct rusage ru;
			//tty path 字符串缓冲区
			char str_tty_path[TTY_PATH_BUF];
			int fd_tty;
	};

}//namespace adan_clib ends


//打印rusage{} 结果
void printRusage(const char *leader, const struct rusage *ru);



int main(void){
	pid_t childPid;
	struct rusage ru;
	int tmp;



	printf("%s\n",ttyname(0));
	exit(0);
	childPid = fork();
	if(childPid == -1){
		MY_DEBUG("fork() failed");
		exit(EXIT_FAILURE);
	}
	if(childPid == 0){
		//子进程
		system("ls -l");
		system("ping -c 4 localhost");
		printf("\n\n");
		exit(EXIT_SUCCESS);
	}
	else{
		//父亲进程
		printf("son PID: %ld\n", (long) childPid);

		//等待子线程结束
		if(wait(NULL) == -1){
			MY_DEBUG("wait() failed");
			exit(EXIT_FAILURE);
		}

		if(getrusage(RUSAGE_CHILDREN, &ru) == -1){
			MY_DEBUG("getrusage() failed");
			exit(EXIT_FAILURE);
		}

		//打印rusage{} 结果
		printRusage("\t", &ru);
	}

	exit(EXIT_SUCCESS);
}



//打印rusage{} 结果
void printRusage(const char *leader, const struct rusage *ru){
	const char *ldr;



	ldr = (leader == NULL) ? "" : leader;

	printf("%sCPU time (secs):         user=%.3f; system=%.3f\n", ldr,\
					ru->ru_utime.tv_sec + ru->ru_utime.tv_usec / 1000000.0,\
					ru->ru_stime.tv_sec + ru->ru_stime.tv_usec / 1000000.0);
	printf("%sMax resident set size:   %ld\n", ldr, ru->ru_maxrss);
	printf("%sIntegral shared memory:  %ld\n", ldr, ru->ru_ixrss);
	printf("%sIntegral unshared data:  %ld\n", ldr, ru->ru_idrss);
	printf("%sIntegral unshared stack: %ld\n", ldr, ru->ru_isrss);
	printf("%sPage reclaims:           %ld\n", ldr, ru->ru_minflt);
	printf("%sPage faults:             %ld\n", ldr, ru->ru_majflt);
	printf("%sSwaps:                   %ld\n", ldr, ru->ru_nswap);
	printf("%sBlock I/Os:              input=%ld; output=%ld\n",\
					ldr, ru->ru_inblock, ru->ru_oublock);
	printf("%sSignals received:        %ld\n", ldr, ru->ru_nsignals);
	printf("%sIPC messages:            sent=%ld; received=%ld\n",\
					ldr, ru->ru_msgsnd, ru->ru_msgrcv);
	printf("%sContext switches:        voluntary=%ld; "\
					"involuntary=%ld\n", ldr, ru->ru_nvcsw, ru->ru_nivcsw);
	return;
}



//函数原型和介绍
/*
	函数原型:
		int getrusage(int who, struct rusage *usage);

	函数解析:
		返回'进程/子进程/线程'用掉的各类系统资源统计信息(只能获取, 不能更改), 如:
			用户开销时间, 系统开销时间, 接收的信号量等等;

		ps:
			哪个进程/线程调用, 就返回该调用着的统计信息, 无需指定pid;
			当调用成功后, 返回0, 否则-1;


	参数说明:
		who 参数:
			RUSAGE_SELF			返回'进程自身的资源'统计信息
			RUSAGE_CHILDREN	返回所有'子进程自身的资源'统计信息
			RUSAGE_THREAD		返回所有'子线程自身的资源'统计信息
		usage 参数:
			资源'统计信息的载体



	struct rusage{} 结构体, 定义在/usr/include/sys/resource.h
	struct rusage {
		struct timeval ru_utime;	// user time used 用户时间占用
		struct timeval ru_stime;	// system time used 系统时间占用
		long ru_maxrss;						// maximum resident set size 最大'驻留内存'大小
		long ru_ixrss;						// integral shared memory size 共享内存大小
		long ru_idrss;						// integral unshared data size 非共享数据大小(私有内存)
		long ru_isrss;						// integral unshared stack size 非共享堆栈大小(私有堆栈)
		long ru_minflt;						// page reclaims 页面回收
		long ru_majflt;						// page faults 页面错误
		long ru_nswap;						// swaps 交换
		long ru_inblock;					// block input operations 块输入操作
		long ru_oublock;					// block output operations 块输出操作
		long ru_msgsnd;						// messages sent 发送的消息
		long ru_msgrcv;						// messages received 收到的消息
		long ru_nsignals;					// signals received 接收到的信号
		long ru_nvcsw;						// voluntary context switches 自动上下文切换
		long ru_nivcsw;						// involuntary context switches 上下文切换
	};
*/
