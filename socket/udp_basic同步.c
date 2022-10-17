//创建一个tcp listener
int udp_binder(const char* srv_ip, unsigned short srv_port){
	int sfd, tmp;
	struct sockaddr_in addr;

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	//修改套接字属性
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));

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



//根据ip & port发送数据(阻塞io, 一旦阻塞则切换线程), 成功返回发送的数据size
int udp_send(int sfd, const char* ip, unsigned short port, const char* pbuf, int buf_len){
	int tmp;
	struct sockaddr_in addr;
	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	tmp = sendto(sfd, pbuf, buf_len, 0, (struct sockaddr*)&addr, sizeof(addr));
	if(tmp > 0)
		return tmp;	//发送成功(非发送成功, 就应该close(sfd))
	if(tmp == 0)	//对端已经关闭
		return -1;
	//此时, tmp == -1
	perror("sendto()");
	return -1;
}



//根据ip & port接收数据(阻塞io, 一旦阻塞则切换线程), 成功返回接收的数据size
int udp_recv(int sfd, const char* ip, unsigned short port, const char* pbuf, int buf_len){
	int tmp;
	struct sockaddr_in addr;
	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	tmp = recvfrom(sfd, *pbuf, buf_len, 0, (struct sockaddr*)&addr, sizeof(addr));
	if(tmp > 0)
		return tmp;	//接收成功(非接收成功, 就应该close(sfd))
	if(tmp == 0)	//对端已经关闭
		return -1;
	//此时, tmp == -1
	perror("recvfrom()");
	return -1;
}



//关闭socket
/*
	udp 不能用shutdown() 关闭文件流, 用了也不起作用的, 
	所以udp 基本上是直接关闭sfd, 当没有任何进程使用这个socket时, 释放sfd 资源;
*/
