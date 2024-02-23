//编译:
//		gcc -g3 ./cksum.c ./raw_udp_s.c -o x

//wireshark 过滤条件: ip.addr == 127.1.1.1
//wireshark: checksum unverified(未核实): 好像问题不大, 默认udp 数据报也是这样的, 同样也是checksum unverified

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include "cksum.h"

#define BUF_MAX (128)

int main(void){
	unsigned char buf_snd[BUF_MAX];
	struct sockaddr_in src, dest;
	int sfd = 0;
	struct udphdr *pUDP = (struct udphdr *)&buf_snd;
	const char data[] = "Hello, UDP!!";

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sfd < 0){
		perror("socket()");
		return -1;
	}

	src.sin_family = AF_INET;
	src.sin_addr.s_addr = inet_addr ("127.0.0.1");
	src.sin_port = htons (12345);

	dest.sin_family = AF_INET;
	dest.sin_port = 80;
	dest.sin_addr.s_addr = inet_addr("127.1.1.1");

	//填充 UDP 报头
	pUDP->uh_sport = src.sin_port;																	//源端口16bit 不需要htons()
	pUDP->uh_dport = htons (dest.sin_port);													//目的端口16bit 需要htons() -- 非常诡异
	pUDP->uh_ulen = htons (sizeof (struct udphdr) + sizeof (data));	//udp head + body 总长
	pUDP->uh_sum = 0;																								//checksum 初始化

	memcpy (buf_snd + sizeof (struct udphdr), data, sizeof (data));	//复制数据到 UDP 报文中

	pUDP->uh_sum = cksumEx (buf_snd, sizeof (struct udphdr) + sizeof (data), &src, &dest, 0);//计算 & 设置UDP 校验和 [wireshark: checksum unverified(未核实)]

	if(sendto (sfd, buf_snd,  sizeof (struct udphdr) + sizeof (data), 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0){
		perror("sendto()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}


