//编译:
//		gcc MTU分析test.c -o x

#include <stdio.h>



//1.UDP / TCP 报头大小考虑
/*
	IP头(MAX=60字节) + [UDP头(max=8字节) / TCP头(max=60字节)]

	IP头加上选项最长60 字节, 最短20字节;
	附加选项40字节, 用来做ip 路由路径指定, 每一跳消耗32bit=4字节, 最高支持指定10跳;

	UDP头固定8 字节;

	TCP头加上选项最长60 字节, 最短20字节;
	附加选项40字节, 用来做窗口扩大, 时间戳等操作, 没有比TCP更长的传输控制协议了, ICMP, IGMP都没有TCP 报头长;
	[此时还不需要考虑ETH 报头]
	#ETH 报头, 固定28 字节(6字节 48bit的MAC 地址)

	ip+tcp报头, 最大容量是: 60+60=120字节, 加上ETH 是120+28=148 字节;
	ip+tcp报头, 最小容量是: 20+20=40字节, 加上ETH 是40+28=68 字节;
	ip+udp报头, 最大容量是: 60+8=68字节, 加上ETH 是68+28=96 字节;
	ip+udp报头, 最小容量是: 20+8=28字节, 加上ETH 是28+28=56 字节;

	所以, 在互联网写应用, udp 真的很节省!!
*/



//2.MTU 考虑
/*
	以太网封装的数据部分帧长为1500-46字节,最小长度为46字节,6+6+2+46+4,不足46字节,则填空字节补足;
	标准802.2 MTU:       1492字节;
	以太网802.3 MTU:     1500字节;
	标准802.2 最小单帧:   38字节;
	以太网802.3 最小单帧: 46字节;
	internet MTU:       576字节;

	(internet MTU 不用考虑ARP 报文的大小, 因为internet MTU肯定小于以太网MTU, 所以肯定不会被切片, 不用担心;
	 但MTU 包括了ip 报头, 所以会不会被切片为MTU 的粒度, 计算时, 还得把ip 报头也计算在内)
*/

//TCP/UDP 报文长度min, max
#define mtu_udp_min (28)
#define mtu_udp_max (68)
#define mtu_tcp_min (40)
#define mtu_tcp_max (120)

//inet 互联网MTU(ARP 报头是加插的, 在以太局域网中需要考虑, inet互联网中不需要考虑)
#define mtu_inet_udp_min (576-mtu_udp_max)
#define mtu_inet_udp_max (576-mtu_udp_min)
#define mtu_inet_tcp_min (576-mtu_tcp_max)
#define mtu_inet_tcp_max (576-mtu_tcp_min)

//以太网802.2 MTU
#define mtu_8022_udp_min (1492-28-mtu_udp_max)
#define mtu_8022_udp_max (1492-28-mtu_udp_min)
#define mtu_8022_tcp_min (1492-28-mtu_tcp_max)
#define mtu_8022_tcp_max (1492-28-mtu_tcp_min)

//以太网802.3 MTU
#define mtu_8023_udp_min (1500-28-mtu_udp_max)
#define mtu_8023_udp_max (1500-28-mtu_udp_min)
#define mtu_8023_tcp_min (1500-28-mtu_tcp_max)
#define mtu_8023_tcp_max (1500-28-mtu_tcp_min)

//单次发送的最大长度, 既然使用了限定MTU, 这里可以忽略
#define mtu_data_max_half (32767)			//IP协议,单次发送操作的max的一半: 2^16-1=65536-1, 一半为32768-1
#define mtu_data_max_err (65536)			//udp sendto() fail, errno = 90;(ip 包太大, sendto() 会报错!!)
#define mtu_data_max (65500)					//udp sendto() 单次发送操作的max(不能再大了, 再大请自己分次发送)





int main(void){
	printf("mtu_inet_udp_min=%d\n", mtu_inet_udp_min);
	printf("mtu_inet_udp_max=%d\n", mtu_inet_udp_max);
	printf("mtu_inet_tcp_min=%d\n", mtu_inet_tcp_min);
	printf("mtu_inet_tcp_max=%d\n", mtu_inet_tcp_max);

	printf("mtu_8022_udp_min=%d\n", mtu_8022_udp_min);
	printf("mtu_8022_udp_max=%d\n", mtu_8022_udp_max);
	printf("mtu_8022_tcp_min=%d\n", mtu_8022_tcp_min);
	printf("mtu_8022_tcp_max=%d\n", mtu_8022_tcp_max);

	printf("mtu_8023_udp_min=%d\n", mtu_8023_udp_min);
	printf("mtu_8023_udp_max=%d\n", mtu_8023_udp_max);
	printf("mtu_8023_tcp_min=%d\n", mtu_8023_tcp_min);
	printf("mtu_8023_tcp_max=%d\n", mtu_8023_tcp_max);
	return 0;
}


