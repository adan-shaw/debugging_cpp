//hello world
//编译:
//		gcc fork子父进程共享fd.c -g3 -o x

//验证fork() 只共享CLONE_VM内存区, 但共享io fd, 也就是共享socket
//此时, fork()子父进程共享'fd文件-pos游标'

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){
	int fd, pid;
	char *buf[128] = {0};
	fd = open("fork子父进程共享fd.c",O_RDONLY);
	if(fd == -1){
		perror("open()");
		return -1;
	}

	pid = fork();
	if(pid == -1){
		perror("fork()");
		return -1;
	}
	if(pid == 0){	//子进程
		read(fd,buf,5);
		printf("child:%s\n",buf);
	}
	else{					//父进程
		read(fd,buf,8);
		printf("father:%s\n",buf);
	}
	close(fd);
	return 0;
}

