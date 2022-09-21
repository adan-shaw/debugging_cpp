//编译:
//		gcc ./pipe序列化.c -g3 -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE_BUF_MAX (4096)

//结构体app_body{}
typedef struct app_body{
	int i1;
	double d2;
	char c3[64];//多余的字节位,继续写入; 虽然不能做到最小化拷贝,但胜在简单,数据对齐,拷贝速度快;
}body_t;



//无名管道测试
void pipe_test(void){
	int ppfd[2], tmp, pos;
	body_t buf;
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

		buf.i1 = 999;
		buf.d2 = 88.88;
		strncpy(buf.c3,"hello fifo",64);

		tmp = write(ppfd[1],&buf,sizeof(buf));	//阻塞写
		if(tmp == -1){
			perror("write()");
			return;
		}
		tmp = write(ppfd[1],&buf,sizeof(buf));	//阻塞写again
		if(tmp == -1){
			perror("write()");
			return;
		}

		close(ppfd[1]);					//关闭ppfd[1]写端
		printf("son written size = %d\n",tmp);
		exit(0);
	}
	else{											//父进程读
		usleep(1000);						//延时读
		close(ppfd[1]);					//关闭无名管道ppfd[1]写端

		tmp = read(ppfd[0],&buf,sizeof(buf));		//阻塞读
		if(tmp == -1){
			perror("read()");
			return;
		}
		printf("read %d size data\n", tmp);
		printf("struct body_t{} data: {%ld, %lf, %s}\n",buf.i1,buf.d2,buf.c3);

		tmp = read(ppfd[0],&buf,sizeof(buf));		//阻塞读again
		if(tmp == -1){
			perror("read()");
			return;
		}
		printf("read %d size data\n", tmp);
		printf("struct body_t{} data: {%ld, %lf, %s}\n",buf.i1,buf.d2,buf.c3);

		close(ppfd[0]);					//关闭ppfd[0]读端
	}
	return;
}

int main(void){
	pipe_test();
	return 0;
}

