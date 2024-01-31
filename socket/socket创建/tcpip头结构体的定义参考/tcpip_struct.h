/*
声明:
	tcp/ip 常用的'协议头结构体', 一般都定义在:
		<netinet/*.h>
		<arpa/*.h>
		<net/*.h>
	其它都太老旧, 废弃了!!

	新的硬件设备, '协议头结构体'需要自己定义!! 新设备由设备厂商自己定义, 随着时代进步, 会有更多变动!! 一般存放在<linux/*.h>

	新的互联网协议, '协议头结构体'需要自己定义!!
	但由于互联网协议都是char 为最小单元, 
	所以基本上不会用到tcp/ip 的底层解析, 有也是用原来的ip/tcp/udp, icmp 都用得很少, eth 用得更少了, 设备商采用eth;
	因此, 新的互联网协议, '协议头结构体'也是需要自己定义的!!


	因此, 这里只是提供一个模板, 给你定义'协议头结构体'的时候, 提供一些参考, 万一你要为设备商写eth 驱动程序呢?
	具体, 实际tcp/ip 是可以直接在	<netinet/*.h>, <arpa/*.h>, <net/*.h> 中找到你想要的'协议头结构体'的;
	你用那种协议, 就找哪个'协议头结构体'
*/

//依赖头文件
#include <netinet/in.h>



//声明'LITTLE_ENDIAN小端字节序'宏 (i386 is little endian)
#ifndef LITTLE_ENDIAN
	#define LITTLE_ENDIAN (1)
#endif

//声明'BIG_ENDIAN小端字节序'宏(只能选一个, 默认以小端为准)
/*
	#ifndef BIG-ENDIAN
		#define BIG-ENDIAN (1)
	#endif
*/





//IP报头(总长度20字节)
typedef struct _ip_hdr{
	#if LITTLE_ENDIAN
	unsigned int ihl:4;					//首部长度
	unsigned int version:4;			//版本
	#else
	unsigned int version:4;			//版本
	unsigned int ihl:4;					//首部长度
	#endif
	unsigned char tos;					//服务类型
	unsigned short tot_len;			//总长度(total len)
	unsigned short id;					//标志
	unsigned short frag_off;		//分片偏移(frag offset)
	unsigned char ttl;					//生存时间
	unsigned char protocol;			//协议
	unsigned short chk_sum;			//检验和
	struct in_addr srcaddr;			//源IP地址
	struct in_addr dstaddr;			//目的IP地址
}ip_hdr;


//TCP报头(总长度20字节)
typedef struct _tcp_hdr{
	unsigned short srcport;			//源端口号
	unsigned short dstport;			//目的端口号
	unsigned int seq;						//序列号
	unsigned int ack;						//确认号
	#if LITTLE_ENDIAN
	unsigned char reserved_1:4;	//保留6位中的4位首部长度
	unsigned char thl:4;				//tcp报头长度
	unsigned char flag:6;				//6位标志
	unsigned char reseverd_2:2;	//保留6位中的2位
	#else
	unsigned char thl:4;				//tcp报头长度
	unsigned char reserved_1:4;	//保留6位中的4位首部长度
	unsigned char reseverd_2:2;	//保留6位中的2位
	unsigned char flag:6;				//6位标志	
	#endif
	unsigned short wnd_size;		//16位窗口大小
	unsigned short chk_sum;			//16位TCP检验和
	unsigned short urg_ptr;			//16为紧急指针
}tcp_hdr;


//UDP报头(总长度8字节)
typedef struct _udp_hdr{
	unsigned short src_port;		//远端口号
	unsigned short dst_port;		//目的端口号
	unsigned short uhl;					//udp报头长度
	unsigned short chk_sum;			//16位udp检验和
}udp_hdr;


//tcp + ip生成的报body(单帧切片max = 576)
#define TCP_DATA_BUF_MAX (536)
typedef struct packet_tcp{
	ip_hdr ip;														//20
	tcp_hdr tcp;													//20
	unsigned char data[TCP_DATA_BUF_MAX];	//536
}TCP_IP;


