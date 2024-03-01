//编译
//		gcc -g3 ./tcp_basic.c tcp_basic_io.c ./tcp_basic_cli.c -o x_cli; 



#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

//sfd 开启异步
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}

int tcp_recv(int sfd, char* rbuf, unsigned int buf_len);
int tcp_send(int sfd, const char* sbuf, unsigned int data_len);
int tcp_recvEx(int sfd, char* rbuf, int buf_len);
int tcp_sendEx(int sfd, const char* sbuf, int data_len);
int tcp_connecter(const char* srv_ip, unsigned short srv_port);


int main(void){
	char buf[128];
	int sfd;

	//子进程还在运行
	sfd = tcp_connecter("127.0.0.1", 6677);
	if(sfd < 0){
		printf("tcp_connecter() failed !!\n");
		return -1;
	}

	//同步io 测试
	tcp_send(sfd, "hello srv", sizeof("hello srv"));
	tcp_recv(sfd, buf, 128);
	printf("tcp_recv(): %s\n", buf);

	//异步io 测试
	__set_nonblocking(sfd);
	memset(&buf,0,128);
	tcp_sendEx(sfd, "hello srv again", sizeof("hello srv again"));
	tcp_recvEx(sfd, buf, 128);
	printf("tcp_recvEx(): %s\n", buf);

	close(sfd);
	return 0;
}

