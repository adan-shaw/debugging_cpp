//编译:
//		gcc -g3 ./raw_icmp_s.c -o x

//以下是一个简单的示例, 它创建一个ICMP Echo请求(通常称为ping请求)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

extern unsigned short checksum (void *buf, int len);

int main (void)
{
	int sfd;
	struct sockaddr_in dest;
	struct ip ip_hdr;
	struct icmp icmp_hdr;
	char packet[sizeof (struct ip) + sizeof (struct icmp)];

	sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	// 填充目的地址的sockaddr_in (后面sendto 需要用)
	dest.sin_family = AF_INET;
	dest.sin_port = 0;
	dest.sin_addr.s_addr = inet_addr ("127.0.0.1");

	// 填充IP报头
	//memset (&ip_hdr, 0, sizeof (ip_hdr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;																// 5 x 4 bytes = 最短ip 报文头
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons (sizeof (struct ip) + sizeof (struct icmp));
	ip_hdr.ip_id = htons (getpid());								// 示例ID
	ip_hdr.ip_off = 0;
	ip_hdr.ip_ttl = 64;															// 示例TTL
	ip_hdr.ip_p = IPPROTO_ICMP;
	ip_hdr.ip_sum = 0;															// 稍后将计算
	ip_hdr.ip_src.s_addr = inet_addr ("127.0.0.1");	// 示例: 源IP
	ip_hdr.ip_dst.s_addr = dest.sin_addr.s_addr;

	// 计算并设置IP报头校验和
	ip_hdr.ip_sum = checksum (&ip_hdr, sizeof (ip_hdr));

	// 填充ICMP报头
	//memset (&icmp_hdr, 0, sizeof (icmp_hdr));
	icmp_hdr.icmp_type = ICMP_ECHO;
	icmp_hdr.icmp_code = 0;
	icmp_hdr.icmp_id = htons (getpid());						// 与IP报头匹配
	icmp_hdr.icmp_seq = htons (1);									// 序列号
	icmp_hdr.icmp_cksum = 0;												// 稍后将计算

	// 计算并设置ICMP校验和
	icmp_hdr.icmp_cksum = checksum (packet + sizeof (ip_hdr), sizeof (icmp_hdr));

	// 将报头复制到数据包中
	memcpy (packet, &ip_hdr, sizeof (ip_hdr));
	memcpy (packet + sizeof (ip_hdr), &icmp_hdr, sizeof (icmp_hdr));

	// 发送数据包
	if (sendto (sfd, packet, sizeof (packet), 0, (struct sockaddr *) &dest, sizeof (dest)) < 0)
	{
		perror ("sendto()");
		return -1;
	}

	printf ("Packet sent successfully!\n");

	close (sfd);
	return 0;
}
