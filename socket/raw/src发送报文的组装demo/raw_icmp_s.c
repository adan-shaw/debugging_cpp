//编译:
//		gcc -g3 ./cksum.c ./raw_icmp_s.c -o x

//wireshark 过滤条件: ip.addr == 127.1.1.1

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include "cksum.h"

#define BUF_MAX (128)

int main(void){
	unsigned char buf_snd[BUF_MAX];
	struct sockaddr_in dest;
	int tmp, sfd = 0;
	struct icmp *pICMP = (struct icmp *)&buf_snd;//如果直接定义为struct icmp icmph; 然后直接sendto(sfd, &icmph), 或者先memcpy(), 再sendto(sfd, buf_snd), 都会失败!!
																							 //报错码: sendto(): Address family not supported by protocol [本demo 是唯一正解]
	pid_t pid = getpid();

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sfd < 0){
		perror("socket()");
		return -1;
	}

	dest.sin_family = AF_INET;
	dest.sin_port = htons (80);
	dest.sin_addr.s_addr = inet_addr("127.1.1.1");

	pICMP->icmp_type = ICMP_ECHO;		//ICMP回显请求
	pICMP->icmp_code = 0;						//code值为0
	pICMP->icmp_cksum = 0;					//初始化cksum
	pICMP->icmp_seq = 0;						//本报的序列号(一般是从0 开始累加的uint, 单发一次icmp, 可以为0)
	pICMP->icmp_id = pid & 0xffff;	//填写PID

	for(tmp = 0; tmp < sizeof(struct icmp); tmp++)
		pICMP->icmp_data[tmp] = tmp;	//填写icmp 数据(随机乱填)

	pICMP->icmp_cksum = cksum((unsigned char *)pICMP, sizeof(struct icmp));//计算&填写cksum

	if(sendto (sfd, buf_snd, sizeof(struct icmp), 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0){
		perror("sendto()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}


