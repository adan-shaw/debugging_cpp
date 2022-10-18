//编译:
//		gcc -g3 ./getsockname获取自己\&getpeername获取对端\[tcp\].c -o x 



//getsockname() / getpeername() 简介
/*
	TCP 专用
	根据'本机的sfd', 获取该sfd自身的的'struct sockaddr_in 地址信息';
	int getsockname(int sfd, struct sockaddr *localaddr, socklen_t *addrlen);
	成功返回0, 出错返回-1;

	根据'与本机sfd相连的对端'struct sockaddr_in 地址信息'(必须已经链接);
	int getpeername(int sfd, struct sockaddr *peeraddr, socklen_t *addrlen);
	成功返回0, 出错返回-1;
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>



int srv(char *bind_ip, unsigned short port){
	int tmp, sfd_bind, sfd_conn;
	struct sockaddr_in addr_srv, addr_bind, addr_conn, addr_peer;
	int addr_bind_len = sizeof(struct sockaddr_in),
			addr_conn_len = sizeof(struct sockaddr_in),
			addr_peer_len = sizeof(struct sockaddr_in);
	char str_ip[INET_ADDRSTRLEN];



	sfd_bind = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd_bind == -1){
		perror("socket()");
		return(0);
	}

	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	//addr_srv.sin_addr.s_addr = htonl(INADDR_ANY);
	if(!inet_pton(AF_INET, bind_ip, &addr_srv.sin_addr)){
		perror("inet_pton()");
		close(sfd_bind);
		return(0);
	}

	if(bind(sfd_bind, (struct sockaddr*)&addr_srv, sizeof(addr_srv)) == -1){
		perror("bind()");
		close(sfd_bind);
		return(0);
	}

	tmp = 1;
	setsockopt(sfd_bind, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_bind, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	listen(sfd_bind, 64);



	//接受client的链接请求
	sfd_conn = accept(sfd_bind, (struct sockaddr *)&addr_bind, &addr_bind_len);
	if(sfd_conn == -1){
		perror("accept()");
		close(sfd_bind);
		return(0);
	}
	printf("srv: accept() 提取的客户端addr info = %s:%d\n",
			inet_ntoa(addr_bind.sin_addr),ntohs(addr_bind.sin_port));


	//getsockname() 获取本机sfd_bind 的struct sockaddr_in 地址信息
	getsockname(sfd_bind, (struct sockaddr *)&addr_bind, &addr_bind_len);
	printf("srv: getsockname() 提取监听sfd_bind的addr info = %s:%d\n",
			inet_ntoa(addr_bind.sin_addr), ntohs(addr_bind.sin_port));


	//获取本机sfd_conn 的地址信息(服务者, 其实就是sfd_bind 的listen() 监听端口)
	getsockname(sfd_conn, (struct sockaddr *)&addr_conn, &addr_conn_len);
	printf("srv: getsockname() 提取'服务者'sfd_conn的addr info = %s:%d\n",
			inet_ntoa(addr_conn.sin_addr), ntohs(addr_conn.sin_port));


	memset(str_ip,'\0',sizeof(str_ip));
	//获取与本机sfd_conn 相连的对端地址信息
	getpeername(sfd_conn, (struct sockaddr *)&addr_peer, &addr_peer_len);
	printf("srv: getpeername() 提取sfd_conn对端的addr info = %s:%d\n",
			inet_ntop(AF_INET, &addr_peer.sin_addr, str_ip, sizeof(str_ip)), ntohs(addr_peer.sin_port));



	close(sfd_conn);
	close(sfd_bind);

	return(1);
}



int cli(char *bind_ip, unsigned short port){
	int sfd_cli;
	struct sockaddr_in addr_srv;

	sfd_cli = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd_cli == -1){
		perror("socket()");
		return(0);
	}

	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	if(!inet_pton(AF_INET, bind_ip, &addr_srv.sin_addr)){
		perror("inet_pton()");
		return(0);
	}

	if(connect(sfd_cli, (struct sockaddr *)&addr_srv, sizeof(addr_srv)) == -1){
		perror("connect()");
		return(0);
	}

	usleep(2000);

	close(sfd_cli);
	return(1);
}



int main(void){
	int pid_son = fork();
	if(pid_son == -1){
		perror("fork()");
		return -1;
	}

	if(pid_son == 0){
		if(!srv("127.0.0.1", 8868))
			printf("子进程启动srv() 失败\n");
		exit(EXIT_SUCCESS);
	}

	usleep(2000);

	if(!cli("127.0.0.1", 8868))
		printf("父进程启动cli() 失败\n");

	return 0;
}
