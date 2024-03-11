//编译:
//		gcc -g3 ./getsockname\&getpeername.c -o x 



//getsockname() 获取自己的sockaddr 信息 / getpeername() 获取对端的sockaddr 信息(TCP专用)
/*
简介:
	对于已建立链接的tcp socket 链接, 是可以随便反向获取'自身/对端'的sockaddr 信息的;
	对于已填写了sockaddr 的sfd, 是可以随便反向获取'自身'的sockaddr 信息的;

API:
	* 根据'本机的sfd', 获取该sfd自身的的'struct sockaddr_in 地址信息'(成功返回0, 出错返回-1)
	int getsockname(int sfd, struct sockaddr *localaddr, socklen_t *addrlen);

	* 根据'与本机sfd相连的对端'struct sockaddr_in 地址信息'(必须是已链接的tcp socket)(成功返回0, 出错返回-1)
	int getpeername(int sfd, struct sockaddr *peeraddr, socklen_t *addrlen);
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define srv_ip "127.0.0.1"
#define portEx (8989)

int srv(const char *bind_ip, unsigned short port){
	int tmp, sfd_li, sfd_cli;
	struct sockaddr_in addr_srv, addr_bind, addr_conn, addr_peer;
	int addr_bind_len = sizeof(struct sockaddr_in),
			addr_conn_len = sizeof(struct sockaddr_in),
			addr_peer_len = sizeof(struct sockaddr_in);
	char str_ip[INET_ADDRSTRLEN];



	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	//addr_srv.sin_addr.s_addr = htonl(INADDR_ANY);
	if(!inet_pton(AF_INET, bind_ip, &addr_srv.sin_addr)){
		perror("inet_pton()");
		return(-1);
	}

	sfd_li = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd_li == -1){
		perror("socket()");
		return(-1);
	}

	if(bind(sfd_li, (struct sockaddr*)&addr_srv, sizeof(addr_srv)) == -1){
		perror("bind()");
		close(sfd_li);
		return(-1);
	}

	tmp = 1;
	setsockopt(sfd_li, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_li, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	listen(sfd_li, 64);



	//接受client的链接请求
	sfd_cli = accept(sfd_li, (struct sockaddr *)&addr_bind, &addr_bind_len);
	if(sfd_cli == -1){
		perror("accept()");
		close(sfd_li);
		return(-1);
	}
	printf("srv: accept() 提取的客户端addr info = %s:%d\n", inet_ntoa(addr_bind.sin_addr),ntohs(addr_bind.sin_port));


	//getsockname() 获取本机sfd_li 的struct sockaddr_in 地址信息
	getsockname(sfd_li, (struct sockaddr *)&addr_bind, &addr_bind_len);
	printf("srv: getsockname() 提取监听sfd_li的addr info = %s:%d\n", inet_ntoa(addr_bind.sin_addr), ntohs(addr_bind.sin_port));

	//获取本机sfd_cli 的地址信息(服务者, 其实就是sfd_li 的listen() 监听端口)
	getsockname(sfd_cli, (struct sockaddr *)&addr_conn, &addr_conn_len);
	printf("srv: getsockname() 提取'服务者'sfd_cli的addr info = %s:%d\n", inet_ntoa(addr_conn.sin_addr), ntohs(addr_conn.sin_port));

	memset(str_ip,'\0',sizeof(str_ip));
	//获取与本机sfd_cli 相连的对端地址信息
	getpeername(sfd_cli, (struct sockaddr *)&addr_peer, &addr_peer_len);
	printf("srv: getpeername() 提取sfd_cli对端的addr info = %s:%d\n", inet_ntop(AF_INET, &addr_peer.sin_addr, str_ip, sizeof(str_ip)), ntohs(addr_peer.sin_port));

	close(sfd_cli);
	close(sfd_li);

	return(0);
}



int cli(const char *bind_ip, unsigned short port){
	int sfd_cli;
	struct sockaddr_in addr_srv;

	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	if(!inet_pton(AF_INET, bind_ip, &addr_srv.sin_addr)){
		perror("inet_pton()");
		return(-1);
	}

	sfd_cli = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd_cli == -1){
		perror("socket()");
		return(-1);
	}

	if(connect(sfd_cli, (struct sockaddr *)&addr_srv, sizeof(addr_srv)) == -1){
		perror("connect()");
		close(sfd_cli);
		return(-1);
	}

	close(sfd_cli);
	return(0);
}



int main(void){
	int pid_son = fork();
	if(pid_son == -1){
		perror("fork()");
		return -1;
	}

	if(pid_son == 0){
		if(srv(srv_ip, portEx) == -1)
			printf("子进程启动srv() 失败\n");
	}
	else{
		usleep(1000);

		if(cli(srv_ip, portEx) == -1)
			printf("父进程启动cli() 失败\n");
	}

	return 0;
}
