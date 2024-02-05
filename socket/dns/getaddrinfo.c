//编译:
//		gcc -g3 ./getaddrinfo.c -o x 



//getaddrinfo() 可以知道某个服务器的公开服务类型, 如:
/*
	通过getaddrinfo() 可以查询到www.baidu.com 提供: tcp/udp/raw 服务
	另外www.baidu.com 还有两台网关server;
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>



//1.打印单个struct addrinfo*结构体
void print_addrinfo(const struct addrinfo* p){
	char str[INET6_ADDRSTRLEN];
	unsigned short port;
	struct sockaddr_in* in4;
	struct sockaddr_in6* in6;
	struct sockaddr_un* un;

	printf("ai_family: ");
	switch(p->ai_family){																	//打印协议簇
		case AF_INET:
			printf("AF_INET, tcp/udp/icmp/igmp/ip/eth\n");
			in4 = (struct sockaddr_in *)(p->ai_addr);					//直接赋值提取struct sockaddr_in地址信息
			port = ntohs(in4->sin_port);											//提取port
			inet_ntop(AF_INET,&in4->sin_addr,str,sizeof(str));//带截断转换ip
			printf("addr4: %s, port: %d\n", str, port);
			break;
		case AF_INET6:
			printf("AF_INET6, tcp/udp/icmp/igmp/ip/eth\n");
			in6 = (struct sockaddr_in6 *)(p->ai_addr);
			port = ntohs(in6->sin6_port);
			inet_ntop(AF_INET6,&in6->sin6_addr,str,sizeof(str));
			printf("addr6: %s, port: %d\n", str, port);
			break;
		//case AF_LOCAL:		printf("AF_LOCAL, tcp/udp\n"); break;//数值等价于AF_UNIX
		case AF_UNIX:
			printf("AF_LOCAL, tcp/udp\n");
			un = (struct sockaddr_un *)(p->ai_addr);
			printf("unix socket path: %s\n", un->sun_path);
			break;
		case AF_UNSPEC:			printf("AF_UNSPEC, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case AF_ROUTE:			printf("AF_ROUTE, tcp/udp/icmp/igmp/ip/eth\n"); break;
		//case AF_NETLINK:	printf("AF_NETLINK, tcp/udp/icmp/igmp/ip/eth\n"); break;//数值等价于AF_PACKET
		case AF_PACKET:			printf("AF_PACKET, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case AF_PPPOX:			printf("AF_PPPOX\n"); break;
		case AF_CAN:				printf("AF_CAN\n"); break;
		case AF_BLUETOOTH:	printf("AF_BLUETOOTH\n"); break;
		default:						printf("default: another ai_family\n"); break;
	}
	//printf("canonical name: %s\n", p->ai_canonname);		//规范名称服务位置(一般没有的, 弃用)

	printf("ai_socktype: ");//打印socket 类型
	switch(p->ai_socktype){
		case SOCK_STREAM:			printf("SOCK_STREAM, tcp only\n"); break;
		case SOCK_DGRAM:			printf("SOCK_DGRAM, tcp/udp\n"); break;
		case SOCK_RAW:				printf("SOCK_RAW, tcp/udp/icmp/igmp\n"); break;
		case SOCK_PACKET:			printf("SOCK_PACKET, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case SOCK_SEQPACKET:	printf("SOCK_SEQPACKET(SEQ PACKET), tcp/udp/icmp/igmp/ip/eth(linux not supported)\n"); break;
		default:							printf("default: another ai_socktype\n"); break;
	}

	printf("ai_protocol: ");//打印'传输协议'类型
	switch(p->ai_protocol){
		case IPPROTO_IP:		printf("ip\n"); break;
		case IPPROTO_TCP:		printf("tcp\n"); break;
		case IPPROTO_UDP:		printf("udp\n"); break;
		case IPPROTO_RAW:		printf("raw\n"); break;
		case IPPROTO_SCTP:	printf("sctp\n"); break;
		case IPPROTO_ICMP:	printf("icmp\n"); break;
		case IPPROTO_IGMP:	printf("igmp\n"); break;
		default:						printf("default: another ai_protocol\n"); break;
	}

	printf("\n");
	return;
}



//2.getaddrinfo()测试函数
void getaddrinfo_test(void){
	const char* serv = "80";															//可以直接写端口号
	//const char* serv = "http";													//也可以直接写'高层协议名'
	const char* host = "www.baidu.com";										//可以直接写url链接(主机域名)
	//const char* host = "180.101.49.14";									//可以直接写ip 地址
	struct addrinfo hint,*res,*res_save;
	int tmp;

	//使用getaddrinfo()前必须先初始化, 否则Segmentation fault
	//bzero(&hint, sizeof(hint));
	//hint.ai_family = AF_INET;
	hint.ai_family = AF_UNSPEC;//互联网自适应(ipv4 / ipv6)

	//执行getaddrinfo()
	tmp = getaddrinfo(host, serv, &hint, &res);
	if(tmp != 0){
		printf("getaddrinfo(%s:%s) error:%s\n", host, serv, gai_strerror(tmp));
		return;
	}

	//保存addrinfo{}结果链表的首地址, 方便释放内存.
	res_save = res;

	//遍历打印addrinfo{}结果链表(遍历的时候, struct addrinfo* 指针会移动, 请先保存!!)
	while(res != NULL){
		print_addrinfo(res);
		res = res->ai_next;
	}

	//释放内存
	freeaddrinfo(res_save);

	return;
}



int main(void){
	getaddrinfo_test();
	return 0;
}



/*
//1.posix dns 查询的简述:
	getaddrinfo(), getnameinfo()两个函数都是posix dns查询函数;

	getaddrinfo()根据'URL字符串', 或者'(ip+端口)字符串', 获取'addrinfo{}结构体'里面的描述的信息.

	getnameinfo()根据'struct sockaddr*通用socket地址信息描述体', 获取'(ip+端口)字符串'.

	'addrinfo{}结构体'是一个链表结构,
	每个'addrinfo{}结构体'都有一个'struct sockaddr*通用地址信息描述体'
	也就是:
		只要有URL字符串, 就可以获取到'目标URL的addrinfo{}结构体'.

		只要知道对方的'struct sockaddr*通用地址信息描述体', 也可以先获取'(ip+端口)字符串';
		再根据'(ip+端口)字符串'+getaddrinfo()得到=>'addrinfo{}结构体'

		只要socket已经连通, 可以通过getsockname(),
		或者getpeername()得到'struct sockaddr*',
		然后先获取'(ip+端口)字符串';
		最后反向求'对方的addrinfo{}结构体'.
		也就是只要连通了, 就能获取'对方的addrinfo{}结构体'



//2.getaddrinfo()函数API
	POSIX.1中, 定义的函数getaddrinfo(), 用来代替过时的gethostbyname()和gethostbyaddr().

	2.1:
	getaddrinfo()根据'host主机名字符串/URL'(ip),[如: www.baidu.com];
	和'service高层协议服务名'(port),[如: http = 80];
	引用返回一个addrinfo{}结构体链表[可以是:(ip地址 + 端口的模式),也可以是:(URL + 端口/高层协议名)]

	获取: getaddrinfo()直向返回值, 成功则返回0, 出错返回一个'非0的错误码'.
	int getaddrinfo(\
		const char* restrict host,\
		const char* restrict service,\
		const struct addrinfo* restrict hint,\
		struct addrinfo** restrict res);

	2.2:
	释放: freeaddrinfo()用来释放一个addrinfo结构体.
	void freeaddrinfo(sruct addrinfo* ai);

	2.3:
	失败: 如果getaddrinfo()失败了, 必须调用gai_strerror()将返回的'非0的错误码'转换成错误消息.
	const char* gai_strerror(int error);



//3.addrinfo 结构体(新型addr 地址信息存放结构体):
	struct addrinfo {
		int ai_flags;                // AI_PASSIVE, AI_CANONNAME; [标志]
		int ai_family;               // AF_xxx; [协议族]
		int ai_socktype;             // SOCK_xxx; [socket类型]
		int ai_protocol;             // 0 or IPPROTO_xxx for IPv4 and IPv6; [协议类型]
		socklen_t ai_addrlen;        // length of ai_addr; [ai_addr地址的长度]
		char *ai_canonname;          // ptr to canonical name for host; [主机对应的标准名称]
		struct sockaddr *ai_addr;    // ptr to socket address structure; [结构体对应的一个网络地址]
		struct addrinfo *ai_next;    // ptr to next structure in linked list; [指向下一个addrinfo结构体的指针]
	};
*/

