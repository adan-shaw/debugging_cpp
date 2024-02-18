//编译:
//		gcc -g3 ./raw_tcp_r.c -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

#define BUF_SIZE (65536)

int main (void)
{
	int sfd, n;
	struct sockaddr_in addr_local, from_addr;
	char buffer[BUF_SIZE];
	socklen_t from_addr_len = sizeof (from_addr);
	struct ip *ip_hdr;
	struct tcphdr *tcp_hdr;

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	//memset (&addr_local, 0, sizeof (addr_local));
	addr_local.sin_family = AF_INET;
	//addr_local.sin_addr.s_addr = INADDR_ANY;					// 监听任意IP
	addr_local.sin_addr.s_addr = inet_addr("127.0.0.1");// 监听回环地址
	addr_local.sin_port = htons (0);										// 任意端口

	// 绑定套接字到本地地址
	if (bind (sfd, (struct sockaddr *) &addr_local, sizeof (addr_local)) < 0)
	{
		perror ("bind()");
		return -1;
	}

	printf ("Waiting for TCP packets...\n");

	// 循环接收数据包
	while (1)
	{
		n = recvfrom (sfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &from_addr, &from_addr_len);
		if (n < 0)
		{
			perror ("recvfrom()");
			continue;
		}

		// 解析IP和TCP报头
		ip_hdr = (struct ip *) (buffer + sizeof (struct ethhdr));
		tcp_hdr = (struct tcphdr *) (buffer + sizeof (struct ethhdr) + ip_hdr->ip_hl * 4);

		// 打印相关信息
		printf ("Received TCP packet from %s:%d to %s:%d\n", \ 
			inet_ntoa (from_addr.sin_addr), ntohs (tcp_hdr->th_dport), \ 
			inet_ntoa (ip_hdr->ip_dst), ntohs (tcp_hdr->th_sport));

		// 接收一个报文就退出 for test only
		break;
	}

	close (sfd);
	return 0;
}
