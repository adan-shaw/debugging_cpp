//编译:
//		gcc -g3 ./raw_icmp_r.c -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

#define BUF_SIZE (65536)

int main (void)
{
	int sfd,n;
	struct sockaddr_in addr_local, addr_from;
	char buffer[BUF_SIZE];
	socklen_t from_addr_len = sizeof (addr_from);
	struct ip *ip_hdr;
	struct icmp icmph;

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	//memset (&addr_local, 0, sizeof (addr_local));
	addr_local.sin_family = AF_INET;
	//addr_local.sin_addr.s_addr = INADDR_ANY;					// 监听任意IP
	addr_local.sin_addr.s_addr = inet_addr("127.0.0.1");// 监听回环地址
	addr_local.sin_port = 0;														// 不指定端口

	// 绑定套接字到本地地址(对于原始套接字, 这一步通常不是必须的, 但仍然可以执行)
	if (bind (sfd, (struct sockaddr *) &addr_local, sizeof (addr_local)) < 0)
	{
		perror ("bind()");
		return -1;
	}

	printf ("Waiting for ICMP packets...\n");

	// 循环接收数据包
	while (1)
	{
		n = recvfrom (sfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr_from, &from_addr_len);
		if (n < 0)
		{
			perror ("recvfrom()");
			continue;
		}

		// 解析IP和ICMP报头
		ip_hdr = (struct ip *) (buffer + sizeof (struct ethhdr));
		memcpy (&icmph, buffer + sizeof (struct ethhdr) + ip_hdr->ip_hl * 4, sizeof (struct icmp));

		// 验证ICMP类型是否为回显请求(ping请求)
		if (icmph.icmp_type == ICMP_ECHO)
		{
			printf ("Received ICMP Echo Request from %s\n", inet_ntoa (addr_from.sin_addr));

			// 可选: 发送ICMP回显应答(ping响应) -- 略

			// 接收一个报文就退出 for test only
			break;
		}
	}

	close (sfd);
	return 0;
}
