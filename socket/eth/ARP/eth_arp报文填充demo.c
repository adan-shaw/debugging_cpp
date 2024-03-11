//编译:
//		gcc -g3 -o x ./eth_arp报文填充demo.c

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>



//802.3 中的mac 地址 == 等价于 802.2 中的'等价于ar_tha'
unsigned char mac_addr[4][ETH_ALEN] = {
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},	//受限广播的mac 地址
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	//回环网络的mac 地址
	{0xc8, 0x3a, 0x35, 0x16, 0x93, 0xe0},	//本机请求加入DHCP 的host 接口的mac 地址
	{0xc8, 0x4a, 0x23, 0x18, 0x13, 0xa0}	//DHCP 路由器host 接口的mac 地址
};

//802.2 中的'源ip 地址'
unsigned char ip_addr[4][32] = {
	"255.255.255.255",										//受限广播的ip 地址
	"127.0.0.1",													//回环网络的ip 地址
	"192.168.0.102",											//本机请求加入DHCP 的host 接口的ip 地址
	"192.168.0.1"													//DHCP 路由器host 接口的host 接口的ip 地址
};

//* IEEE 802.3 协议头(已在if_ether.h 中声明, 不需要重新定义)
//struct ethhdr;

//*2.打包IEEE 802.2 协议头(if_ether.h 中没有'IEEE 802.2 协议头'声明, 在这里定义):
struct arp8022
{
	unsigned short ar_hrd;								//arp 硬件类型
	unsigned short ar_pro;								//arp 协议类型(与802.3 中的一致)
	unsigned char ar_hln;									//硬件地址长度
	unsigned char ar_pln;									//协议地址长度
	unsigned short ar_op;									//arp 操作码
	unsigned char ar_sha[ETH_ALEN];				//发送方mac 地址(与802.3 中的一致)
	unsigned char ar_sip[4];							//发送方ip 地址
	unsigned char ar_tha[ETH_ALEN];				//接收方mac 地址(与802.3 中的一致)
	unsigned char ar_tip[4];							//接收方ip 地址
};



//填写arp 请求报文802.2
void pickup_arp_ask8022(unsigned char *packet){
	//unsignedchar packet[ETH_FRAME_LEN];	//定义帧缓冲区(802.3 max = 1514)
	struct arp8022 *p = (struct arp8022 *)packet;

	//填充eth_8022
	p->ar_hrd = htons (0x01);							//arp 硬件类型(1 表示以太网)
	p->ar_pro = htons (0x0806);						//802.2 arp 请求
	p->ar_hln = 6;												//硬件地址长度(恒值)
	p->ar_pln = 4;												//协议地址长度(恒值)
	p->ar_op = htons (0x01);							//1=ARP 请求
	memcpy(p->ar_tha, mac_addr[0], ETH_ALEN);		//接收方mac 目的地址(arp 请求是广播出去, 所以目的mac 地址等于全部0xff)
	memcpy(p->ar_sha, mac_addr[2], ETH_ALEN);		//发送方mac 源地址
	memcpy(p->ar_tip, ip_addr[0], ETH_ALEN);		//接收方ip 目的地址(广播ip地址)
	memcpy(p->ar_sip, ip_addr[2], ETH_ALEN);		//发送方ip 源地址

	return;
}

//填写arp 请求报文802.3
void pickup_arp_ask8023(unsigned char *packet){
	struct ethhdr *p = (struct ethhdr *)packet;

	//填充eth_8023
	p->h_proto = htons (0x0806);								//802.3 arp 请求
	memcpy(p->h_dest, mac_addr[0], ETH_ALEN);		//接收方mac 目的地址(arp 请求是广播出去, 所以目的mac 地址等于全部0xff)
	memcpy(p->h_source, mac_addr[2], ETH_ALEN);	//发送方mac 源地址

	//ip 报文和其他报文, 自己填充
	return;
}



//填写arp 应答报文802.2
void pickup_arp_answer8022(unsigned char *packet){
	struct arp8022 *p = (struct arp8022 *)packet;

	p->ar_hrd = htons (0x01);							//arp 硬件类型(1 表示以太网)
	p->ar_pro = htons (0x8035);						//802.2 arp 应答
	p->ar_hln = 6;												//硬件地址长度(恒值)
	p->ar_pln = 4;												//协议地址长度(恒值)
	p->ar_op = htons (0x02);							//2=ARP 应答
	memcpy(p->ar_tha, mac_addr[2], ETH_ALEN);		//接收方mac 目的地址
	memcpy(p->ar_sha, mac_addr[3], ETH_ALEN);		//发送方mac 源地址
	memcpy(p->ar_tip, ip_addr[2], ETH_ALEN);		//接收方ip 目的地址(arp 应答目标ip = 主机)
	memcpy(p->ar_sip, ip_addr[3], ETH_ALEN);		//发送方ip 源地址(arp 应答源ip = 路由器)

	return;
}

//填写arp 应答报文802.3
void pickup_arp_answer8023(unsigned char *packet){
	struct ethhdr *p = (struct ethhdr *)packet;

	p->h_proto = htons (0x8035);								//802.2 arp 应答
	memcpy(p->h_dest, mac_addr[2], ETH_ALEN);		//接收方mac 目的地址(arp 应答目标ip = 主机)
	memcpy(p->h_source, mac_addr[3], ETH_ALEN);	//发送方mac 源地址(arp 应答源ip = 路由器)

	//ip 报文和其他报文, 自己填充
	return;
}


int main(void){
	return 0;
}