//udp + ip生成的报body
#define UDP_DATA_BUF_MAX (536)
typedef struct packet_udp{
	ip_hdr ip;															//20
	udphdr udp;															//8
	unsigned char data[UDP_DATA_BUF_MAX];		//548
}UDP_IP;


#define TH_FIN (0x01)
#define TH_SYN (0x02)
#define TH_RST (0x04)
#define TH_PUSH (0x08)
#define TH_ACK (0x10)
#define TH_URG (0x20)





//
//icmp的各种形式:
//
// icmp 报body, 直接报头即够用了, 不需要组建data
//icmp报头(总长度4字节)
typedef struct _icmp_hdr{
	unsigned char icmp_type;		//类型
	unsigned char code;					//代码
	unsigned short chk_sum;			//16位检验和
}icmp_hdr;

//icmp报文(能到达目的地,响应-请求包)
struct icmp8{
	unsigned char icmp_type;		//type of message(报文类型)
	unsigned char icmp_code;		//type sub code(报文类型子码)
	unsigned short icmp_cksum;
	unsigned short icmp_id;
	unsigned short icmp_seq;
	char icmp_data[1];
};

//icmp报文(能返回目的地,响应-应答包)
struct icmp0{
	unsigned char icmp_type;		//type of message(报文类型)
	unsigned char icmp_code;		//type sub code(报文类型子码)
	unsigned short icmp_cksum;
	unsigned short icmp_id;
	unsigned short icmp_seq;
	char icmp_data[1];
};

//icmp报文(不能到达目的地)
struct icmp3{
	unsigned char icmp_type;		//type of message(报文类型)
	unsigned char icmp_code;		//type sub code(报文类型子码),例如:0网络原因不能到达,1主机原因不能到达
	unsigned short icmp_cksum;
	unsigned short icmp_pmvoid;
	unsigned short icmp_nextmtu;
	char icmp_data[1];
};

//icmp报文(重发结构体)
struct icmp5{
	unsigned char icmp_type;		//type of message(报文类型)
	unsigned char icmp_code;		//type sub code(报文类型子码)
	unsigned short icmp_cksum;
	struct in_addr icmp_gwaddr;
	char icmp_data[1];
};

struct icmp11{
	unsigned char icmp_type;		//type of message(报文类型)
	unsigned char icmp_code;		//type sub code(报文类型子码)
	unsigned short icmp_cksum;
	u_int icmp_void;
	char icmp_data[1];
};





// 标准802.2 MTU:       1492字节;
// 以太网802.3 MTU:     1500字节;
// arp 报body, 直接报头即够用了, 不需要组建data
#define ETHER_ADDR_LEN (6)								//NIC物理地址占6字节

//ether_type:
#define ETHERTYPE_IP (0x0800)							//IP Protocal
#define ETHERTYPE_ARP (0x0806)						//Address Resolution Protocal
#define ETHERTYPE_REVARP (0x0835)					//Reverse Address Resolution Protocal 逆地址解析协议

//ETH 以太网报头(总长度14字节)
typedef struct _eth_hdr{
	unsigned char dstmac[ETHER_ADDR_LEN];		//目标mac地址
	unsigned char srcmac[ETHER_ADDR_LEN];		//源mac地址
	unsigned short eth_type;								//以太网类型
}eth_hdr;


//arp 报头
typedef struct _arp_hdr{
	unsigned short ar_hrd;
	unsigned short ar_pro;
	unsigned char ar_hln;
	unsigned char ar_pln;
	unsigned short ar_op;
}arp_hdr;


//arp 报body
typedef struct _eth_arp{
	struct arp_hdr ea_hdr;
	unsigned char arp_sha[ETHER_ADDR_LEN];
	unsigned char arp_spa[4];
	unsigned char arp_tha[ETHER_ADDR_LEN];
	unsigned char arp_tpa[4];
}eth_arp;


#define arp_hrd (ea_hdr.ar_hrd)
#define arp_pro (ea_hdr.ar_pro)
#define arp_hln (ea_hdr.ar_hln)
#define arp_pln (ea_hdr.ar_pln)
#define arp_op (ea_hdr.ar_op)

#define ARPHRD (1)






