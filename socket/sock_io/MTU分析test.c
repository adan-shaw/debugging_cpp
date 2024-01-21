//编译:
//		gcc MTU分析test.c -o x

#include <stdio.h>



//ARP 报头是加插的, 在局域网中需要考虑, 互联网中不需要考虑;
#define mtu_udp_min (28)
#define mtu_udp_max (68)
#define mtu_tcp_min (40)
#define mtu_tcp_max (120)

#define mtu_inet_udp_min (576-mtu_udp_min)
#define mtu_inet_udp_max (576-mtu_udp_max)
#define mtu_inet_tcp_min (576-mtu_tcp_min)
#define mtu_inet_tcp_max (576-mtu_tcp_max)



//以太网802.2 MTU
#define mtu_8022_udp_min (1492-28-mtu_udp_min)
#define mtu_8022_udp_max (1492-28-mtu_udp_max)
#define mtu_8022_tcp_min (1492-28-mtu_tcp_min)
#define mtu_8022_tcp_max (1492-28-mtu_tcp_max)



//以太网802.3 MTU
#define mtu_8023_udp_min (1500-28-mtu_udp_min)
#define mtu_8023_udp_max (1500-28-mtu_udp_max)
#define mtu_8023_tcp_min (1500-28-mtu_tcp_min)
#define mtu_8023_tcp_max (1500-28-mtu_tcp_max)



//单次发送的最大长度, 既然使用了限定MTU, 这里可以忽略
#define mtu_data_max_half (32767)			//IP协议,单次发送操作的max的一半:2^16-1=65536-1,一半为32768-1
#define mtu_data_max_err (65536)			//sendto() fail, errno = 90;
#define mtu_data_max (65500)					//udp 单次发送操作的max(不能再大了, 再大请自己分次发送)



//有多少个帧, 最大接收/发送的数据长度=MTU*frame_size_max;
#define frame_size_max (1024)



typedef struct{
	char* frame_pool[frame_size_max];
	int cur_pos;	//前n 个都是已使用frame				[每次使用前, 初始化为0]
	int len_max;	//数据总长(方便取出数据时使用)		[每次使用前, 初始化为0]
}frame_stack_t;	//不支持多线程使用, 每个线程都应在pth私有数据中, 存放一个frame_stack;



typedef struct{
	int num;			//帧编号(唯一, 必须为前置第一个元素, 重排时会用到)
	int len;			//本帧的数据长度
	char* data[mtu_inet_udp_min];
}frame_inet_udp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_inet_udp_max];
}frame_inet_udp_max_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_inet_tcp_min];
}frame_inet_tcp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_inet_tcp_max];
}frame_inet_tcp_max_t;



typedef struct{
	int num;			//帧编号(唯一, 必须为前置第一个元素, 重排时会用到)
	int len;			//本帧的数据长度
	char* data[mtu_8022_udp_min];
}frame_8022_udp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8022_udp_max];
}frame_8022_udp_max_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8022_tcp_min];
}frame_8022_tcp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8022_tcp_max];
}frame_8022_tcp_max_t;



typedef struct{
	int num;			//帧编号(唯一, 必须为前置第一个元素, 重排时会用到)
	int len;			//本帧的数据长度
	char* data[mtu_8023_udp_min];
}frame_8023_udp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8023_udp_max];
}frame_8023_udp_max_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8023_tcp_min];
}frame_8023_tcp_min_t;

typedef struct{
	int num;
	int len;
	char* data[mtu_8023_tcp_max];
}frame_8023_tcp_max_t;





int main(void){
	printf("%d\n", mtu_inet_udp_min);
	printf("%d\n", mtu_inet_udp_max);
	printf("%d\n", mtu_inet_tcp_min);
	printf("%d\n", mtu_inet_tcp_min);

	printf("%d\n", mtu_8022_udp_min);
	printf("%d\n", mtu_8022_udp_max);
	printf("%d\n", mtu_8022_tcp_min);
	printf("%d\n", mtu_8022_tcp_min);

	printf("%d\n", mtu_8023_udp_min);
	printf("%d\n", mtu_8023_udp_max);
	printf("%d\n", mtu_8023_tcp_min);
	printf("%d\n", mtu_8023_tcp_min);
	return 0;
}
