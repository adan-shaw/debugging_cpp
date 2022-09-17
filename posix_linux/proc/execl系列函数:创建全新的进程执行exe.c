//编译:
//		gcc ./execl系列函数\:创建全新的进程执行exe.c -g3 -o x



//execl 系列函数[ 没有exec(), 只有execl() ]:
/*
	//字符串常量版
	int execl(const char *path, const char *arg, ...);		//需要指定exe文件的路径, 如exe有参数, 也需要指明
	int execlp(const char *file, const char *arg, ...);		//指定exe文件名, 带exe参数执行(系统自己在/bin, /usr/local/bin查找exe)
	int execle(const char *path, const char *arg, ...);		//execl()的基础上, 可多指定env环境变量

	//字符串缓冲区版(少用, 和上面的一样)
	int execv(const char *path, char *const argv[]);
	int execvp(const char *file, char *const argv[]);
	int execvpe(const char *file, char *const argv[], char *const envp[]);

	execl() 函数的功能:
		execl() 是用'新exe中的数据段'替换了'原来进程中的代码段、数据段以及堆和栈'中的数据;
		execl() 并不是创建一个全新的进程, getpid(), getppid() 都不变;

		只有fork() + execl() 联用的时候, 才会是全新的进程, 全新的数据段, 完全全新的进程;
		但父进程getppid() 仍然是原来的进程, 因为fork() 的时候, 父进程继承为原来进程的pid;
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>



//本函数会导致进程陷入死循环, 递归调用, 不断创建新的进程, 不断退出进程, 子进程不停被init() 进程收养;
//停止办法: 直接删掉./x 文件, 找不到./x 执行文件, 报错终止;
void test_execl_pid(void){
	printf("getppid()=%d,getpid()=%d\n",getppid(),getpid());
	if(fork() == 0){		//子进程执行execl()
		if(execl("./x", (char *) NULL) == -1)
			perror("execle()");
		exit(EXIT_SUCCESS);
	}
	return;
}

void test_execl(void){
	if(fork() == 0){		//fork() + execl(), 创建全新子进程的标准做法
		if(execl("/bin/ls","-a","-l","./x",(char*)NULL) == -1)
			perror("execle()");
		exit(EXIT_SUCCESS);
	}
	return;
}

void test_execlp(void){
	if(fork() == 0){		//自己找/bin, /usr/bin, /usr/local/bin 版本
		if(execlp("ls","-a","-l","./x",(char*)NULL) == -1)
			perror("execle()");
		exit(EXIT_SUCCESS);
	}
	return;
}

void test_execle(void){
	//char *env[] = { "ADAN=adan", "SHAW=shaw", NULL };
	char *env[] = { "PATH=/tmp", "USER=adan", NULL };
	if(fork() == 0){		//带环境变量版本(慎用, 有点小问题, 未解决)
		if(execle("/bin/ls","-a","-l","./x",(char*)NULL, env) == -1)//不知道为何卡死
			perror("execle()");
		exit(EXIT_SUCCESS);
	}
	return;
}

int main(void){
	printf("getppid()=%d,getpid()=%d\n",getppid(),getpid());
	test_execl();
	test_execlp();
	test_execle();

	test_execl_pid();
	return 0;
}
