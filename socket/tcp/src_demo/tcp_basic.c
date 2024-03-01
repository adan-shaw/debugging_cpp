#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>



//创建一个tcp listener
int tcp_listener(const char* srv_ip, unsigned short srv_port, int sock_pending_que_max){
	struct sockaddr_in addr;
	struct linger m_linger = {0,0};
	//m_linger.l_onoff=0;
	//m_linger.l_linger=0;
	int tmp = 0, sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	//修改套接字属性(任何accept() socket 都会拷贝listener 的socket 属性, 因此该这个, server 所有socket 都会改变)
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));		//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));		//重用端口

	setsockopt(sfd, SOL_SOCKET, SO_LINGER, &m_linger, sizeof(int));	//close()后强制关闭, 不经历TIME_WAIT的过程

	tmp = 2000;//2 秒超时
	setsockopt(sfd, SOL_SOCKET, SO_SNDTIMEO, &tmp, sizeof(int));		//发送时限
	setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &tmp, sizeof(int));		//接收时限
	tmp = 32*1024;//设置为32K
	setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &tmp, sizeof(int));			//接收缓冲区
	setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &tmp, sizeof(int));			//发送缓冲区
	//tmp = 1;
	//setsockopt(sfd, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, &tmp, sizeof(int));//非阻塞模式下, connect()的过程中可以设置connect()延时, 直到accpet()被呼叫;(win 特产?)

	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(srv_port);

	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1){
		perror("bind()");
		close(sfd);
		return -1;
	}

	listen(sfd, sock_pending_que_max);
	return sfd;
}



//创建一个accept() 三次握手[sfd 属性拷贝from sfd_li](阻塞操作)
int tcp_accepter(int sfd_li, struct sockaddr *addr){
	socklen_t addr_len = sizeof(struct sockaddr);
	int sfd = accept(sfd_li, addr, &addr_len);
	if(sfd == -1){
		perror("accept()");
		return -1;
	}
	return sfd;
}



//创建一个tcp connecter(阻塞操作)
int tcp_connecter(const char* srv_ip, unsigned short srv_port){
	struct sockaddr_in addr;
	struct linger m_linger = {0,0};
	//m_linger.l_onoff=0;
	//m_linger.l_linger=0;
	int tmp, sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	//修改套接字属性(client only)
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));		//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));		//重用端口

	setsockopt(sfd, SOL_SOCKET, SO_LINGER, &m_linger, sizeof(int));	//close()后强制关闭, 不经历TIME_WAIT的过程

	tmp = 2000;//2 秒超时
	setsockopt(sfd, SOL_SOCKET, SO_SNDTIMEO, &tmp, sizeof(int));		//发送时限
	setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &tmp, sizeof(int));		//接收时限
	tmp = 32*1024;//设置为32K
	setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &tmp, sizeof(int));			//接收缓冲区
	setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &tmp, sizeof(int));			//发送缓冲区

	//bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(srv_port);

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) == -1){
		perror("connect()");
		close(sfd);
		return -1;
	}
	return sfd;
}



//1.socket 的关闭方式
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




