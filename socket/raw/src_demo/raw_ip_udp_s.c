//编译:
//		gcc -g3 ./cksum.c ./raw_ip_udp_s.c -o x

//wireshark 过滤条件: ip.addr == 127.1.1.1
//wireshark: checksum unverified(未核实): 好像问题不大, 默认udp 数据报也是这样的, 同样也是checksum unverified

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "cksum.h"

#define BUF_MAX (256)

//逐个字符打印缓冲区data (跟wireshark 中看到的数据, 是一致的, 这样打印意义不大, 除非terminal 调试)
void print_buf(char *buf, unsigned int len){
	unsigned int tmp = 0;
	while(tmp < len)
		printf("%c", buf[tmp++]);
	printf("\n");
}

int main(void){
	unsigned char buf_snd[BUF_MAX];
	const char data[] = "Hello, UDP!!";
	struct sockaddr_in src, dest;
	int len_all, sfd = 0;
	struct ip *pIP = (struct ip *)&buf_snd;																//第一次赋值, 不需要移动指针, 直接赋值可以
																																				//第二次赋值, 需要移动指针;
	//struct udphdr *pUDP = (struct udphdr *)&buf_snd + sizeof(struct ip);//错误的指针移动赋值方式(弃用)
	struct udphdr *pUDP = (struct udphdr *)&buf_snd[sizeof(struct ip)];		//正确的指针移动赋值方式
	void *pdata = (void *)&buf_snd[sizeof(struct ip) + sizeof(struct udphdr)];
	

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sfd < 0){
		perror("socket()");
		return -1;
	}

	setsockopt(sfd, SOL_IP, IP_HDRINCL, "1", 1);//原始套接字RAW 默认不开启ip 协议报文的填充, 设置sfd 强制开启ip 报文填充

	src.sin_family = AF_INET;
	src.sin_addr.s_addr = inet_addr ("127.0.0.1");
	src.sin_port = htons (12345);

	dest.sin_family = AF_INET;										//16bit
	dest.sin_port = htons (80);										//16bit
	dest.sin_addr.s_addr = inet_addr("127.1.1.1");//32 bit

	//计算报文总长
	len_all = sizeof(struct ip) + sizeof(struct udphdr) + sizeof (data);

	//填充ip 报文
	pIP->ip_v = IPVERSION;							//4
	pIP->ip_hl = sizeof(struct ip) >> 2;//5 * 4 = 20bit (ip 报头长度, 这样描述可以应对ip 拓展报头) [这个数据是32bit 的, 不需要用htonl()?]
	pIP->ip_tos = 0;
	pIP->ip_len = htons (len_all);			//ip 报文总长度 = ip + (udp head + body 总长)
	pIP->ip_id = htons (getpid());
	pIP->ip_off = 0;										//htons(0), 全0 可以不用htons()
	pIP->ip_ttl = 64;
	pIP->ip_p = IPPROTO_UDP;
	pIP->ip_src.s_addr = src.sin_addr.s_addr;
	pIP->ip_dst.s_addr = dest.sin_addr.s_addr;
	pIP->ip_sum = 0;
	pIP->ip_sum = cksum (pIP, sizeof (struct ip));									// 计算 && 设置IP 报头校验和

	//填充 UDP 报头(udp 报文头部算不正确, 不知道为什么)
	pUDP->uh_sport = src.sin_port;																	//源端口16bit
	pUDP->uh_dport = dest.sin_port;																	//目的端口16bit
	pUDP->uh_ulen = htons (sizeof (struct udphdr) + sizeof (data));	//udp head + body 总长
	pUDP->uh_sum = 0;																								//checksum 初始化

	strncpy (pdata, data, sizeof (data));														//复制数据到 UDP 报文中

	pUDP->uh_sum = cksumEx (pUDP, sizeof (struct udphdr) + sizeof (data), &src, &dest, 0);//计算 & 设置UDP 校验和 [wireshark: checksum unverified(未核实)]

	//**********************************************debug*****************************************************
	//printf("sendto() data len_all = %d\n", len_all);//for test only
	//printf("%d, %d, %d\n",sizeof(struct ip), sizeof(struct udphdr), sizeof (data));//for test only
	//print_buf(buf_snd, len_all);//for test only (这样打印的意义不大, 不如在wireshark 中查看)
	//*** gdb 中直接打印buf_snd 缓冲区, 也很有效, 可以看到整个buf_snd 缓冲区的数据, 但缓冲区里面的杂乱数据, 也打印出来了 ***
	//**********************************************debug*****************************************************
	if(sendto (sfd, buf_snd, len_all , 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0){
		perror("sendto()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}


