#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>



//****************
// set SOL_SOCKET 
//****************
//s1.SO_DONTROUTE 设置禁止转发, '直接向对方的网络接口发送消息', 无需查询本机路由表;(同一局域网内可用)
/*
	SO_DONTROUTE 不管你本机设置网关是什么, 它总是以数据的目的地址作为下一跳;
	所以如果对方的网络接口, 和你不是一个网段, 可能会不可达;

	SO_DONTROUTE 能跳过本机路由表, 但不能跳过路由器的路由表;
	所以, SO_DONTROUTE 并不能命令路由器如何工作;
*/
#define __set_sockopt_dontroute(sfd,opt_val) {opt_val=1; setsockopt(sfd, SOL_SOCKET, SO_DONTROUTE, &opt_val, sizeof(int));}


//s2.SO_BINDTODEVICE 设置socket 绑定固定网卡, 所有io 只通过这个网卡进行发送
/*
	SO_BINDTODEVICE 只是为套接字绑定了一个网络接口, 非常适合listen();
	如果tcp listen 使用SO_BINDTODEVICE 绑定了固定的网卡, 
	那么所有的客户端socket, 都只跟这个网卡口通信,
	因为accept() 实际上是拷贝listen sfd, 
	则自然会拷贝SO_BINDTODEVICE 属性
*/
#define __set_sockopt_binddevice(sfd,pifreq) {setsockopt(sfd, SOL_SOCKET, SO_BINDTODEVICE, pifreq,sizeof(struct ifreq));}
/*
	//标准使用demo:
	int __set_sockopt_binddevice(int sfd, const char* dev){
		struct ifreq interface;
		strncpy(interface.ifr_name, dev, IFNAMSIZ);
		if(-1 == setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,(char*)&interface, sizeof(struct ifreq))){
			perror("setsockopt");
			return 0;
		}
		return 1;
	}
*/



//****************
// set IPPROTO_IP 
//****************

//s1.IP_HDRINCL 如果是TRUE, IP头就会随'即将发送的数据'一起提交, 并从'读取的数据'中返回;
/*
	并不会多copy 一份ip 头, 
	只是会在读取数据的时候, 将ip 头也一并写入rbuf 读取缓冲区, 
	需要SOCKET_RAW, 需要root 权限, 一般原始套接字才用;
*/
#define __set_sockopt_hdrincl(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int));}

//s2.IP_TTL 设置ttl(数据包在路由过程中的'转发存活时间')
#define __set_sockopt_ttl(sfd,ttl) {setsockopt(sfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));}

//s3.IP_TOS 设置IP服务类型
//	 在IP头中,有一个Type-of-Service字段,该字段描述了IP包的优先级和QoS选项,IP_TOS可以来设定该字段的值;
#define __set_sockopt_tos(sfd,tos) {setsockopt(sfd, IPPROTO_IP, IP_TOS, &tos, sizeof(int));}

//s4.IP_DONTFRAGMENT 设置不准对ip数据报进行分片
//	 设置后,如果IP数据报大于MTU,就会被丢弃,并返回ICMP错误)
#define __set_sockopt_dontfragment(sfd,opt_val) {opt_val=1; setsockopt(sfd, IPPROTO_IP, IP_DONTFRAGMENT, &opt_val, sizeof(int));}

//s5.IP_OPTIONS 手工设置ip 头
/*
	struct in_pktinfo{
		unsigned int ipi_ifindex;			//接口索引
		struct in_addr ipi_spec_dst;	//路由目的地址
		struct in_addr ipi_addr;			//头标识目的地址
	};
	ipi_ifindex		指的是接收包的接口的唯一索引
	ipi_spec_dst	指的是路由表记录中的目的地址
	ipi_addr			指的是包头中的目的地址

	如果给sendmsg(2)传递了IP_PKTINFO,
	那么外发的包会通过在ipi_ifindex中指定的接口发送出去,
	同时把ipi_spec_dst设置为目的地址．
*/
#define __set_sockopt_options(sfd,pin_pktinfo) {setsockopt(sfd, IPPROTO_IP, IP_OPTIONS, pin_pktinfo,sizeof(struct in_pktinfo));}



//*****************
// get IPPROTO_IP 
//*****************

inline int get_sockopt_opts_ip(int sfd, const int sock_opt){
	int opt_val = -1;
	int opt_len = sizeof(int);
	if(getsockopt(sfd,IPPROTO_IP,sock_opt,&opt_val,&opt_len)==-1){
		perror("getsockopt");
		return -1;
	}
	return opt_val;//返回: 开启返回'1/整形数', 未开启返回0 [统一标准的, 不用担心]
}


//g1.__get_sockopt_opts_int_ip(): 查询socketopt 是否已经开启, 成功返回int 整形(大于0), 失败返回-1
//支持查询的socketopt list:
/*
	IP_TTL
	IP_TOS
*/
#define __get_sockopt_opts_ip_int(sfd,sock_opt) (get_sockopt_opts_ip(sfd, sock_opt))


//g2.__get_sockopt_opts_bool_ip(): 查询socketopt 是否已经开启, 开启返回1, 未开启返回0, bug返回-1
//支持查询的socketopt list:
/*
	IP_HDRINCL
	IP_DONTFRAGMENT
*/
#define __get_sockopt_opts_ip_bool(sfd,sock_opt) (get_sockopt_opts_ip(sfd, sock_opt))





