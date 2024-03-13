//udp 异步太复杂, 暂时不搞(有需要的话, 请自己动手实现)
/*
	udp 异步需要重写实现:
		* 异步udp_accept()
		* 异步udp_send()
		* 异步udp_recv()
		* 异步udp_write()
		* 异步udp_read()

	以上这些函数都需要进行异步改造, 比较麻烦复杂, 
	可以参考网易云风skynet 的源码, 更现成, 直接的代码, 参考价值更高;
	本教材只做原理演示, 属于教学性质;
*/



//同步: 根据ip & port阻塞发送一次数据(一旦阻塞, 线程会被挂起), 成功返回发送的数据size, 失败返回-1; [* 这样封装, 有点复杂, 实用意义不大, 只做模板参考 *]
int udp_send(int sfd, struct sockaddr_in *addr_peer, const char* ip, unsigned short port, const char* pbuf, int data_len){
	int tmp;
	struct sockaddr_in addr;

	if(addr_peer == NULL){//不输入addr_peer, 则自动填充
		//bzero(&addr, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		if(ip == NULL)
			addr.sin_addr.s_addr = INADDR_ANY;
		else
			addr.sin_addr.s_addr = inet_addr(ip);
		addr_peer = addr;
	}

	tmp = sendto(sfd, pbuf, data_len, 0, (struct sockaddr*)addr_peer, sizeof(struct sockaddr_in));
	if(tmp > 0)
		return tmp;//发送成功(非发送成功, 就应该close(sfd))
	if(tmp == 0){
		//if(data_len == tmp)//data_len == tmp == 0
		if(data_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 数据没有发出去
	}
	//此时, tmp == -1
	perror("sendto()");
	return -1;
}

//同步: 根据ip & port阻塞接收一次数据(一旦阻塞, 线程会被挂起), 成功返回发送的数据size, 失败返回-1; [* 这样封装, 有点复杂, 实用意义不大, 只做模板参考 *]
int udp_recv(int sfd, struct sockaddr_in *addr_peer, const char* ip, unsigned short port, const char* pbuf, int buf_len){
	int tmp;
	socklen_t len = sizeof(struct sockaddr_in);
	struct sockaddr_in addr;

	if(addr_peer == NULL){//不输入addr_peer, 则自动填充
		//bzero(&addr, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		if(ip == NULL)
			addr.sin_addr.s_addr = INADDR_ANY;
		else
			addr.sin_addr.s_addr = inet_addr(ip);
		addr_peer = addr;
	}

	tmp = recvfrom(sfd, *pbuf, buf_len, 0, (struct sockaddr*)addr_peer, &len);
	if(tmp > 0)
		return tmp;//接收成功(非接收成功, 就应该close(sfd))
	if(tmp == 0){
		//if(buf_len == tmp)//buf_len == tmp == 0 传入的数据长度为0 (虽然极度不推荐这样做, 但既然传入了, 逻辑上也要返回0)
		if(buf_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 什么数据都没有接收到
	}
	//此时, tmp == -1
	perror("recvfrom()");
	return -1;
}



//同步: 向一个已执行bind() + connect() 的udp sfd 进行write() 操作; [* 这样封装, 有点复杂, 实用意义不大, 只做模板参考 *]
int udp_write(int sfd, const char* pbuf, int data_len){
	int tmp = write(sfd, pbuf, data_len);
	if(tmp > 0)
		return tmp;//发送成功(非发送成功, 就应该close(sfd))
	if(tmp == 0){
		//if(data_len == tmp)//data_len == tmp == 0
		if(data_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 数据没有发出去
	}
	//此时, tmp == -1
	perror("write()");
	return -1;
}

//同步: 向一个已执行bind() + connect() 的udp sfd 进行read() 操作; [* 这样封装, 有点复杂, 实用意义不大, 只做模板参考 *]
int udp_read(int sfd, const char* pbuf, int buf_len){
	int tmp = read(sfd, *pbuf, buf_len);
	if(tmp > 0)
		return tmp;//接收成功(非接收成功, 就应该close(sfd))
	if(tmp == 0){
		//if(buf_len == tmp)//buf_len == tmp == 0
		if(buf_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 什么数据都没有接收到
	}
	//此时, tmp == -1
	perror("read()");
	return -1;
}




//1.io 操作错误时, 返回的errno 值的意思
/*
	case EAGAIN:					//套接字已标记为非阻塞, 网卡正忙, 稍后再试

	case ECONNREFUSE:			//远程主机阻绝网络连接
	case ENOTCONN:				//与对端套接字尚未被连接上

	case EFAULT:					//内存空间访问出错
	case ENOMEM:					//内存不足
	case EINTR:						//操作被信号中断
	case EINVAL:					//参数无效

	case EBADF:						//sfd不是有效的描述符
	case ENOTSOCK:				//sfd不是套接字(当返回值是0时, 为正常关闭连接)
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
