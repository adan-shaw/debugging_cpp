//AF_PACKET 协议簇简介:
/*
AF_PACKET 协议簇的本质:
	设置成AF_PACKET 选项后, 从网卡接收的数据包可以直接传送到应用程序而不经过内核处理;

总结:
	PF_INET + SOCK_RAW 的情况, ip 报文是可以自动填写的;
	需要手动填写ip 报文的情况:
		PF_INET + SOCK_PACKET 
		PF_PACKET + SOCK_RAW 
		PF_PACKET + SOCK_DGRAM 
	另外:
		无论是PF_INET/PF_PACKET, 
		无论是什么类型的套接字(raw/udp报式/tcp流式), 
		无论ip 报文是否填写, 
		无论4层协议是(tcp/udp/icmp/igmp/sctp), 
		io 函数统一都是:
			sendto()/send()
			recv()/recvfrom()
		修改socket 选项, 统一是:
			getsockopt()/setsockopt();
*/



#include <netpacket/packet.h>
// AF_PACKET 专用sockaddr, 与AF_INET 常用的struct sockaddr, struct sockaddr_in 有很大差别, 多了很多信息;
struct sockaddr_ll
{
	unsigned short int sll_family;	// 一般为AF_PACKET, 等价于sockaddr_in中的sa_family一样
	unsigned short int sll_protocol;// 上层协议, 一般是ETH_P_IP
	int sll_ifindex;								// 接口类型
	unsigned short int sll_hatype;	// 报头类型
	unsigned char sll_pkttype;			// 包类型
	unsigned char sll_halen;				// 地址长度
	unsigned char sll_addr[8];			// MAC地址
};

#include <linux/if_ether.h>
//sll_protocol 表示上层的协议类型:
#define ETH_P_LOOP 0x0060      // Ethernet Loopback packet(以太网回环网络协议)
#define ETH_P_PUP 0x0200       // Xerox PUP packet
#define ETH_P_PUPAT 0x0201     // Xerox PUP Addr Trans packet
#define ETH_P_IP 0x0800        // Internet Protocol packet(一般上层都是IP 协议)
#define ETH_P_X25 0x0805       // CCITT X.25
#define ETH_P_ARP 0x0806       // Address Resolution packet
#define ETH_P_BPQ 0x08FF       // G8BPQ AX.25 Ethernet Packet*/
#define ETH_P_IEEEPUP 0x0a00   // Xerox IEEE802.3 PUP packet
#define ETH_P_IEEEPUPAT 0x0a01 // Xerox IEEE802.3 PUP Addr Trans packet
#define ETH_P_DEC 0x6000       // DEC Assigned proto
#define ETH_P_DNA_DL 0x6001    // DEC DNA Dump/Load
#define ETH_P_DNA_RC 0x6002    // DEC DNA Remote Console
#define ETH_P_DNA_RT 0x6003    // DEC DNA Routing
#define ETH_P_LAT 0x6004       // DEC LAT
#define ETH_P_DIAG 0x6005      // DEC Diagnostics
#define ETH_P_CUST 0x6006      // DEC Customer use
#define ETH_P_SCA 0x6007       // DEC Systems Comms Arch
#define ETH_P_RARP 0x8035      // Reverse Addr Res packet
#define ETH_P_ATALK 0x809B     // Appletalk DDP
#define ETH_P_AARP 0x80F3      // Appletalk AARP
#define ETH_P_IPX 0x8137       // IPX over DIX
#define ETH_P_IPV6 0x86DD      // IPv6 over bluebook(IPv6 协议)
#define ETH_P_PPP_DISC 0x8863  // PPPoE discovery messages
#define ETH_P_PPP_SES 0x8864   // PPPoE session messages
#define ETH_P_ATMMPOA 0x884c   // MultiProtocol Over ATM
#define ETH_P_ATMFATE 0x8884   // Frame-based ATM Transport over Ethernet

#include <linux/netdevice.h>
//sll_ifindex 表示接口类型, 也可以选择:
enum {
	IF_PORT_UNKNOWN = 0,
	IF_PORT_10BASE2,
	IF_PORT_10BASET,  //基于双绞线的10M以太网
	IF_PORT_AUI,
	IF_PORT_100BASET, //基于双绞线的100M以太网(推荐)
	IF_PORT_100BASETX,
	IF_PORT_100BASEFX
};

#include <net/if_arp.h>
//sll_hatype ARP 硬件地址类型, 也可以选择:
#define ARPHRD_NETROM 0    // From KA9Q: NET/ROM pseudo.
#define ARPHRD_ETHER 1     // Ethernet 10/100Mbps. (一般选择这个)
#define ARPHRD_EETHER 2    // Experimental Ethernet.
#define ARPHRD_AX25 3      // AX.25 Level 2.
#define ARPHRD_PRONET 4    // PROnet token ring.
#define ARPHRD_CHAOS 5     // Chaosnet.
#define ARPHRD_IEEE802 6   // IEEE 802.2 Ethernet/TR/TB.
#define ARPHRD_ARCNET 7    // ARCnet.
#define ARPHRD_APPLETLK 8  // APPLEtalk.
#define ARPHRD_DLCI 15     // Frame Relay DLCI.
#define ARPHRD_ATM 19      // ATM.
#define ARPHRD_METRICOM 23 // Metricom STRIP (new IANA id).

//sll_pkttype 包含有效的分组类型:
/*
	目标地址是本地主机的分组用:                             PACKET_HOST       主机ip, 常用
	物理层广播分组用:                                     PACKET_BROADCAST  广播
	发送到一个物理层多路广播地址的分组用:                    PACKET_MULTICAST  多播
	在混杂(promiscuous)模式下的设备驱动器发向其他主机的分组用: PACKET_OTHERHOST  混杂模式 + 主机ip
	本源于本地主机的分组被环回到分组套接口用:                 PACKET_OUTGOING   回环网络下用的类型

	这些类型只对接收到的分组有意义;
	sll_addr 和 sll_halen 包括物理层(例如 IEEE 802.3)地址和地址长度;
	精确的解释依赖于设备;
*/

//sll_halen 为MAC地址长度(6 bytes)
#define ETH_ALEN 6         // Octets in one ethernet addr
#define ETH_HLEN 14        // Total octets in header.
#define ETH_ZLEN 60        // Min. octets in frame sans FCS
#define ETH_DATA_LEN 1500  // Max. octets in payload
#define ETH_FRAME_LEN 1514 // Max. octets in frame sans FCS

//sll_addr[8] 为目的MAC地址(按sockaddr_in.sin_addr为目的IP地址推测)







