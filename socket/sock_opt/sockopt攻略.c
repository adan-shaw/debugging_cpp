//socket addr 重用的意义:
/*
	sfd close(), 一般不会立即关闭, 而是需要经历TIME_WAIT的过程后, 才会关闭socket;

	sfd shutdown(), 可以蔑视TIME_WAIT的过程, 直接通知client 不用等了, 直接关闭socket;
	但shutdown() 使用起来非常麻烦, 不可能每个失败操作, 都可以shutdown(), shutdown() 是主动的, 不应该常用;


	因此, 想要忽视TIME_WAIT的过程的办法, 有三个:
		* 重用该addr, 即便占用, 也不影响;
			addr 重用可以让两个socket 能同时占用一个addr, 防止TIME_WAIT的过程导致addr 占用的问题;

		* 当然也可以shutdown(), 但只能单次操作, 没法一一应对意外关闭的情况;

		* 或者直接设置socket 属性为SO_DONTLINGER, 不允许Linger, 这样也可以快速关闭socket 而忽视TIME_WAIT的过程;
*/

//socket port 重用的意义:
//	混杂模式, 手动混杂模式, 一个端口两个app server;





//1.设置地址重用
int tmp=TRUE;
setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&tmp, sizeof(int));

//2.如果要已经处于连接状态的soket在调用close()后强制关闭, 不经历TIME_WAIT的过程: 
int tmp = FALSE;
setsockopt(sfd, SOL_SOCKET, SO_DONTLINGER, (const char*)&tmp, sizeof(int));

//3.如果在发送数据的过程中(send()没有完成, 还有数据没发送)而调用了close(),
//	以前我们一般采取的措施是"从容关闭"shutdown(s,SD_BOTH),
//	但数据是肯定丢失了, 如何设置让程序满足具体应用的要求(即让没发完的数据发送出去后, 再关闭socket)
struct linger {
	u_short l_onoff;
	u_short l_linger;
};
linger m_sLinger;

m_sLinger.l_onoff=1;	//在close()调用,但还有数据没发送完毕的时候容许逗留; 如果m_sLinger.l_onoff=0; 则等同SO_DONTLINGER, 会直接粗暴关闭socket
m_sLinger.l_linger=3;	//(容许逗留的时间为3秒)
setsockopt(sfd, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger));



//4.在send(),recv()过程中, 有时由于网络状况等原因, 发收不能预期进行, 而设置收发时限: 
int timeout=1000;//1秒
setsockopt(socket, SOL_S0CKET, SO_SNDTIMEO, (char *)&timeout, sizeof(int));//发送时限
setsockopt(socket, SOL_S0CKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));//接收时限



//5.在send()的时候, 返回的是: 实际'发送/接收'的字节(同步), 或'发送/接收'到socket缓冲区的字节(异步);
//	在实际的过程中'发送/接收'数据量比较大, 可以设置更大的socket缓冲区, 而避免了send(), recv()不断的循环收发: 
//	ps: 系统默认的发送和接收一次为8688字节(约为8.5K);
int rbuf=32*1024;//设置为32K
setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, (const char*)&rbuf, sizeof(int));//接收缓冲区
int sbuf=32*1024;
setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, (const char*)&sbuf, sizeof(int));//发送缓冲区



//6.如果在发送数据的时, 希望不经历由系统缓冲区到socket缓冲区的拷贝而影响程序的性能(即关闭系统socket 自带的缓冲区, 直接用户态内存buf 发送);
//	send() 默认情况是: 将用户态缓冲区拷贝到socket缓冲区的内容;
//	recv() 默认情况是: 将socket缓冲区的内容拷贝到用户态缓冲区;
int tmp=0;
setsockopt(socket, SOL_S0CKET, SO_SNDBUF, (char *)&tmp, sizeof(int));
setsockopt(socket, SOL_S0CKET, SO_RCVBUF, (char *)&tmp, sizeof(int));



//7.在client连接服务器过程中, 如果处于非阻塞模式下的socket在connect()的过程中可以设置connect()延时, 直到accpet()被呼叫;
//	(本函数设置只有在非阻塞的过程中有显著的作用, 在阻塞的函数调用中作用不大)
int tmp=TRUE;
setsockopt(sfd, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&tmp, sizeof(int));





