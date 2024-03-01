#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

//同步: 阻塞接收一次数据(一旦阻塞, 线程会被挂起), 成功返回接收的数据size, 失败返回-1
int tcp_recv(int sfd, char* rbuf, unsigned int buf_len){
	int tmp = recv(sfd, rbuf, buf_len, 0);
	if(tmp > 0)
		return tmp;
	if(tmp == 0){
		//if(buf_len == tmp)//buf_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
		if(buf_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 什么数据都没有接收到
	}
	//if(tmp < 0)//这句话可以省掉, 节省语句
	perror("recv()");
	return -1;
}

//同步: 阻塞发送一次数据(一旦阻塞, 线程会被挂起), 成功返回发送的数据size, 失败返回-1
int tcp_send(int sfd, const char* sbuf, unsigned int data_len){
	int tmp = send(sfd, sbuf, data_len, 0);
	if(tmp > 0)
		return tmp;
	if(tmp == 0){
		//if(data_len == tmp)//data_len == tmp == 0
		if(data_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 数据没有发出去
	}
	//if(tmp < 0)
	perror("send()");
	return -1;
}



//sfd 开启异步
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}



//异步: 接收一次数据(永远不会挂起线程), 成功返回接收的数据size, 失败返回-1
int tcp_recvEx(int sfd, char* rbuf, int buf_len){
	int tmp, count = buf_len;
	char* ptmp = rbuf;
	while(1){
		tmp = recv(sfd, ptmp, count, MSG_DONTWAIT);
		if(tmp > 0){
			count -= tmp;
			ptmp += tmp;			//指针位移
			if(count <= 0)
				return tmp;			//全部数据接收完毕
			else
				continue;				//数据未接收完毕, 继续接收
		}
		if(tmp == 0){
			//if(buf_len == tmp)//buf_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
			if(buf_len == 0)
				return tmp;
			else
				return -1;			//对端关闭, socket 什么数据都没有接收到
		}
		//if(tmp < 0)				//这句话可以省掉, 节省语句
		if(errno == EINTR)	//操作被信号中断, 则继续
			continue;
		if(errno == EAGAIN)	//网卡正忙, 继续尝试
			continue;
		//尝试失败, 接收错误
		perror("recv()");
		return -1;
	}
}

//异步: 发送一次数据(永远不会挂起线程), 成功返回发送的数据size, 失败返回-1
int tcp_sendEx(int sfd, const char* sbuf, int data_len){
	int tmp, count = data_len;
	const char* ptmp = sbuf;
	while(1){
		tmp = send(sfd, ptmp, count, MSG_DONTWAIT);
		if(tmp > 0){
			count -= tmp;
			ptmp += tmp;			//指针位移
			if(count <= 0)
				return tmp;			//全部数据发生完毕
			else
				continue;				//数据未发生完毕, 继续发生
		}
		if(tmp == 0){
			//if(data_len == tmp)//data_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
			if(data_len == 0)
				return tmp;
			else
				return -1;			//对端关闭, socket 什么数据都没有接收到
		}
		//if(tmp < 0)				//这句话可以省掉, 节省语句
		if(errno == EINTR)	//操作被信号中断, 则继续
			continue;
		if(errno == EAGAIN)	//网卡正忙, 继续尝试
			continue;
		//尝试失败, 接收错误
		perror("send()");
		return -1;
	}
}





//1.io 操作错误时, 返回的errno 值的意思
/*
	case EAGAIN:				//套接字已标记为非阻塞, 网卡正忙, 稍后再试

	case ECONNREFUSE:		//远程主机阻绝网络连接
	case ENOTCONN:			//与对端套接字尚未被连接上

	case EFAULT:				//内存空间访问出错
	case ENOMEM:				//内存不足
	case EINTR:					//操作被信号中断
	case EINVAL:				//参数无效

	case EBADF:					//sfd不是有效的描述符
	case ENOTSOCK:			//sfd不是套接字(当返回值是0时, 为正常关闭连接)
*/



//2.tcp io缓冲区特性
/*
	1.I/O缓冲区在每个TCP套接字中单独存在;
	2.I/O缓冲区在创建套接字时自动生成;
	3.即使关闭套接字也会'继续传送输出缓冲区'中遗留的数据;
	4.关闭套接字将'丢失输入缓冲区'中的数据.

	输入输出缓冲区的默认大小一般都是8K,可以通过 getsockopt()函数获取.
	如果设置了SO_DONTLINGER, socket 关闭, 就不会再发送数据了, 缓冲区也会随之释放(当然关闭时, 没有io 任务, socket 也会立即释放);
*/



//3.io 零拷贝技术
/*
	socket 的io 零拷贝技术, 不只是sendfile() 一个, 
	writev()/sendmmsg(), mmap() 等, 都可以实现零拷贝技术, 跟sendfile() 一样;
*/



//4.socket 异步模式
/*
	异步模式下, errno=EAGAIN, 表示网卡设备忙(ETH 以太网载波侦听的物理特性, 再次展现);
	这时候, 再次尝试socket io可能会成功, 网卡可能会空闲, 
	因此如果socket io 操作返回errno=EAGAIN, 则可以再试;
*/



//5.tdp/udp 的最简单报头设计
/*
* tcp 报头 + len
* udp 报头 + count
*/



//6.tcp/udp 发送大数据包的区别
/*
	如果write()写入的数据大于socket缓冲区,则:
	TCP:
		1,阻塞fd: 超过MTU数据会自动分片, 阻塞直到发送完毕(无最大长度限制);
		2,非阻塞fd: 超过MTU数据会自动分片, 将会返回-1, errno=EAGAIN, 设备自动表示正忙, 正在占用;

	UDP:
		1,阻塞fd: 如果超过UDP最大包限制将会出错返回-1;
		2,非阻塞fd: 如果网卡忙, 返回-1, errno=EAGAIN;

	另外:
		由于socket 的读写缓冲区是分立的, 且默认一个socket 同一时间, 只能有一个io 操作在进行,
		所以, 理论上可以'默认单次最大'写入8kb 数据,
		(但如果写入时, 网卡未空闲, 上一次数据未发出去, 下一波数据又来了, 这样的操作越多, '资源竞争冲突'会越严重!!
		 虽然网卡io 的速度是很快的, 很快可以将数据发出去, 你几乎感觉不出来区别;
		 但还是要极力避免的'资源竞争冲突', 提高网络程序的并发性能)
*/



