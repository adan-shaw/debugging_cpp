//编译:
//		gcc -g3 ./ipv6_srv.c -o x_srv; gcc -g3 ./ipv6_cli.c -o x_cli



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



#define BUF_MAX (1023)	//缓冲区长度
#define MYPORT (8888)		//服务器侦听端口
#define BACKLOG (10)		//服务器侦听长度



int main(void){
	int sfd, tmp;
	struct sockaddr_in6 server_addr;
	char buf[BUF_MAX + 1];



	//填充服务器地址(in6addr_loopback = IPv6回环地址)
	//bzero(&server_addr, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(MYPORT);
	server_addr.sin6_addr = in6addr_loopback;

	//建立IPv6套接字
	sfd = socket(AF_INET6, SOCK_STREAM, 0);
	if(sfd == -1){
		perror("socket()");
		return(-1);
	}

	//连接服务器
	if(connect(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
		perror("connect()");
		return(-1);
	}

	//接收服务器数据
	bzero(buf, BUF_MAX + 1);
	tmp = recv(sfd, buf, BUF_MAX + 1, 0);
	if(tmp >= 0)
		printf("RECVED %d bytes:%s\n", tmp, buf);
	else
		perror("recv()");

	//回发数据
	//bzero(buf, BUF_MAX + 1);
	strncpy(buf,"From Client", BUF_MAX + 1);
	tmp = send(sfd, buf, strlen(buf), 0);
	if(tmp >= 0)
		printf("SENDED %d bytes:%s\n", tmp, buf);
	else
		perror("send()");

	//关闭socket
	close(sfd);
	return 0;
}
