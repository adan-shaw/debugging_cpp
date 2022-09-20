//编译:
//		gcc ./pipe简单.c -g3 -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE_BUF (4096)

//管道测试
void pipe_test(void){
	int ppfd[2], tmp;
	char pipe_buf[PIPE_BUF], *pr_buf, *pw_buf;
	pid_t main_pid = getpid();

	if(pipe(ppfd) == -1){			//创建管道fd[2] 组
		perror("pipe()");
		return;
	}

	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){							//子进程写
		close(ppfd[0]);					//关闭管道ppfd[0]读端

		strncpy(pipe_buf,"hello pipe!!",sizeof(pipe_buf));
		tmp = write(ppfd[1],pipe_buf,strlen(pipe_buf)+1);	//阻塞写
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
		close(ppfd[1]);					//关闭管道ppfd[1]写端

		tmp = read(ppfd[0],pipe_buf,sizeof(pipe_buf));		//阻塞读
		if(tmp == -1){
			perror("read()");
			return;
		}
		printf("read size = %d, msg: %s\n",tmp,pipe_buf);
		close(ppfd[0]);					//关闭ppfd[0]读端
	}
	return;
}

int main(void){
	pipe_test();
	return 0;
}

