//编译:
//		gcc -g3 ./raw_tcp_s.c -o x

//此示例将展示如何设置TCP报头并发送一个简单的SYN报文

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

extern unsigned short checksum (void *buf, int len);

int main (void)
{
	int sfd;
	struct sockaddr_in dest, src;
	struct ip ip_hdr;
	struct tcphdr tcp_hdr;
	char packet[sizeof (struct ip) + sizeof (struct tcphdr)];

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	// 填充目的地址的sockaddr_in (后面sendto 需要用)
	//memset (&src, 0, sizeof (src));
	src.sin_family = AF_INET;
	src.sin_addr.s_addr = inet_addr ("127.0.0.1");	// 示例源IP
	src.sin_port = htons (12345);										// 示例源端口

	//memset (&dest, 0, sizeof (dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr ("127.0.0.1");	// 示例目标IP
	dest.sin_port = htons (80);											// 示例目标端口(例如: HTTP端口)

	// 填充IP报头
	//memset (&ip_hdr, 0, sizeof (ip_hdr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;																// 5 * 4 = 20 bytes
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons (sizeof (struct ip) + sizeof (struct tcphdr));
	ip_hdr.ip_id = htons (getpid());
	ip_hdr.ip_off = 0;
	ip_hdr.ip_ttl = 64;
	ip_hdr.ip_p = IPPROTO_TCP;
	ip_hdr.ip_sum = 0;
	ip_hdr.ip_src = src.sin_addr;
	ip_hdr.ip_dst = dest.sin_addr;

	// 计算并设置IP报头校验和
	ip_hdr.ip_sum = checksum ((unsigned short *) &ip_hdr, sizeof (ip_hdr));

	// 填充TCP报头
	memset (&tcp_hdr, 0, sizeof (tcp_hdr));
	tcp_hdr.th_sport = src.sin_port;
	tcp_hdr.th_dport = dest.sin_port;
	tcp_hdr.th_seq = htonl (getpid() + 123456789);	// 初始序列号
	tcp_hdr.th_ack = 0;															// 无确认号
	tcp_hdr.th_off = sizeof (struct tcphdr) / 4;		// TCP头大小
	tcp_hdr.th_flags = TH_SYN;											// SYN标志位
	tcp_hdr.th_win = htons (65535);									// 窗口大小
	tcp_hdr.th_urp = 0;															// 紧急指针
	tcp_hdr.th_sum = 0;															// 校验和初始化为0

	// 计算并设置TCP校验和
	tcp_hdr.th_sum = checksum ((unsigned short *) (packet + sizeof (struct ip)), sizeof (struct tcphdr));

	// 复制IP和TCP报头到数据包中
	memcpy (packet, &ip_hdr, sizeof (ip_hdr));
	memcpy (packet + sizeof (ip_hdr), &tcp_hdr, sizeof (tcp_hdr));

	// 发送数据包
	if (sendto (sfd, packet, sizeof (packet), 0, (struct sockaddr *) &dest, sizeof (dest)) < 0)
	{
		perror ("sendto()");
		return -1;
	}

	printf ("TCP SYN packet sent successfully!\n");

	close (sfd);
	return 0;
}
