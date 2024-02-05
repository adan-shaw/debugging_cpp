//编译:
//		gcc -g3 ./ipv6,4自适应.c -o x

//AF_UNSPEC 意味着:
/*
	函数返回的是适用于指定主机名和服务名且适合任何协议族的地址;
	如果某个主机既有AAAA记录(IPV6)地址, 同时又有A记录(IPV4)地址, 
	那么AAAA记录将作为sockaddr_in6结构返回, 而A记录则作为sockaddr_in结构返回;

	简单来说, AF_UNSPEC 只是一个自适应选项, 本质上还是AF_INET / AF_INET6;
	如果你要支持IPV4 / IPV6 混杂, 可以试试;
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define srv_ip "127.0.0.1"
#define srv_port "12345"



/*
	struct addrinfo {
		int ai_flags;             // AI_PASSIVE, AI_CANONNAME; [标志]
		int ai_family;            // AF_xxx; [协议族]
		int ai_socktype;          // SOCK_xxx; [socket类型]
		int ai_protocol;          // 0 or IPPROTO_xxx for IPv4 and IPv6; [协议类型]
		socklen_t ai_addrlen;     // length of ai_addr; [ai_addr地址的长度]
		char *ai_canonname;       // ptr to canonical name for host; [主机对应的标准名称]
		struct sockaddr *ai_addr; // ptr to socket address structure; [结构体对应的一个网络地址]
		struct addrinfo *ai_next; // ptr to next structure in linked list; [指向下一个addrinfo结构体的指针]
	};
*/
// 解析struct addrinfo{} 新型addr 结构体
void print_addrinfo(struct addrinfo *p){
	printf("ai_family: ");//打印协议簇
	switch(p->ai_family){
		//case AF_LOCAL:			printf("AF_LOCAL, tcp/udp\n"); break;//数值等价于AF_UNIX
		case AF_UNIX:				printf("AF_LOCAL, tcp/udp\n"); break;
		case AF_INET:				printf("AF_INET, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case AF_INET6:			printf("AF_INET6, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case AF_ROUTE:			printf("AF_ROUTE, tcp/udp/icmp/igmp/ip/eth\n"); break;
		//case AF_NETLINK:		printf("AF_NETLINK, tcp/udp/icmp/igmp/ip/eth\n"); break;//数值等价于AF_PACKET
		case AF_PACKET:			printf("AF_PACKET, tcp/udp/icmp/igmp/ip/eth\n"); break;
		case AF_PPPOX:			printf("AF_PPPOX\n"); break;
		case AF_CAN:				printf("AF_CAN\n"); break;
		case AF_BLUETOOTH:	printf("AF_BLUETOOTH\n"); break;
		default:						printf("default: another ai_family\n"); break;
	}

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
	return;
}



int cli (void)
{
	int sfd, tmp;
	struct addrinfo hints, *res;

	//ipv4 ipv6 自适应模式, 要先用getaddrinfo() 做过滤[即自我选择], 让编译器决定使用ipv4 or ipv6
	//memset (&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	//res 是返回的struct addrinfo 指针;
	//hints 是输入的struct addrinfo 参数实体
	tmp = getaddrinfo (srv_ip, srv_port, &hints, &res);
	if (tmp != 0)
	{
		printf ("getaddrinfo(): %s\n", gai_strerror (tmp));
		return -1;
	}

	print_addrinfo(res);//打印过滤结果(编译器会自适应, 选择ipv4 or ipv6)

	sfd = socket (res->ai_family, res->ai_socktype, 0);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	tmp = sendto (sfd, "hi,i'm cli()\n", 13, 0, res->ai_addr, res->ai_addrlen);
	if (tmp < 0)
	{
		perror ("sendto()");
		return -1;
	}

	close (sfd);

	freeaddrinfo (res);

	return 0;
}



int srv (void){
	int sfd, tmp;
	struct addrinfo hints, *res;
	char buf[1024];

	//memset (&hints, 0, sizeof (hints));
	//hints.ai_family = AF_INET;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	tmp = getaddrinfo (NULL, srv_port, &hints, &res);
	if (tmp != 0)
	{
		printf ("getaddrinfo():%s\n", gai_strerror (tmp));
		return -1;
	}

	print_addrinfo(res);

	sfd = socket (res->ai_family, res->ai_socktype, 0);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	if (bind (sfd, res->ai_addr, res->ai_addrlen) != 0)
	{
		perror ("bind()");
		close (sfd);
		return -1;
	}

	memset (buf, 0, sizeof (buf));
	tmp = recv (sfd, buf, sizeof (buf), 0);
	if (tmp < 0)
	{
		perror ("recv()");
		close (sfd);
		return -1;
	}
	printf ("data len=%d, %s\n", tmp, buf);

	close (sfd);

	freeaddrinfo (res);
	return 0;
}



int main(void){
	int pid = fork();
	if(pid == 0)
		srv();
	else{
		usleep(1000);
		cli();
	}
	return 0;
}
