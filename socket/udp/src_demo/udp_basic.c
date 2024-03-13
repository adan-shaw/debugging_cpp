#include "udp_basic.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>



//创建一个udp sfd, 必然执行bind(ip+port), 选择性执行connect: 
/*
	do_connect:
		1     = 执行connect()
		0     = 不执行connect()
		其他值 = 不执行connect()
	一般情况下:
		服务器不执行connect(), 只创建sfd + bind()
		客户端执行connect(), 创建sfd + bind() + connect()
	return:
		成功返回sfd, 失败返回-1
*/
int udp_binder_connect(const char* srv_ip, unsigned short srv_port, unsigned short do_connect){
	struct sockaddr_in addr;
	int tmp, sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(srv_port);
	if(srv_ip == NULL)
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		addr.sin_addr.s_addr = inet_addr(srv_ip);

	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) == -1)
		perror("bind()");
		close(sfd);
		return -1;
	}

	if(do_connect == 1)
		if(connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
			perror("connect()");
			close(sfd);
			return -1;
		}

	return sfd;
}

//直接令sfd 执行dup(sfd) + connect(addr_peer)
/*
	* 输入sfd_bind == server 执行bind() 的sfd

	* 输入addr_peer == server 执行第一次recvfrom()时, 返回的struct sockaddr_in *addr_peer;
		输入addr_peer == NULL(常用), 则使用addr_peer 填充地址信息;
		不输入addr_peer != NULL, 则使用srv_ip + srv_port 填充地址信息(效率低, 少用)
		两种方法2选1 !!

	* return:
			成功返回sfd, 失败返回-1
*/
int udp_accept(int sfd_bind, struct sockaddr_in *addr_peer, const char* srv_ip, unsigned short srv_port){
	struct sockaddr_in addr;
	int sfd = dup(sfd_bind);

	if(addr_peer == NULL){//不输入addr_peer, 则自动填充
		//bzero(&addr, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(srv_port);
		if(srv_ip == NULL)
			addr.sin_addr.s_addr = INADDR_ANY;
		else
			addr.sin_addr.s_addr = inet_addr(srv_ip);
		addr_peer = addr;
	}

	if(connect(sfd, (struct sockaddr *)addr_peer, sizeof(struct sockaddr_in)) == -1) {
		perror("connect()");
		close(sfd);
		return -1;
	}

	return sfd;
}



//由于udp acceptEx() 异步, 太复杂了, 这里不做演示实现了, 需要用到的时候, 请自己重新写一个;




