//编译:
//		gcc -g3 ./raw_udp_r.c -o x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

#define BUF_SIZE (65536)

int main (void)
{
	int sfd, n, head_len;
	struct sockaddr_in addr_local, addr_from;
	char buffer[BUF_SIZE];
	socklen_t from_addr_len = sizeof (addr_from);
	struct ip *ip_hdr;
	struct udphdr *udp_hdr;

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_UDP);
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

	printf ("Waiting for UDP packets...\n");

	// 循环接收数据包
	while (1)
	{
		n = recvfrom (sfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr_from, &from_addr_len);
		if (n < 0)
		{
			perror ("recvfrom()");
			continue;
		}

		// 解析IP和UDP报头
		ip_hdr = (struct ip *) (buffer + sizeof (struct ethhdr));
		udp_hdr = (struct udphdr *) (buffer + sizeof (struct ethhdr) + ip_hdr->ip_hl * 4);

		// 打印相关信息
		printf ("Received UDP packet from %s:%d to %s:%d\n", \ 
			inet_ntoa (addr_from.sin_addr), ntohs (udp_hdr->uh_sport), \ 
			inet_ntoa (ip_hdr->ip_dst), ntohs (udp_hdr->uh_dport));

		//计算udp 报头长度(得到的ip_hdr->ip_hl 报文大小为0, bug, 需要调整一下)
		head_len = sizeof (struct ethhdr) + ip_hdr->ip_hl * 4 + sizeof (struct udphdr);
		printf ("UDP head lenght: %d+%d+%d=%d\n", sizeof (struct ethhdr), ip_hdr->ip_hl * 4, sizeof (struct udphdr), head_len);

		// 打印负载数据(如果有的话)
		if (n > head_len)
		{
			printf ("Payload: ");
			fwrite (buffer + head_len, 1, n - head_len, stdout);//切片输出到stdout, 粒度为1, 长度为n - (head_len)
			printf ("\n");
		}

		// 接收一个报文就退出 for test only
		break;
	}

	close (sfd);
	return 0;
}
