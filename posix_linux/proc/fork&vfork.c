//编译:
//		gcc ./fork\&vfork.c -g3 -o x 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//vfork()的特点:
/*
	vfork()子进程必定先运行, 等到子进程调用exit()/execl()后, 父进程才能继续运行;
	vfork()其它特性, 跟fork()一样, 子进程与父进程共享数据(写时复制,读时共享)

	fork() 创建子进程, 父子进程, 谁先运行是随机的;
*/



//快速创建子进程(不管对错, 默认为一定成功)
void quick_vfork(void){
	if(vfork() == 0){	//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	return;
}

void normally_vfork(void){
	int tmp;
	tmp=vfork();
	if(tmp == -1){
		perror("vfork()");
		return;
	}
	if(tmp == 0){			//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	return;
}



//快速创建子进程(不管对错, 默认为一定成功)
void quick_fork(void){
	if(fork() == 0){	//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	sleep(1);					//等待子进程结束, 再继续执行
	return;
}

void normally_fork(void){
	int tmp;
	tmp=fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){			//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	sleep(1);					//等待子进程结束, 再继续执行
	return;
}



int main(void){
	quick_vfork();
	normally_vfork();
	quick_fork();
	normally_fork();
	return 0;
}
