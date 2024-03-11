#include "udp_basic.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>



//创建一个udp binder
int udp_binder(const char* srv_ip, unsigned short srv_port){
	int sfd, tmp;
	struct sockaddr_in addr;

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(srv_port);

	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1)
		perror("bind()");
		close(sfd);
		return -1;
	}

	return sfd;
}



//udp connect(): 成功返回0, 失败返回-1
int udp_connect(int sfd, const char* targer_ip, unsigned short targer_port){
	struct sockaddr_in addr_tmp;//服务端tmp addr info
	addr_tmp.sin_family=AF_INET;
	addr_tmp.sin_port = htons(targer_port);
	if(targer_ip == NULL)
		addr_tmp.sin_addr.s_addr = INADDR_ANY;
	else
		addr_tmp.sin_addr.s_addr = inet_addr(targer_ip);

	//绑定本地ip 地址
	if(bind(sfd, (struct sockaddr *)&addr_tmp, sizeof(struct sockaddr)) == -1){
		perror("bind()");
		return -1;
	}

	//connect() 绑定sfd + addr_tmp对方的地址信息
	if(connect(sfd, (struct sockaddr *)addr_tmp, sizeof(struct sockaddr)) == -1) {
		perror("connect()");
		return -1;
	}

	return 0;
}



//由于udp acceptEx() 异步, 太复杂了, 这里不做演示实现了, 需要用到的时候, 请自己重新写一个;



//udp accept(): 返回一个struct udp_accept_ret{} 结构体(里面有各种udp server 所需的数据)
struct udp_accept_ret {
	struct sockaddr_in addr_peer;	//返回值: 对端ip 信息
	unsigned char *pbuf;					//返回值(但需要先初始化buf 缓冲区): 第一次recvfrom() 的数据
	int data_len;									//返回值: 第一次recvfrom() 的数据len
	int pbuf_len;									//输入值: buf 缓冲区len
	int sfd_new;									//返回值: udp server 对接的sfd(相当于accept() 之后拷贝的sfd)
};//用结构体来表达, 更便捷明了(函数参数太多, 不好操作)

int udp_accept(int sfd_li, struct udp_accept_ret *p, unsigned short udp_srv_bind_port){
	int tmp;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	struct sockaddr_in addr_tmp;//服务端tmp addr info
	addr_tmp.sin_family=AF_INET;
	addr_tmp.sin_port = htons(udp_srv_bind_port);
	addr_tmp.sin_addr.s_addr = INADDR_ANY;

	//接收消息, 顺便获取对方的ip 地址信息(阻塞) (非阻塞, 请重写本函数)
	p->data_len = recvfrom(sfd_li, p->pbuf, p->pbuf_len, 0, (struct sockaddr *)&p->addr_peer, &cli_len);
	if(p->data_len == -1) {
		perror("recvfrom()");
		return -1;
	}

	//创建一个新的socket
	if((p->sfd_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(p->sfd_new, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(p->sfd_new, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	//绑定本地ip 地址
	if(bind(p->sfd_new, (struct sockaddr *)&addr_tmp, sizeof(struct sockaddr)) == -1){
		perror("bind()");
		close(p->sfd_new);
		return -1;
	}

	//connect() 绑定p->sfd_new + addr_peer对方的地址信息
	p->addr_peer.sin_family = AF_INET;
	if(connect(p->sfd_new, (struct sockaddr *)&p->addr_peer, sizeof(struct sockaddr)) == -1) {
		perror("connect()");
		close(p->sfd_new);
		return -1;
	}

	return p->sfd_new;
}

//用结构体来表达, 更便捷明了(函数参数太多, 不好操作)
int udp_accept_old(int sfd_li, unsigned short udp_srv_bind_port, struct sockaddr_in *addr_peer_ret, unsigned char *pbuf_ret, int *data_len_ret, int pbuf_len){
	int sfd_new, tmp;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	struct sockaddr_in addr_tmp;//服务端tmp addr info
	addr_tmp.sin_family=AF_INET;
	addr_tmp.sin_port = htons(udp_srv_bind_port);
	addr_tmp.sin_addr.s_addr = INADDR_ANY;

	//接收消息, 顺便获取对方的ip 地址信息(阻塞) (非阻塞, 请重写本函数)
	*data_len_ret = recvfrom(sfd_li, pbuf_ret, pbuf_len, 0, (struct sockaddr *)addr_peer_ret, &cli_len);
	if(data_len_ret == -1) {
		perror("recvfrom()");
		return -1;
	}

	//创建一个新的socket
	if((sfd_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd_new, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_new, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	//绑定本地ip 地址
	if(bind(sfd_new, (struct sockaddr *)&addr_tmp, sizeof(struct sockaddr)) == -1){
		perror("bind()");
		close(sfd_new);
		return -1;
	}

	//connect() 绑定sfd_new + addr_peer_ret对方的地址信息
	addr_peer_ret->sin_family = AF_INET;
	if(connect(sfd_new, (struct sockaddr *)addr_peer_ret, sizeof(struct sockaddr)) == -1) {
		perror("connect()");
		close(sfd_new);
		return -1;
	}

	return sfd_new;
}

