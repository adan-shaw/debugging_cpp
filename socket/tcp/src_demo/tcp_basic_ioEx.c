#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>



//sfd 开启异步
#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK) == -1) perror("fcntl()");}



//异步拓展info
/*
	为了保证代码的简洁性, 这里不做字符串拼接, 只做原理展示;

	由于tcp 是可以保证数据的顺序性的, 
	因此, 序列化过程中, 最简单的就是分两次发送/接收: 
		一次发送len, 一次发送data, 这样最省事; 
		一次接收len, 一次接收data, 接收也是分两次;

	ps:
		两次io 操作的消耗, 一般情况下, 会比字符串拼接消耗大!!
		所以, 最优解决办法, 还是直接使用字符串拼接, 将数据组装成一个buf, 统一发送!!
		这样做性能最好(推荐使用c++, c 语言字符串操作十分困难, 悲剧)


	关于缓冲区, 做成static 静态变量模式, 实现单一自增长, 多进程/多线程, 有不同做法:
		* 多进程程序, 不用理会, 因为会写时复制; 
		* 多线程程序, 应该将这两个关键静态变量, 写入线程属性中, 做线程全局变量;

	关于切换select, epoll 等io 事件通知模式, 不同的做法有:
		* 多线程模型下, 可以在线程属性中, 定义一个线程全局变量为fd_set, 这时候再启动线程, 这时候fd_set 就是全局共享的, 可以更方便;
			多进程模型下, 不用理会, 写时复制, 自动屏蔽差异;

		* 如果切换为epoll, 同理, 可以用epoll 把select 模块进行重写(不推荐!!)
			epoll 的跨平台和实时性没有select 好,
			epoll 适合多线程共享, select 实时性高, 单不适合多线程共享;

	关于异步io 卡死的问题:
		tcp 异步收发函数比较复杂, data_len, buf_len, 长度控制不好, 很容易陷入逻辑死锁!!
		(一旦陷入死锁, 是程序员自身的水平问题, 不是tcp/ip 协议栈 or posix API 的问题)
		如果需要制定数据帧, 规模比较大, 不推荐;
		导致卡死的主要原因:
			recv() 不知道自己改接收多少字节数据, 而你有输入了比较大的接收数值, recv() 就会一直等待, 等待收够了数据才返回;
			(异步socket 也会等到收够了数据, 才会返回!! 
			 所以, 编码时, 应该接收多少数据, 必须输入一个准确值, 否则异步tcp socket 也是会死锁的)

	tdp/udp 的最简单报头设计:
		tcp 报头 + len
		udp 报头 + count
*/



//异步: 接收一次数据(永远不会挂起线程), 成功返回接收的数据size, 失败返回-1
int tcp_recvEx(int sfd, char* rbuf, unsigned int data_len){
	int tmp;
	unsigned int count = 0;
	while(1){
		tmp = recv(sfd, rbuf, data_len - count, MSG_DONTWAIT);
		if(tmp > 0){
			//printf("tcp_recvEx() count=%d, tmp=%d, data_len=%d\n", count, tmp, data_len);//for debugging only
			if(data_len - tmp <= 0)
				return count;			//全部数据接收完毕
			else{
				count += tmp;
				rbuf += tmp;			//指针位移
				continue;					//数据未接收完毕, 继续接收
			}
		}
		if(tmp == 0){
			//if(data_len == tmp)//data_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
			if(data_len == 0)
				return 0;
			else
				return -1;				//对端关闭, socket 什么数据都没有接收到
		}
		//if(tmp < 0)					//这句话可以省掉, 节省语句
		if(errno == EINTR)		//操作被信号中断, 则继续
			continue;
		if(errno == EAGAIN)		//网卡正忙, 继续尝试
			continue;
		//尝试失败, 接收错误
		perror("recv()");
		return -1;
	}
}

//异步: 发送一次数据(永远不会挂起线程), 成功返回发送的数据size, 失败返回-1
int tcp_sendEx(int sfd, const char* sbuf, unsigned int data_len){
	int tmp;
	unsigned int count = 0;
	const char* ptmp = sbuf;
	while(1){
		tmp = send(sfd, sbuf, data_len - count, MSG_DONTWAIT);
		if(tmp > 0){
			if(data_len - count <= 0)
				return count;			//全部数据发送完毕
			else{
				count += tmp;
				sbuf += tmp;			//指针位移
				continue;					//数据未发生完毕, 继续发生
			}
		}
		if(tmp == 0){
			//if(data_len == tmp)//data_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
			if(data_len == 0)
				return 0;
			else
				return -1;				//对端关闭, socket 什么数据都没有接收到
		}
		//if(tmp < 0)					//这句话可以省掉, 节省语句
		if(errno == EINTR)		//操作被信号中断, 则继续
			continue;
		if(errno == EAGAIN)		//网卡正忙, 继续尝试
			continue;
		//尝试失败, 接收错误
		perror("send()");
		return -1;
	}
}



//c 语言字符串操作函数: 废弃
/*
unsigned int tcp_recvEx_getlen(char* rbuf, unsigned int buf_len){
	unsigned int *len = (unsigned int *)rbuf;
	return ntohl(len);
}

#define tcp_recvEx_getdata(rbuf) (rbuf[4])
//unsigned char* tcp_recvEx_getdata(char* rbuf){ return rbuf[4]; }

void tcp_sendEx_pack(unsigned char* sbuf, const unsigned char* data, unsigned int data_len){
	static unsigned char* sbuf = NULL;//非线程安全函数, 多线程操作时, 需要把这两个静态变量, 写入线程属性中, 做线程全局变量;
	static unsigned int buf_len = 0;
	unsigned int tmp;
	if(buf_len < data_len + 4){
		//计算新的缓冲区大小(对齐内存)
		if(buf_len % 8 == 0)
			buf_len = (data_len + 4) * 8;
		else
			buf_len = ( data_len + 4 + 8 - (buf_len % 8) ) * 8;
		//非空先释放
		if(sbuf != NULL)
			free(sbuf);
		//无论空/非空, 最后都创建新的内存块
		sbuf = (char*)malloc(buf_len);
	}
	memcpy(sbuf, htonl(data_len), 4);
	memcpy(sbuf, data, data_len);
}
*/
