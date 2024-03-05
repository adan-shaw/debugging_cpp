#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>



//数据类型: socklen_t = size_t = unsigned int; (不能有负数长度)



//发送/接受时, socket 缓冲区占用情况(以发送为例, 接收同理):
/*
	在send()的时候, 返回的是实际发送出去的字节;

	在同步发送时:
		不使用socket缓冲区, 而是使用send() 操作'指认缓冲区', 直接发送'指认缓冲区'中的数据;

	在异步发送时:
		先将数据存放到缓冲区, 网卡空闲时, 会自动发送, 
		返回errno: EAGAIN / EWOULDBLOCK
			表示'发送数据'已经拷贝至'socket sbuf发送缓冲区',
			等待网卡空闲, 将会自动执行发送;
		如果send()在等待协议传送数据时, 网络突然'断开/错误', 则执行SO_LINGER socket关闭例程

	sendfile()时:
		直接读取文件发送, 不创建任何缓冲区, 直接内核网卡buf 缓冲, 读取文件内容发送;
		(最好加只读文件锁, 只读共享, 写操作需要等待)
*/


//socket 缓冲区大小应该设置为多大的问题:
/*
	并不是越大越好, 具体业务需求;
	一个socket的rbuf+sbuf, 默认值:8kb*2, (实际数值: 8688字节(约为8.5K))
	加上sfd 文件描述符的其他数据, 单个sfd 描述符的struct 结构体消耗, 并不是一个小数目
	加上tcp/ip 双向通道+反向代理原则:
		一个通道就是2 个socket, 全双工同时读写;
		反向代理两个通道就是4 个socket, 开销不小;

	假设一个服务器并发2W 个socket, 也就5k个http 客户端, 10kb*2 * 2w = 400mb左右, 并不是小数目;
	所以一般互联网业务, socket 缓冲区默认就够用了, 除非是流媒体业务, 那用udp 比较适合;
*/



//s0.设置fd nonblocking								[对fcntl()的F_GETFL返回值进行'| 或运算'=新增]
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}
//#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK) == -1) perror("fcntl()");}

//s0.设置fd blocking(取消nonblocking)	[对fcntl()的F_GETFL返回值进行'& 与运算'=削减]
#define __set_blocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)&O_NONBLOCK);}
//#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)&O_NONBLOCK) == -1) perror("fcntl()");}



//****************
// set SOL_SOCKET 
//****************

//s1.SO_RCVBUF 设置接收缓冲区(默认上限8kb) [内存不足的情况下, 可能会因为申请缓冲区而出错]
#define __set_sockopt_revbuf(sfd,rbuf_max) {setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &rbuf_max, sizeof(int));}

//s2.SO_SNDBUF 设置发送缓冲区(默认上限8kb)
#define __set_sockopt_sndbuf(sfd,sbuf_max) {setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &sbuf_max, sizeof(int));}

//s3.SO_RCVLOWAT 设置'触发io接收事件的下限'(默认下限1 bit) [数据触发io事件下限, 低于这个数据量不认为有socket recv操作]
#define __set_sockopt_revlowat(sfd,data_min) {setsockopt(sfd, SOL_SOCKET, SO_RCVLOWAT, &data_min, sizeof(int));}

//s4.SO_SNDLOWAT 设置'触发io发送事件的下限'(默认下限1 bit) [数据触发io事件下限, 低于这个数据量不认为有socket send操作]
#define __set_sockopt_sndlowat(sfd,data_min) {setsockopt(sfd, SOL_SOCKET, SO_SNDLOWAT, &data_min, sizeof(int));}

//s5.SO_RCVTIMEO 设置接收超时(linux默认最高是70s)
#define __set_sockopt_revtimeout(sfd,ptimeval) {setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, ptimeval,sizeof(struct timeval));}

//s6.SO_SNDTIMEO 设置发送超时(linux默认最高是70s)
#define __set_sockopt_sndtimeout(sfd,ptimeval) {setsockopt(sfd, SOL_SOCKET, SO_SNDTIMEO, ptimeval,sizeof(struct timeval));}

