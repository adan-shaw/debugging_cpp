#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>



//TCP 轮询单播的作用(互联网应用, 也很适合):
/*
	由于多播始终是基于UDP 协议, 面对较为准确的数据传递是, 需要自己手动实现数据安全传输;
	(多播数据传输, 实际安全性不高)
	在数据量不大的情况下, 直接使用TCP 轮询单播, 也是一种不错的选择;

	一般互联网-异步服务器, 50ms-150ms延迟都是可以接受的, 200ms以上开始不行了;
	做工业偏实时服务器, 或者做多媒体重数据流服务器, 一般会用udp'组播/广播';
*/



//*************
// SOL_SOCKET 
//*************

//g1.SO_ACCEPTCONN 查询sfd是否在监听端口状态(udp不兼容,只能查询,不能设置)



//s1.SO_CONDITIONAL_ACCEPT 设置有条件连接, 对connect() 函数有效(默认是关闭的)
/*
	当客户端设置socket为异步模式时, connect 服务器失败会马上返回,
	设置SO_CONDITIONAL_ACCEPT 延时, '客户端异步sfd'执行connect 会陷入阻塞, 直到链接成功
*/
#define __set_sockopt_connectEx(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, &opt_val, sizeof(int));}


//s2.SO_KEEPALIVE 设置设置tcp socket 内嵌'心跳包'
/*
	SO_KEEPALIVE 是一个非常争议的选项, 只能保证socket 的活性, 不能做其它校验工作;

	SO_KEEPALIVE 缺陷1: (不能做其它校验工作)
		利用TCP／IP协议层内置的SO_KEEPALIVE 功能来实现'心跳功能',
		虽然简单, 但不能做其它校验工作, 
		'心跳包'由tcp/ip协议自定义, 简单省时, 但拓展性不强;
		如果没办法在'心跳包'中添加数据, 保活客户端可能会出现:
			网络io跟得上, 但数据已经与服务器/其他客户端脱节, 造成业务错误,
			所以, 一般游戏服务器要自己做'心跳包'.

	SO_KEEPALIVE 缺陷2:
		(SO_KEEPALIVE 数据包'优先级低', 突然断线时, 会被resend 数据包抢占)
		如果TCP 客户端'停电/断网', 但我们无法知道连接已经断开,
		因此, 发送数据失败时, 会自动进行重传.
		由于重传包的优先级要高于SO_KEEPALIVE 的数据包,
		因此SO_KEEPALIVE 的数据包无法发送出去.
		只有在长时间的重传失败之后我们才能判断此连接断开了.
		(重传失败, 判断连接断开需要较长时间, 达不到及时性失效检测)

	SO_KEEPALIVE 缺陷3:
		(SO_KEEPALIVE 定时器, 会被resend重置, 重传操作, 不断重置定时器, 定时器被占用, SO_KEEPALIVE 根本没有使用权)
		SO_KEEPALIVE 的原理是(需要占用定时器):
			在'定时器时间周期内', 没有任何数据接收和发送, 才会发出SO_KEEPALIVE 心跳包'检测对端是否存活',
		重传操作会占用'定时器', 因此SO_KEEPALIVE 模块将无法知道自己该在什么时候发出心跳包, 
		所以SO_KEEPALIVE 操作优先级, 低于resend
*/
#define __set_sockopt_keepalive(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &opt_val, sizeof(int));}


//s3.SO_LINGER 设置优雅关闭(如有io未完成, 则等待完成再关闭)
/*
	缺省close()行为是:
		如有数据残留在socket发送缓冲区上, 则先把剩余数据发送给对方, 等待发送完毕确认, 再close()返回;

	l_onoff = 0; 则SO_LINGER 被忽略, 等同于'缺省close()行为'

	l_onoff != 0; l_linger = 0;
		close()立刻返回, 通过发送RST分组(而不是用正常的FIN|ACK|FIN|ACK四次挥手分组)来关闭该连接;
		至于发送缓冲区中如果有未发送完的数据,立即丢弃;
		主动关闭'对方的TCP状态检测',跳过TIMEWAIT,直接进入CLOSED;
		网上很多人想利用这一点, 来解决服务器上出现大量的TIMEWAIT状态的socket的问题,
		但这并不一定是个好主意,具体需求具体分析;

	l_onoff != 0; l_linger > 0;(单位: 秒)
		close()不会立刻返回, 如果socket发送缓冲区中仍残留数据,
		则进程进入睡眠, 内核进入定时状态去尽量去发送这些数据;
		在超时之前, 如果所有数据都发送完且被对方确认,
		内核用正常的FIN|ACK|FIN|ACK四个分组来关闭该连接,close()成功返回;
		如果超时之时,数据仍然未能成功发送及被确认,
		则用上述'l_onoff != 0; l_linger = 0;'方式来关闭此连接, 且close()返回EWOULDBLOCK
*/
#define __set_sockopt_linger(sfd,plinger) {setsockopt(sfd, SOL_SOCKET, SO_LINGER, ptimeval, sizeof(struct linger));}

