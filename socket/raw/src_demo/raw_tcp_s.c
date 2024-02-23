//编译:
//		gcc -g3 ./cksum.c ./raw_tcp_s.c -o x

//wireshark 过滤条件: ip.addr == 127.1.1.1

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "cksum.h"

#define BUF_MAX (72)

unsigned long randomEx(unsigned int begin, unsigned int end){
	unsigned int uiTmp;
	uiTmp = (unsigned int)( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	return(unsigned long)rand_r(&uiTmp) % end;
}

int main(void){
	unsigned char buf_snd[BUF_MAX];
	struct sockaddr_in src, dest;
	int sfd = 0;
	struct tcphdr *pTCP = (struct tcphdr *)&buf_snd;
	const char data[] = "Hello, TCP!!";

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
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

	//制作tcp 紧急syn 请求链接, 可骚扰hacking
	pTCP->source = src.sin_port;								//源端口16bit 不需要htons()
	pTCP->dest = htons (dest.sin_port);					//目的端口16bit 需要htons() -- 非常诡异
	pTCP->seq = htonl(randomEx(0, 65535));			//序列号
	pTCP->ack_seq = htonl(randomEx(0, 65535));	//确认序列号
	pTCP->syn = 1;															//请求连接标志
	pTCP->urg = 1;															//紧急指针标志
	//pTCP->window = htons(randomEx(0, 65535));		//滑动窗口大小(可以为0)
	//pTCP->urg_ptr = htons(randomEx(0, 65535));	//紧急字段指针(可以为0)
	pTCP->window = 0;
	pTCP->urg_ptr = 0;
	pTCP->check = 0;
	pTCP->check = cksumEx (buf_snd, sizeof (struct tcphdr) + sizeof (data), &src, &dest, 1);

	if(sendto (sfd, buf_snd, sizeof(struct tcphdr) + sizeof (data), 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0){
		perror("sendto()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}


