//创建一个tcp listener
int tcp_listener(const char* srv_ip, unsigned short srv_port, int sock_pending_que_max){
	int sfd, tmp;
	struct sockaddr_in addr;

	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	//修改套接字属性
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

	listen(sfd, sock_pending_que_max);
	return sfd;
}



//创建一个accept() 三次握手[sfd 属性拷贝from sfd_li](阻塞操作)
int tcp_accepter(int sfd_li){
	struct sockaddr addr;
	socklen_t addr_len = sizeof(struct sockaddr);
	//bzero(&addr, sizeof(struct sockaddr));
	int sfd = accept(sfd_li, &addr, &addr_len);
	if(sfd == -1){
		perror("accept()");
		return -1;
	}
	return sfd;
}



//创建一个tcp connecter(阻塞操作)
int tcp_connecter(const char* srv_ip, unsigned short srv_port){
	struct sockaddr_in addr;
	int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1){
		perror("connect()");
		return -1;
	}
	return sfd;
}



//接收/发送一次数据 -- 说明版(不能调用)
int tcp_ioEx(int sfd, void* pbuf, unsigned int len, int flags){
	int tmp = recv(sfd, pbuf, len, flags);
	int tmp = send(sfd, pbuf, len, flags);
	if(tmp >= 0)
		return tmp;					//阻塞同步操作: 大于0, 一定是发送/接受成功了
	//if(tmp == 0)
		//return tmp;				//阻塞同步操作: 等于0, 只有对端关闭这一种可能(errno==ENOTSOCK), 此时你也应该close(sfd);
												//不要乱用shutdown(), 只有当对端还在线的时候, 你才可以shutdown(), 而且shutdown() 操作是不可逆的

	//if(tmp == -1)				//上面两个if 排除之后, 此时的tmp返回值, 肯定是-1
	assert(tmp == -1);
	switch(errno){
		case EAGAIN:				//套接字已标记为非阻塞, 网卡正忙, 稍后再试

		case ECONNREFUSE:		//远程主机阻绝网络连接
		case ENOTCONN:			//与对端套接字尚未被连接上

		case EFAULT:				//内存空间访问出错
		case ENOMEM:				//内存不足
		case EINTR:					//操作被信号中断
		case EINVAL:				//参数无效

		case EBADF:					//sfd不是有效的描述符
		case ENOTSOCK:			//sfd不是套接字(当返回值是0时, 为正常关闭连接)
			perror("recv()");
			return -1;
		default:
			perror("recv() [unknow error]");
			return -1;
	}
	//NOTREACHED
}



//接收一次数据(阻塞io, 一旦阻塞则切换线程), 成功返回接收的数据size
int tcp_recv(int sfd, void* rbuf, unsigned int buf_len){
	int tmp = recv(sfd, *rbuf, buf_len, 0);
	if(tmp > 0)
		return tmp;					//接收成功(非接收成功, 就应该close(sfd))
	if(tmp == 0)					//对端已经关闭
		return -1;
	//此时, tmp == -1
	perror("recv()");
	return -1;
}

//发送一次数据(阻塞io, 一旦阻塞则切换线程), 成功返回发送的数据size
int tcp_send(int sfd, void* sbuf, unsigned int len){
	int tmp = send(sfd, sbuf, len, 0);
	if(tmp >= 0)
		return tmp;					//发送成功(非发送成功, 就应该close(sfd))
	if(tmp == 0)					//对端已经关闭
		return -1;
	//此时, tmp == -1
	perror("send()");
	return -1;
}



//
/*
	通知对方, 不要再发送/接收数据:
		int shutdown(int socket, int how);
		SHUT_RD: 断开输入流. 套接字无法接收数据(即使缓冲区收到数据也会被清除), 无法调用输入相关函数;
		SHUT_WD: 断开输出流. 套接字无法发送数据, 但如果输出缓冲区中还有未传输的数据, 则将传递到目标主机;
		SHUT_RDWR: 同时断开I/O流;

		shutdown() 与close() 的区别:
			close() 只是关闭一个进程/线程的fd, 如果其他进程/线程, 具备sfd 的dup() 拷贝, 则其它线程仍然可以使用socket 进行通信;
			在只有一个sfd 的情况下, close() 等于SHUT_RDWR, 
			在有多个sfd 的情况下, close() 跟shutdown() 区别很大;
		shutdown() 主要用在sfd 具有dup() 拷贝的情况下, 常见于fork() 多进程程序中;
		ps:
			udp 不能使用;

	关闭fd 句柄:
		int close(int fildes);
*/
