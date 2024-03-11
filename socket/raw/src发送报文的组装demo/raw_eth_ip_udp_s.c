//编译:
//		gcc -g3 ./cksum.c ./raw_eth_ip_udp_s.c -o x

//wireshark 过滤条件: ip.addr == 127.1.1.1
//wireshark: checksum unverified(未核实): 好像问题不大, 默认udp 数据报也是这样的, 同样也是checksum unverified

//使用bsd socket 的方式, 发送ETH 报文, 实在太复杂难搞, 发送ETH 报文, 可能用libpcap 会方便很多!!
//使用bsd socket 的方式, 一旦出错, wireshark 完全收不到任何信息, 一条报文都没有!! 空白!! 调试不知道从何入手!! 
//(虽然编译没问题, 程序执行没错误, 但网络数据不知道发到哪里去了, 估计是MAC 地址的问题, IP+UDP 报文都是验证过, 不会出错的)

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if_arp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/if.h>//struct ifreq {}
#include <sys/ioctl.h>
#include "cksum.h"

#define BUF_MAX (256)

int main(void){
	unsigned char buf_snd[BUF_MAX];
	const char data[] = "Hello, UDP!!";
	struct sockaddr_ll src, dest;
	struct ifreq ifr;
	const char *iface = "lo";
	int ifindex, len_all, sfd = 0;
	struct ether_header *pETH = (struct ether_header *)&buf_snd;
	struct ip *pIP = (struct ip *)&buf_snd[sizeof(struct ether_header)];
	struct udphdr *pUDP = (struct udphdr *)&buf_snd[sizeof(struct ether_header) + sizeof(struct ip)];
	void *pdata = (void *)&buf_snd[sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct udphdr)];
	

	sfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_UDP);
	if(sfd < 0){
		perror("socket()");
		return -1;
	}

	//setsockopt(sfd, SOL_IP, IP_HDRINCL, "1", 1);//AF_PACKET 协议簇默认需要填写全部报文

	//获取接口索引
	strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
	if(ioctl(sfd, SIOCGIFINDEX, &ifr) < 0){
		perror("ioctl()");
		close(sfd);
		return -1;
	}
	else
		ifindex = ifr.ifr_ifindex;

	//填充struct sockaddr_ll {}
	src.sll_family = AF_PACKET;
	src.sll_protocol = ETH_P_IP;
	src.sll_ifindex = ifindex;//接口索引int 
	//src.sll_hatype = ARPHRD_ETHER;
	//src.sll_pkttype = PACKET_HOST;
	src.sll_halen = ETH_ALEN;
	memset(src.sll_addr,0,ETH_ALEN);

	dest.sll_family = AF_PACKET;
	dest.sll_protocol = ETH_P_IP;
	dest.sll_ifindex = ifindex;//接口索引int 
	//dest.sll_hatype = ARPHRD_ETHER;
	//dest.sll_pkttype = PACKET_HOST;
	dest.sll_halen = ETH_ALEN;
	memset(dest.sll_addr,0,ETH_ALEN);

	//填充ETH 报文
	memset(pETH->ether_dhost,0,ETH_ALEN);	//全0 即127.0.0.1 的MAC 地址
	memset(pETH->ether_shost,0,ETH_ALEN);
	pETH->ether_type = ETH_P_IP;					//type = ip 报文

	//计算报文总长
	len_all = sizeof(struct ip) + sizeof(struct udphdr) + sizeof (data);

	//填充ip 报文
	pIP->ip_v = IPVERSION;							//4
	pIP->ip_hl = sizeof(struct ip) >> 2;//5 * 4 = 20bit (ip 报头长度, 这样描述可以应对ip 拓展报头)
	pIP->ip_tos = 0;
	pIP->ip_len = htons (len_all);			//ip 报文总长度 = ip + (udp head + body 总长)
	pIP->ip_id = htons (getpid());
	pIP->ip_off = 0;										//htons(0), 全0 可以不用htons()
	pIP->ip_ttl = 64;
	pIP->ip_p = IPPROTO_UDP;
	pIP->ip_src.s_addr = inet_addr ("127.0.0.1");
	pIP->ip_dst.s_addr = inet_addr("127.0.0.1");
	pIP->ip_sum = 0;
	pIP->ip_sum = cksum ((unsigned char *)pIP, sizeof (struct ip));	// 计算 && 设置IP 报头校验和

	//填充 UDP 报头(udp 报文头部算不正确, 不知道为什么)
	pUDP->uh_sport = htons (12345);																	//源端口16bit
	pUDP->uh_dport = htons (80);																		//目的端口16bit
	pUDP->uh_ulen = htons (sizeof (struct udphdr) + sizeof (data));	//udp head + body 总长
	pUDP->uh_sum = 0;																								//checksum 初始化

	strncpy (pdata, data, sizeof (data));														//复制数据到 UDP 报文中

	//计算 & 设置UDP 校验和 [wireshark: checksum unverified(未核实)]
	pUDP->uh_sum = cksumEx ((unsigned char *)pUDP, sizeof (struct udphdr) + sizeof (data), pIP->ip_src.s_addr, pIP->ip_dst.s_addr, 0);

	if(sendto (sfd, buf_snd, len_all, 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_ll)) < 0){
		perror("sendto()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}