//s4.SO_DONTLINGER 禁用SO_LINGER
#define __set_sockopt_nonlinger(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_DONTLINGER, &opt_val, sizeof(int));}



//*****************
// set IPPROTO_TCP 
//*****************

//s1.TCP_NODELAY 设置使用Nagle算法
/*
	默认情况下, 发送数据采用Negale 算法.
	这样虽然提高了网络吞吐量, 但是实时性却降低了, 在一些交互性很强的应用程序来说是不允许的, 
	因此, 有实时性要求的场景, 可以使用TCP_NODELAY禁止Negale算法;

	禁用Nagle算法后, 应用程序向内核递交的每个数据包都会立即发送出去.
	需要注意的是, 虽然禁止了Negale算法, 但网络的传输仍然受到TCP确认延迟机制的影响.
	nagle算法:
		为了减少小包的发送次数,进行延迟发包.只有在两个情况下会触发发送:1.前面一个包的ack已经到达   2. 200ms超时到了


	详情查看: ./web网文/Nagle 算法 && CORK算法.html
*/
#define __set_sockopt_tcpnodelay(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(int));}


//s2.TCP_CORK 设置使用CORK算法
/*
	所谓的CORK就是塞子的意思,形象地理解就是用CORK将连接塞住,使得数据先不发出去,等到拔去塞子后再发出去.
	设置该选项后,内核会尽力把小数据包拼接成一个大的数据包(一个MTU)再发送出去,
	当然若一定时间后(一般为200ms,具体等待多久,尚待确认),
	内核仍然没有组合成一个MTU时也必须发送现有的数据(不可能让数据一直等待吧).
	然而,TCP_CORK的实现可能并不像你想象的那么完美,CORK并不会将连接完全塞住.
	内核其实并不知道应用层到底什么时候会发送第二批数据用于和第一批数据拼接以达到MTU的大小,
	因此内核会给出一个时间限制,在该时间内没有拼接成一个大包(努力接近MTU)的话,内核就会无条件发送.
	也就是说若应用层程序发送小包数据的间隔不够短时,
	TCP_CORK就没有一点作用,反而失去了数据的实时性(每个小包数据都会延时一定时间再发送).
	tcp_cork:
		它是一种加强的nagle算法,过程和nagle算法类似,都是累计数据然后发送.
		但它没有nagle中1的限制,
		所以,在设置cork后,即使所有ack都已经收到,
		但我还是不想发送数据,我还想继续等待应用层更多的数据,
		所以它的效果比nagle更好.
	详情查看: ./web网文/Nagle 算法 && CORK算法.html
*/
#define __set_sockopt_tcpcork(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_TCP, TCP_CORK, &opt_val, sizeof(int));}


//s3.TCP_QUICKACK 设置使用QUICKACK算法
/*
	用户层可通过setsockopt系统调用的选项TCP_QUICKACK开启QUICKACK模式, 
	但是这不是永久生效的, 内核根据之后的报文处理, 可能退出此模式;
	QUICKACK模式开启之后, 
	如果套接口存在需要调度的ACK, 调用tcp_cleanup_rbuf函数进一步检查ACK是否需要发送;
	详情查看: ./web网文/TCP之QUICKACK.html
*/
#define __set_sockopt_tcpquickack(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_TCP, TCP_QUICKACK, &opt_val, sizeof(int));}



//*****************
// get IPPROTO_TCP 
//*****************

//g1.get_sockopt_opts_tcp(): 查询socketopt 是否已经开启, 开启返回1, 未开启返回0, bug返回-1
//支持查询的socketopt list:
/*
	TCP_NODELAY
	TCP_CORK
	TCP_QUICKACK
*/
inline int get_sockopt_opts_tcp(int sfd, const int sock_opt){
	int opt_val = -1;
	int opt_len = sizeof(int);
	if(getsockopt(sfd,IPPROTO_TCP,sock_opt,&opt_val,&opt_len) == -1){
		perror("getsockopt()");
		return -1;
	}
	return opt_val;
}


//g2.TCP_INFO 获取tcp 相信信息(引用返回struct tcp_info* ptcp_info)
//详情, 查看: ./web网文/getsockopt.html
inline int get_sockopt_opts_tcpinfo(int sfd, struct tcp_info* ptcp_info){
	int len = sizeof(struct tcp_info);
	if(getsockopt(sfd,IPPROTO_TCP,TCP_INFO,ptcp_info,&len) == -1){
		perror("getsockopt()");
		return 0;
	}
	return 1;//调用成功返回1, 失败返回0
}
