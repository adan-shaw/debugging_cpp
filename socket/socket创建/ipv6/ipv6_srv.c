//编译:
//		gcc -g3 ./ipv6_srv.c -o x_srv; gcc -g3 ./ipv6_cli.c -o x_cli



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>



#define BUF_MAX (1023)	//缓冲区长度
#define MYPORT (8888)		//服务器侦听端口
#define BACKLOG (10)		//服务器侦听长度



int main(void){
	int sfd_li, sfd_cli;
	struct sockaddr_in6 addr6_srv;
	struct sockaddr_in6 addr_cli;
	static char buf[BUF_MAX + 1];
	socklen_t len;



	//建立IPv6套接字
	sfd_li = socket(AF_INET6, SOCK_STREAM, 0);
	if(sfd_li == -1){
		perror("socket()");
		return(-1);
	}

	//填充服务器地址(in6addr_any = IPv6任意地址)
	//bzero(&addr6_srv, sizeof(addr6_srv));
	addr6_srv.sin6_family = AF_INET6;
	addr6_srv.sin6_port = htons(MYPORT);
	addr6_srv.sin6_addr = in6addr_any;

	//绑定
	if(bind(sfd_li,(struct sockaddr*)&addr6_srv, sizeof(struct sockaddr_in6)) == -1){
		perror("bind()");
		return(-1);
	}

	//设置侦听队列
	/*
	err = listen(sfd_li, BACKLOG);
	if(err == -1){
		perror("listen error");
		return(-1);
	}
	*/
	listen(sfd_li, BACKLOG);

	//循环等待客户端连接server
	while(-1){
		len = sizeof(struct sockaddr);
		//等待客户端连接
		sfd_cli = accept(sfd_li, (struct sockaddr*)&addr_cli, &len);
		if(sfd_cli == -1){
			perror("accept()");
			close(sfd_li);
			return(-1);
		}
		else{
			//将客户端的地址转换成字符串
			inet_ntop(AF_INET6, &addr_cli.sin6_addr, buf, sizeof(buf));
			printf("a client from ip: %s, port %d, socket %d\n", buf, addr_cli.sin6_port, sfd_cli);
		}

		//发消息给客户端
		//bzero(buf, BUF_MAX + 1);
		strncpy(buf,"From Server",BUF_MAX + 1);
		len = send(sfd_cli, buf, strlen(buf), 0);
		if(len < 0)
			perror("send()");
		else
			printf("message '%s' send success, %dbytes\n", buf, len);

		//接收客户端的消息
		bzero(buf, BUF_MAX + 1);
		len = recv(sfd_cli, buf, BUF_MAX + 1, 0);
		if(len >= 0)
			printf("recv message success:'%s',%dbytes\n", buf, len);
		else
			perror("recv()");

		//处理每个新连接上的数据收发结束
		close(sfd_cli);
	}

	close(sfd_li);
	return 0;
}
