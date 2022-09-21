//编译:
//		gcc ./pipe简单.c -g3 -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE_BUF_MAX (4096)

//无名管道测试
void pipe_test(void){
	int ppfd[2], tmp, pos;
	char pipe_buf[PIPE_BUF_MAX];
	pid_t main_pid = getpid();

	if(pipe(ppfd) == -1){			//创建无名管道fd[2] 组
		perror("pipe()");
		return;
	}

	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){							//子进程写
		close(ppfd[0]);					//关闭无名管道ppfd[0]读端

		strncpy(pipe_buf,"hello pipe!!",sizeof(pipe_buf));
		tmp = write(ppfd[1],&pipe_buf,strlen(pipe_buf)+1);	//阻塞写
		if(tmp == -1){
			perror("write()");
			return;
		}
		tmp = write(ppfd[1],&pipe_buf,strlen(pipe_buf)+1);	//阻塞写again
		if(tmp == -1){
			perror("write()");
			return;
		}
		printf("son written size = %d\n",tmp);
		close(ppfd[1]);					//关闭ppfd[1]写端
		exit(0);
	}
	else{											//父进程读
		usleep(1000);						//延时读
		close(ppfd[1]);					//关闭无名管道ppfd[1]写端

		tmp = read(ppfd[0],&pipe_buf,sizeof(pipe_buf));		//阻塞读
		if(tmp == -1){
			perror("read()");
			return;
		}
		pos = strlen(pipe_buf)+1;
		printf("read size = %d, msg: %s, msg2: %s\n",tmp,pipe_buf,&pipe_buf[pos]);
		close(ppfd[0]);					//关闭ppfd[0]读端
	}
	return;
}

int main(void){
	pipe_test();
	return 0;
}



//pipe 简介
/*
	1.'pipe无名管道'是流式ipc 通信工具, 没有消息边界, 一般只用作传输字符串;
		但由于'pipe无名管道'在同一linux 本机运行, 没有'大小端数字存储'的区分,
		所以, 也可以传输数字变量, 结构体变量;

		因此, 'pipe无名管道'不需要json, xml等字符串序列化手段, 就可以传递数据.
		但不能传递指针, 传递指针极易造成指针访问越界(馊主意)
		[因为每个进程的虚拟地址都不一样, 传递指针只会无限增加程序复杂性, 且降低程序的可靠性;
		 吃力不讨好地在进程传递指针or 在不同进程间传递shm/mmap指针, 是一个馊主意!!]



	2.'pipe无名管道'的读写规则:
		'pipe无名管道'支持:
			* 先写后读(可阻塞可异步);
			* 先读后写(阻塞读, 等待写[异步写或阻塞写]);

		'pipe无名管道'先写入的数据, 不会丢失, 而是存在于'内核的'pipe无名管道'缓冲区'中, 
		如果'内核的'pipe无名管道'缓冲区'已满, 则write()会阻塞, 或者write()异步会写入失败, 返回-1;
		直至'内核的'pipe无名管道'缓冲区'空闲, 换置出足够的空闲空间, 能容纳write()写入的字节数时, 才能继续写入数据;
		(ps: 'pipe无名管道'绝对不会丢失数据)

		因此, 'pipe无名管道'其实跟tcp socket 是比较类似的, 
		都是流式ipc 通信工具, 没有消息边界, 但不会丢失数据;
		('pipe无名管道'内核buf默认max=1024*63 bit)


		阻塞读/写(推荐):
			int ppfd[2];//'pipe无名管道'int数组
			pipe(ppfd);	//'pipe无名管道'默认返回两个阻塞读写的fd

		异步读/写:
			通过fcntl()将fd设置成O_NONBLOCK, 即可开启异步读写.
			异步读/写失败, 且errno=EAGAIN, 表示pipe 正常, io读写稍后再试, 一般是:
				读: 暂时没有数据可读, 请稍后再试;
				写: 暂时没有空余空间可写, 请稍后再试;


		'pipe无名管道'的双向通信的实现:
			'pipe无名管道'是半双工的, 单个'pipe无名管道', 只能单向传输数据;
			想实现全双工, 则需要打开两个'pipe无名管道';



	3.'pipe无名管道'的其他细节:
		pipe作为linux中最基础的进程间通信机制, 经常在shell中使用, 例如:
		# 将ps程序的stdout FILE流, 通过无名管道'|'重定向到grep程序的stdin FILE流中, 做FILE流中介(反正不会溢出,不会丢数据)
		ps aux | grep aaa

		# FILE流中介, 可多级操作
		ps aux | grep aaa | grep a



	4.函数原型:
		int pipe(int pipefd[2]);							//默认'pipe无名管道'权限版(常用)
		int pipe2(int pipefd[2], int flags);	//带自定义'pipe无名管道'权限版
		成功返回0和两个无名管道fd(默认阻塞io读写), 失败返回-1;
*/
