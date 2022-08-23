#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>



//服务器bind 绑定端口, 类似tcp listen() 端口, 也会拷贝套接字属性;
//但udp 没有握手过程, 服务器需要获取客户端的ip+port, 并记录下来, 当作回发信息的时候使用;

/*
	单播:
		udp 点对点, 类似tcp.
		实际上tcp也是一帧一帧数据发出去的, 跟udp没啥区别, udp也可以点对点.
		但数据安全, 完整性/正确性保障, udp需要用户自己实现.
		udp 比tcp 点对点的不足:
			1.udp 不能扛冲击(没有握手机制), 防止伪造数据攻击, 各种安全功能要自己搭建;
			2.网络传输中丢失机会会更高, 也没有应答机制, 收到数据没有应答回复;
			3.丢包重发机制要自己建;
			4.数据接收, 同样需要重排, 而且可能会有rbuf接收缓冲区溢出的问题;
			5.( 以后再补充, 差不多了 )


	多播[或者叫组播](类似internet dns 解析服务):
		A主机创建multi_cast 地址后,
		其他主机就可以通过gethostbyname()获取'多播组'(多播主机地址列表)了;
		A主机负责维护'多播组', 更新重发/关闭通知等.
		A主机负责提供多播搭建的询问服务.

		多播可以跨网络多播, 同样可以在互联网上面做多播.
		多播地址 = gethostbyname(需要用inet_pton 创建)
		interface = 随意ip 分配
		要接收多播数据, 需要先加入'多播组'(多播主机地址列表).

		UDP组播的基本步骤:
			建立socket
			socket和端口绑定
			加入一个组播组
			通过sendto / recvfrom进行数据的收发
			关闭socket

		另外:
			用tcp 做'逻辑轮询,遍历发送'式多播, 实际上也是可行的, 但是很幼稚, 慢很多;
			低级互联网业务, 可以这么玩, 如数据量不大, 实时性要求不高的, 可以这么做;


	广播:
		每个socket 都是独立体, 仅在局域网内广播;
		谁打开了broadcast socketopt mode,
		谁就可以接收<局域网>广播数据.
		只要局域网内'任何一个成员'发广播数据,
		局域网内'全体打开broadcast socketopt mode的sfd'都会收到数据.

		现在的网络设备都会阻止广播包, 防止广播风波的问题, 但不会拒绝多播报文.
		也可以调节路由器, 接收广播报文;
*/



//****************
// set SOL_SOCKET 
//****************

//s1.SO_BROADCAST 允许接收广播包
/*
	不开启的话, 否则广播数据都会被扔包, 
	linux 收到之后, 会找对应的套接字确认, 是否需要广播包, 不需要就扔掉
*/
#define __set_sockopt_broadcast(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &opt_val, sizeof(int));}



//****************
// set IPPROTO_IP 
//****************

//s1.IP_MULTICAST_IF 设置sfd绑定port端口作为'多播数据端口'来使用(占用)
#define __set_sockopt_multicast_port(sfd,port) {setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_IF, &port,sizeof(int));}

//s2.IP_MULTICAST_TTL 设置多播数据的TTL(多播默认TTL为1)
#define __set_sockopt_multicast_ttl(sfd,ttl) {setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,sizeof(int));}

//s3.IP_MULTICAST_LOOP 如果为TRUE, 发至的多播数据会'原封不动地'反射回'套接字的进入缓冲区'(默认会给套接字数据队列中)
#define __set_sockopt_multicast_loop(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_LOOP, &opt_val, sizeof(int));}


//一下的多播操作, 带结构体的都是: [只能set设置,不可get查询]
//其实也不需要查询, 只要同意接收多播数据, 就自动接收, 无需查询;
//想踢某个ip 出多播组, 有就踢, 没有就是空操作;


//s4.IP_ADD_MEMBERSHIP 将sfd加入'指定多播组'[只能set设置,不可get查询]
/*
	struct ip_mreq{
		struct in_addr imr_multiaddr;	//多播组主机ip地址=gethostbyname(), 需要用inet_pton 来创建;
		struct in_addr imr_interface;	//自己的ip, 可随意分配htonl(INADDR_ANY); 用来告诉对方, 这是我的ip;
};
*/
inline int __set_sockopt_multicast_add(int *sfd, const char* multiaddr, const char* ip_himself, struct ip_mreq* ip_group){
	ip_group->imr_multiaddr.s_addr = inet_addr(multiaddr);
	//ip_group->imr_interface.s_addr = htonl(INADDR_ANY);		//自动分配当前网卡ip
	ip_group->imr_interface.s_addr = inet_addr(ip_himself);	//指定网卡ip
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ip_group, sizeof(struct ip_mreq))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}

//s5.IP_DROP_MEMBERSHIP 设置将sfd从'指定多播组'删除(退出多播组)
inline int __set_sockopt_multicast_drop(int *sfd, const char* multiaddr, const char* ip_himself, struct ip_mreq* ip_group){
	ip_group->imr_multiaddr.s_addr = inet_addr(multiaddr);
	ip_group->imr_interface.s_addr = inet_addr(ip_himself);
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ip_group, sizeof(struct ip_mreq))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}


