//编译:
//		gcc -g3 ./getaddrinfo.c -o x 



/*
//1.posix dns 查询的简述:
	getaddrinfo(), getnameinfo()两个函数都是posix dns查询函数;

	getaddrinfo()根据'URL字符串', 或者'(ip+端口)字符串'
		获取‘addrinfo{}结构体’里面的描述的信息.

	getnameinfo()根据'struct sockaddr*通用socket地址信息描述体',
		获取'(ip+端口)字符串'.


	'addrinfo{}结构体'是一个链表结构,
	每个'addrinfo{}结构体'都有一个'struct sockaddr*通用地址信息描述体'
	也就是:
		只要有URL字符串, 就可以获取到'目标URL的addrinfo{}结构体'.

		只要知道对方的'struct sockaddr*通用地址信息描述体',
			也可以先获取'(ip+端口)字符串';
			再根据'(ip+端口)字符串'+getaddrinfo()得到=>'addrinfo{}结构体'

		只要socket已经连通, 可以通过getsockname(),
		或者getpeername()得到'struct sockaddr*',
		然后先获取'(ip+端口)字符串';
		最后反向求'对方的addrinfo{}结构体'.
		也就是只要连通了, 就能获取'对方的addrinfo{}结构体'



//2.getaddrinfo()函数API
	POSIX.1中, 定义的函数getaddrinfo(),
	用来代替过时的gethostbyname()和gethostbyaddr().

	2.1:
	getaddrinfo()根据'host主机名字符串/URL'(ip),[如: www.baidu.com];
		和'service高层协议服务名'(port),[如: http = 80];
		引用返回一个addrinfo{}结构体链表.
		[可以是:(ip地址 + 端口的模式),也可以是：(URL + 端口/高层协议名)]

	getaddrinfo()直向返回值, 成功则返回0, 出错返回一个'非0的错误码'.
	int getaddrinfo(\
		const char* restrict host,\
		const char* restrict service,\
		const struct addrinfo* restrict hint,\
		struct addrinfo** restrict res);//获取

	2.2:
	freeaddrinfo()用来释放一个addrinfo结构体.
	void freeaddrinfo(sruct addrinfo* ai);//释放

	2.3:
	如果getaddrinfo()失败了,
	必须调用gai_strerror()将返回的'非0的错误码'转换成错误消息:
	const char* gai_strerror(int error);



//3.addrinfo 结构体:
	struct addrinfo{
		int ai_flags;								// Input flags.
		int ai_family;							// Protocol family for socket.
		int ai_socktype;						// Socket type.
		int ai_protocol;						// Protocol for socket.
		socklen_t ai_addrlen;				// Length of socket address.
		struct sockaddr *ai_addr;		// Socket address for socket.
		char *ai_canonname;					// Canonical name for service location.
		struct addrinfo *ai_next;		// Pointer to next in list.
	};
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>



//1.打印单个struct addrinfo*结构体
void print_addrinfo(const struct addrinfo*);

//2.getaddrinfo()测试函数
void getaddrinfo_test(void);



int main(void){
	getaddrinfo_test();
	return 0;
}



//1.打印单个struct addrinfo*结构体
void print_addrinfo(const struct addrinfo* addr){
	char str[INET6_ADDRSTRLEN];
	short port;
	struct sockaddr_in* in4;
	struct sockaddr_in6* in6;



	//1.打印协议簇类型
	//	顺便根据协议簇类型, 选择ipv4 or ipv6进行‘socket地址信息转换’
	printf("ai_family: ");
	switch (addr->ai_family){
		case AF_INET:
			printf("ipv4\n");
			//ipv4 socket 地址信息提取
			in4 = (struct sockaddr_in *)(addr->ai_addr);
			port = ntohs(in4->sin_port);											//提取port
			inet_ntop(AF_INET,&in4->sin_addr,str,sizeof(str));//带截断转换ip
			break;
		case AF_INET6:
			printf("ipv6\n");
			in6 = (struct sockaddr_in6 *)(addr->ai_addr);
			port = ntohs(in6->sin6_port);
			inet_ntop(AF_INET6,&in6->sin6_addr,str,sizeof(str));
			break;
		default:
			printf("Unknown\n");
			break;
	}


	//2.打印socket 类型
	printf("ai_socktype: ");
	switch (addr->ai_socktype){
		case SOCK_STREAM:			printf("stream-TCP\n");			break;
		case SOCK_DGRAM:			printf("dgram-UDP\n");			break;
		case SOCK_RAW:				printf("raw-IP/ICMP/IGMP\n");break;
		case SOCK_SEQPACKET:	printf("seqpacket-SCTP\n");	break;
		case SOCK_PACKET:			printf("packet-PACKET\n");	break;
		default:							printf("others\n");					break;
	}


	//3.打印'传输协议'类型
	printf("protocol: ");
	switch(addr->ai_protocol){
		case IPPROTO_IP:		printf("ip\n");			break;
		case IPPROTO_TCP:		printf("tcp\n");		break;
		case IPPROTO_UDP:		printf("udp\n");		break;
		case IPPROTO_RAW:		printf("raw\n");		break;
		case IPPROTO_SCTP:	printf("sctp\n");		break;
		case IPPROTO_ICMP:	printf("icmp\n");		break;
		case IPPROTO_IGMP:	printf("igmp\n");		break;
		default:						printf("others\n");	break;
	}


	//4.打印地址, 端口等其它信息
	printf("address: %s\n", str);			//打印之前存储的ip 字符串
	printf("port: %d\n", port);				//打印之前存储的port
	printf("canonical name: %s\n\n", addr->ai_canonname);//规范名称服务位置

	return;
}



//2.getaddrinfo()测试函数
void getaddrinfo_test(void){
	//char* serv = "http";//也可以直接写'高层协议名'
	const char* serv = "80";							//可以直接写端口号
	const char* host = "www.baidu.com";		//可以直接写url链接(主机域名)
	//const char* host = "14.215.177.39";	//可以直接写ip 地址
	struct addrinfo hint,*res,*res_save;
	int tmp;



	//1.初始化struct addrinfo{} 实体
	bzero(&hint, sizeof(hint));
	//hint.ai_family = AF_UNSPEC;//初始化协议簇为0,自动化,未知(可省略)


	//2.getaddrinfo()
	tmp = getaddrinfo(host, serv, &hint, &res);
	if(tmp != 0){
		printf("Cannot get information for URL:%s%s\n\terror:%s\n\n",\
				serv, host, gai_strerror(tmp));
		return;
	}


	//3.保存addrinfo{}结果链表的首地址, 方便释放内存.
	res_save = res;


	//4.遍历打印addrinfo{}结果链表
	while(res != NULL){
		print_addrinfo(res);
		res = res->ai_next;
	}


	//5.释放内存
	freeaddrinfo(res_save);

	return;
}


