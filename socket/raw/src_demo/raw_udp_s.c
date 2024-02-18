//编译:
//		gcc -g3 ./raw_udp_s.c -o x

//以下是一个使用C语言编写的简单示例, 它展示了如何使用raw socket发送UDP报文: 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

extern unsigned short checksum (void *buf, int len);

int main (void)
{
	int sfd;
	struct sockaddr_in dest, src;
	struct ip ip_hdr;
	struct udphdr udp_hdr;
	const char data[] = "Hello, UDP!";
	char packet[sizeof (struct ip) + sizeof (struct udphdr) + sizeof (data)];

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_UDP);
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
	dest.sin_port = htons (53);											// 示例目标端口(例如: DNS端口)

	// 填充IP报头
	//memset (&ip_hdr, 0, sizeof (ip_hdr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;																// 5 * 4 = 20 bytes
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons (sizeof (struct ip) + sizeof (struct udphdr) + sizeof (data));
	ip_hdr.ip_id = htons (getpid());
	ip_hdr.ip_off = 0;
	ip_hdr.ip_ttl = 64;
	ip_hdr.ip_p = IPPROTO_UDP;
	ip_hdr.ip_sum = 0;
	ip_hdr.ip_src = src.sin_addr;
	ip_hdr.ip_dst = dest.sin_addr;

	// 计算并设置IP报头校验和
	ip_hdr.ip_sum = checksum ((unsigned short *) &ip_hdr, sizeof (ip_hdr));

	// 填充UDP报头
	memset (&udp_hdr, 0, sizeof (udp_hdr));
	udp_hdr.uh_sport = src.sin_port;
	udp_hdr.uh_dport = dest.sin_port;
	udp_hdr.uh_ulen = htons (sizeof (struct udphdr) + sizeof (data));
	udp_hdr.uh_sum = 0;

	// 复制数据到报文中
	memcpy (packet, &ip_hdr, sizeof (ip_hdr));
	memcpy (packet + sizeof (ip_hdr), &udp_hdr, sizeof (udp_hdr));
	memcpy (packet + sizeof (ip_hdr) + sizeof (udp_hdr), data, sizeof (data));

	// 计算并设置UDP校验和
	udp_hdr.uh_sum = checksum (packet + sizeof (ip_hdr), sizeof (udp_hdr) + sizeof (data));

	// 发送数据包
	if (sendto (sfd, packet, sizeof (packet), 0, (struct sockaddr *) &dest, sizeof (dest)) < 0)
	{
		perror ("sendto()");
		return -1;
	}

	printf ("UDP packet sent successfully!\n");

	close (sfd);
	return 0;
}
