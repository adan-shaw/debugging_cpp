//编译
//		gcc -g3 ./tcp_basic.c tcp_basic_io.c ./tcp_basic_srv.c -o x_srv; 



#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

//sfd 开启异步
#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK) == -1) perror("fcntl()");}

int tcp_recv(int sfd, char* rbuf, unsigned int buf_len);
int tcp_send(int sfd, const char* sbuf, unsigned int data_len);
int tcp_recvEx(int sfd, char* rbuf, unsigned int data_len);
int tcp_sendEx(int sfd, const char* sbuf, unsigned int data_len);
int tcp_accepter(int sfd_li, struct sockaddr *addr);
int tcp_listener(const char* srv_ip, unsigned short srv_port, int sock_pending_que_max);



int main(void){
	char buf[128];
	char ip_str[INET_ADDRSTRLEN];
	struct sockaddr addr;
	struct sockaddr_in addr_in;
	unsigned int len;
	int sfd, sfd_li = tcp_listener("127.0.0.1", 6677, 64);
	if(sfd_li < 0){
		printf("tcp_listener() failed !!\n");
		return -1;
	}
	else{
		//同步io 测试
		sfd = tcp_accepter(sfd_li,&addr);
		memcpy(&addr,&addr_in,sizeof(struct sockaddr));
		inet_ntop(AF_INET, &addr_in.sin_addr, ip_str, INET_ADDRSTRLEN);
		printf("server: tcp_accepter(): %s\n", ip_str);

		tcp_recv(sfd, buf, 128);
		printf("server: tcp_recv(): %s\n", buf);
		tcp_send(sfd, "hello client", sizeof("hello client"));

		//异步io 测试
		__set_nonblocking(sfd);
		memset(&buf,0,128);

		tcp_recvEx(sfd, (unsigned char*)&len, 4);
		len = ntohl(len);
		tcp_recvEx(sfd, buf, len);
		printf("server: tcp_recvEx(%d): %s\n", len, buf);

		len = htonl(sizeof("hello client again"));
		tcp_sendEx(sfd, (unsigned char*)&len, 4);
		tcp_sendEx(sfd, "hello client again", sizeof("hello client again"));
	}
	close(sfd);
	close(sfd_li);
	return 0;
}