//s6.IP_ADD_SOURCE_MEMBERSHIP 绑定某个多播ip, 缩减多播接收范围, 只接收某个ip 的多播数据;
/*
	在多播组的基础上, 只接受某个ip 发出的多播数据, 其他ip 发出的多播数据忽略, 扔掉;
	一般用作: 客户端锁定服务器, 只接收服务器发来的多播信息, 客户端之间的多播信息, 直接忽略屏蔽;
*/
/*
	struct ip_mreq_source{
		struct in_addr imr_multiaddr;		//多播组主机ip地址=gethostbyname()
		struct in_addr imr_sourceaddr;	//可接收的'源数据的IP地址'(一般是服务器ip 地址, 也是多播管理员imr_multiaddr 的ip)
		struct in_addr imr_interface;		//自己的ip, 可随意分配htonl(INADDR_ANY); 用来告诉对方, 这是我的ip;
	};
*/
inline int __set_sockopt_multicast_add_src(int *sfd, const char* multiaddr, const char* srcaddr, \
		const char* ip_himself, struct ip_mreq_source* ip_group_s){
	ip_group_s->imr_multiaddr.s_addr = inet_addr(multiaddr);
	ip_group_s->imr_sourceaddr.s_addr = inet_addr(srcaddr);
	ip_group_s->imr_interface.s_addr = inet_addr(ip_himself);
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ip_group_s, sizeof(struct ip_mreq_source))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}

//s7.IP_DROP_SOURCE_MEMBERSHIP 解除某个多播ip的绑定
inline int __set_sockopt_multicast_drop_src(int *sfd, const char* multiaddr, const char* srcaddr, \
		const char* ip_himself, struct ip_mreq_source* ip_group_s){
	ip_group_s->imr_multiaddr.s_addr = inet_addr(multiaddr);
	ip_group_s->imr_sourceaddr.s_addr = inet_addr(srcaddr);
	ip_group_s->imr_interface.s_addr = inet_addr(ip_himself);
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, &ip_group_s, sizeof(struct ip_mreq_source))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}


//s8.IP_BLOCK_SOURCE 设置多播黑名单(拉黑某个ip 源)
inline int __set_sockopt_multicast_block_src(int *sfd, const char* multiaddr, const char* srcaddr, \
		const char* ip_himself, struct ip_mreq_source* ip_group_s){
	ip_group_s->imr_multiaddr.s_addr = inet_addr(multiaddr);
	ip_group_s->imr_sourceaddr.s_addr = inet_addr(srcaddr);
	ip_group_s->imr_interface.s_addr = inet_addr(ip_himself);
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_BLOCK_SOURCE, &ip_group_s, sizeof(struct ip_mreq_source))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}

//s9.IP_UNBLOCK_SOURCE 解除多播黑名单(解除拉黑ip 源)
inline int __set_sockopt_multicast_unblock_src(int *sfd, const char* multiaddr, const char* srcaddr, \
		const char* ip_himself, struct ip_mreq_source* ip_group_s){
	ip_group_s->imr_multiaddr.s_addr = inet_addr(multiaddr);
	ip_group_s->imr_sourceaddr.s_addr = inet_addr(srcaddr);
	ip_group_s->imr_interface.s_addr = inet_addr(ip_himself);
	if(-1 == setsockopt(*sfd, IPPROTO_IP, IP_UNBLOCK_SOURCE, &ip_group_s, sizeof(struct ip_mreq_source))){
		perror("setsockopt");
		return 0;
	}
	else
		return 1;
}



//****************
// get IPPROTO_IP 
//****************


inline int get_sockopt_opts_udp(int sfd, const int sock_opt){
	int opt_val = -1;
	int opt_len = sizeof(int);
	if(getsockopt(sfd,IPPROTO_IP,sock_opt,&opt_val,&opt_len)==-1){
		perror("getsockopt");
		return -1;
	}
	return opt_val;//返回: 开启返回'1/整形数', 未开启返回0 [统一标准的, 不用担心]
}


//g1.__get_sockopt_opts_int(): 查询socketopt 是否已经开启, 成功返回int 整形(大于0), 失败返回-1
//支持查询的socketopt list:
/*
	IP_MULTICAST_IF
	IP_MULTICAST_TTL
*/
#define __get_sockopt_opts_udp_int(sfd,sock_opt) (get_sockopt_opts_udp(sfd, sock_opt))


//g2.__get_sockopt_opts_bool(): 查询socketopt 是否已经开启, 开启返回1, 未开启返回0, bug返回-1
//支持查询的socketopt list:
/*
	IP_MULTICAST_LOOP
*/
#define __get_sockopt_opts_udp_bool(sfd,sock_opt) (get_sockopt_opts_udp(sfd, sock_opt))