//s7.SO_REUSERADDR 设置ip地址重用(默认关闭)
//(正常情况下: ip地址释放后2分钟后, 才能被再次使用; 但设置SO_REUSEADDR, ip地址释放后, 可以马上再次使用)
#define __set_sockopt_reuseaddr(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(int));}

//s8.SO_EXCLUSIVEADDRUSE 设置端口独占(没有这个选项?)
#define __set_sockopt_exclusiveaddr(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, &opt_val, sizeof(int));}

//s9.SO_REUSEPORT 设置端口重用(默认关闭)
#define __set_sockopt_reuseport(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &opt_val, sizeof(int));}

//s10.SO_BSDCOMPAT 设置兼容BSD tcp/ip(默认关闭)
#define __set_sockopt_bsd(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_BSDCOMPAT, &opt_val, sizeof(int));}

//s11.SO_OOBINLINE 设置带外数据的返回方式: 在普通socket数据流中返回
#define __set_sockopt_oob(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_OOBINLINE, &opt_val, sizeof(int));}

//s12.SO_UPDATE_ACCEPT_CONTEXT 更新SOCKET状态
#define __set_sockopt_update(sfd) {setsockopt(sfd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, &sfd, sizeof(int));}



//*****************
// get SOL_SOCKET 
//*****************

inline int get_sockopt_opts(int sfd, const int sock_opt){
	int opt_val = -1;
	int opt_len = sizeof(int);
	if(getsockopt(sfd,SOL_SOCKET,sock_opt,&opt_val,&opt_len)==-1){
		perror("getsockopt");
		return -1;
	}
	return opt_val;					//返回: 开启返回'1/整形数', 未开启返回0 [统一标准的, 不用担心]
}


//g1.__get_sockopt_opts_int(): 查询socketopt 是否已经开启, 成功返回int 整形(大于0), 失败返回-1
//支持查询的socketopt list:
/*
	SO_TYPE									SOCK_STREAM / SOCK_DGRAM / SOCK_RAW ...
	SO_RCVBUF
	SO_SNDBUF
	SO_RCVLOWAT
	SO_SNDLOWAT
	SO_RCVTIMEO
	SO_SNDTIMEO
	SO_MAX_MSG_SIZE					一条消息的最大长度
*/
#define __get_sockopt_opts_int(sfd,sock_opt) (get_sockopt_opts(sfd, sock_opt))


//g2.__get_sockopt_opts_bool(): 查询socketopt 是否已经开启, 开启返回1, 未开启返回0, bug返回-1
//支持查询的socketopt list:
/*
	SO_REUSEADDR
	SO_EXCLUSIVEADDRUSE
	SO_REUSEPORT
	SO_BSDCOMPAT
	SO_OOBINLINE						查询带外数据是否会在普通数据流中返回

	SO_DEBUG								允许调试输出(很少用)
	SO_ERROR								查询socket 错误的原因, 返回一个errno 值
	SO_UPDATE_ACCEPT_CONTEXT更新SOCKET状态

	SO_DONTROUTE						opt_ip.c:  设置禁止转发

	SO_CONDITIONAL_ACCEPT		opt_tcp.c: 异步socket 阻塞connect()
	SO_KEEPALIVE						opt_tcp.c: 保活
	SO_DONTLINGER						opt_tcp.c: 是否禁用LINGER优雅关闭
	SO_ACCEPTCONN						opt_tcp.c: 查询sfd是否在监听端口状态(udp不兼容,只能查询,不能设置)

	SO_BROADCAST						opt_udp.c: 查询sfd是否开启了'接收广播包'
*/
#define __get_sockopt_opts_bool(sfd,sock_opt) (get_sockopt_opts(sfd, sock_opt))


//g3.SO_ERROR 快速函数
#define __set_sockopt_err(sfd,opt_val) {opt_val=1; if(opt_val=__get_sockopt_opts_bool(sfd, SOL_SOCKET, SO_ERROR, &opt_val, sizeof(int))) printf("%s\n",strerror(opt_val));}



