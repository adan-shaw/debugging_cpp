//编译:
//		gcc -g3 ./addr地址信息转换.c -o x 



//1.网络地址'转换函数'list
/*
	需要你自己保证"输入字符串"的可靠性, 
	所有网络地址'转换函数'都不保证返回值的可靠性(错误ip字符串, 会返回错误的转换值):
	int						inet_aton(const char *cp, struct in_addr *inp);
	in_addr_t			inet_addr(const char *cp);
	in_addr_t			inet_network(const char *cp);

	char*					inet_ntoa(struct in_addr in);

	struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host);
	in_addr_t			inet_lnaof(struct in_addr in);
	in_addr_t			inet_netof(struct in_addr in);

	//新型转换函数
	const char *	inet_ntop(int af, const void *src, char *dst,\
													 socklen_t size);
	int						inet_pton(int af, const char *src, void *dst);
*/



//2.网络地址'结构体'&'数据类型':
/*
	in_addr_t = uint32_t;	//uint32_t 定义在<stdint.h>
	socklen_t = uint32;		//uint32   定义在<stdint.h>
	in_port_t = uint16_t	//uint16_t 定义在<stdint.h>

	struct sockaddr_un;	//unix socket 特有(略)
	struct sockaddr_in;	//inet4 socket 特有
	struct in_addr;			//inet4 socket 结构体的成员之一
	struct sockaddr;		//通用socket 地址描述结构体

	struct sockaddr{
		unsigned char sa_len;		// total length
		sa_family_t sa_family;	// address family
		char sa_data[14];				// variable-length address
	};
	struct sockaddr_in{
		sa_family_t sin_family;	// address_family
		in_port_t sin_port;			// port number
		struct in_addr sin_addr;// IPv4 address
	};
	struct in_addr{
		in_addr_t s_addr;				//IPv4 address
	};


	由于'网络地址格式'与'特定的通信域'相关,
	为了使得'不同格式的地址'都能够传入'套接字'相关的函数,
	所有的地址必须先被强制转换为一个'通用的地址结构'struct sockaddr;


	注意:
		struct in_addr 虽然是ulong, 但不得直接'='号赋值,
		struct in_addr 不是广义上的unsigned long int,
		而是4 个uchar 描述0.0.0.0 的地址信息.

		struct in_addr 也是一个结构体, 如果你非要赋值, 只有一种是有效的:
				//表示0.0.0.0
				addr_in.sin_addr.s_addr = 0;
		否则只能用内存'cp拷贝'赋值.
		这种意图也不明确的'cp拷贝'赋值,尽量不要用:
			uint32_t uip;
			memcpy(&addr_in.sin_addr.s_addr, &uip, sizeof(uint32_t));


	inet4地址结构体, 拷贝到'通用sockaddr地址描述结构体', 可以直接拷贝:
		struct sockaddr_in addr_in;
		struct sockaddr addr;
		memcpy(&addr,&addr_in,sizeof(struct sockaddr));
*/



//3.杂谈
/*
	网络地址信息载体struct sockaddr_in, struct sockaddr等结构体,
	跟socket fd没有半毛钱关系!!
	这些结构体只是为了描述网络地址信息和端口,
	而socket fd则是文件系统属性的融合, 是一个int 文件描述符;

	当然socket fd 继承了部分文件描述符属性,
	具体继承了多少, 看网卡驱动的实现情况,
	具体会有一个网卡通用标准, 来规范socket fd的文件描述符属性的部分;

	socket fd 并不能反向获取<网络地址信息载体>,
	因为socket创建的时候,只是一个sfd文件描述符,并不具备socket传输属性,
	当<链接建立>之后,你能够通过getsocketopt()来获取对方的一些地址信息,
	实在找不到, 你还能通过getsocketopt() 获取ip 头, 肯定可以解决的;
*/



#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//0.
void inet_addr_basic(void);

//1.
void inet_network_demo(void);

//2.
void inet_addr_demo(void);

//3.
void inet_aton_demo(void);

//4.
void inet_ntoa_demo(void);

//5.inet_pton()(线程安全版)
void inet_pton_demo(void);

//6.inet_ntop()(线程安全版)
void inet_ntop_demo(void);



int main(void){
	//1.基础demo
	inet_addr_basic();

	//2.旧式转换函数demo
	inet_addr_demo();
	inet_network_demo();
	inet_aton_demo();
	inet_ntoa_demo();

	//3.新型转换(线程安全版本)
	inet_pton_demo();
	inet_ntop_demo();
	return 0;
}



//0.
void inet_addr_basic(void){
	const char *ip = "192.168.0.1";
	unsigned short port = 9213;
	struct sockaddr_in addr_in;
	struct sockaddr addr;



	//struct sockaddr_in 结构体填充demo
	bzero(&addr_in,sizeof(addr_in));				//bzero 是一个全部置'\0'的函数
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr(ip);//字符串转in_addr_t = uint32_t
	addr_in.sin_port = htons(port);					//网络字节序转换=uint16_t

	//inet4地址结构体, 拷贝到'通用sockaddr地址描述结构体', 直接拷贝
	memcpy(&addr,&addr_in,sizeof(struct sockaddr));


	//打印转换后的情况, 是乱码
	printf("inet_addr(%s)=%d=uint32_t\n\
		htons(%d)=%d=uint16_t\n\n",\
		ip,addr_in.sin_addr.s_addr,port,addr_in.sin_port);

	printf("struct sockaddr_in:\n\
		[%d](inet4协议簇), [%d](地址指针值=整形式0.0.0.0)\n\
		[%d](网络字节序), [%d](整形式0.0.0.0)\n",\
		addr_in.sin_family,addr_in.sin_addr,\
		addr_in.sin_port,addr_in.sin_addr.s_addr);

	printf("struct sockaddr:\n\
		[%d](inet4协议簇), [%s](无字符串结束符)\n",addr.sa_family,addr.sa_data);
	printf("\n\n");
}



//1.inet_network()'网络地址'转换函数(需要你自己保证"输入字符串"的可靠性,这个函数api不保证可靠性)
void inet_network_demo(void){
	ulong uip;


	//正确使用demo:
	uip = inet_network("10.0.0.0");
	printf("inet_network(\"10.0.0.0\") = %d\n",uip);

	printf("\n\n");
}



//2.测试inet_addr()'主机地址'转换函数(需要你自己保证"输入字符串"的可靠性,这个函数api不保证可靠性)
void inet_addr_demo(void){
	ulong uip;


	//正确使用demo:
	uip = inet_addr("10.0.0.0");
	printf("inet_addr(\"10.0.0.0\") = %d\n",uip);

	printf("\n\n");
}



//3.inet_aton() '通用地址'转换函数:
//	成功返回1, 失败返回0, 结果需要引用返回(但没办法保证, 地址是否正确)
void inet_aton_demo(void){
	struct sockaddr_in addr_in;
	ulong uip;


	//正确使用demo:
	uip = inet_aton("10.0.0.0",&addr_in.sin_addr);
	printf("ret = %d\n",uip);
	printf("inet_aton(\"10.0.0.0\") = %d\n",addr_in.sin_addr.s_addr);

	printf("\n\n");
}



//4.inet_ntoa() '通用地址'反向转换函数
//	成功返回1, 失败返回0, 结果需要引用返回(但没办法保证, 地址是否正确)
void inet_ntoa_demo(void){
	struct sockaddr_in addr_in,addr_in2;
	ulong uip;
	char ip_tmp[16];
	char* ret;


	//struct sockaddr_in 结构体填充demo
	bzero(&addr_in,sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr("192.168.1.101");
	addr_in.sin_port = htons(666);

	//正确使用demo:
	strncpy(ip_tmp,inet_ntoa(addr_in.sin_addr),16);
	printf("  inet_ntoa(%d)=%s\n",addr_in.sin_addr,ip_tmp);

	//struct sockaddr_in 同类拷贝赋值
	memcpy(&addr_in2,&addr_in,sizeof(struct sockaddr_in));

	//struct sockaddr_in 与unsigned long 拷贝赋值
	uip = inet_addr("192.168.0.74");
	memcpy(&addr_in.sin_addr, &uip, 4);
	uip = inet_addr("211.168.6.64");
	memcpy(&addr_in2.sin_addr, &uip, 4);

	printf("\n\n");
}



//5.inet_pton()'通用地址'转换函数(线程安全版)
//	成功返回1, 失败返回0, 结果需要引用返回(但没办法保证, 地址是否正确)
void inet_pton_demo(void){
	struct sockaddr_in addr_in;
	struct in_addr addr_t;
	ulong uip;


	//正确使用demo:
	if(inet_pton(AF_INET, "10.0.0.0", &addr_t))
		printf("inet_pton(AF_INET,\"10.0.0.0\",&addr_t)=%d\n",addr_t.s_addr);
	else
		printf("inet_pton(AF_INET,\"10.0.0.0\",&addr_t) fail!!\n");

	printf("\n\n");
}



//6.inet_ntop() '通用地址'反向转换函数(线程安全版)
//	成功返回1, 失败返回0, 结果需要引用返回(但没办法保证, 地址是否正确)
void inet_ntop_demo(void){
	struct sockaddr_in addr_in,addr_in2;
	ulong uip;
	char ip_tmp[16],ip_tmp2[16];
	char* ret;


	//struct sockaddr_in 结构体填充demo
	bzero(&addr_in,sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr("192.168.1.101");
	addr_in.sin_port = htons(666);

	//正确使用demo:
	memset(ip_tmp,'\0',16);
	//strncpy(ip_tmp,inet_ntop(AF_INET,&addr_in,ip_tmp,16),16);
	inet_ntop(AF_INET,&addr_in.sin_addr,ip_tmp,16);
	printf("inet_ntop(AF_INET,%d,ip_tmp,16)=%s\n",&addr_in.sin_addr,ip_tmp);

	//struct sockaddr_in 同类拷贝赋值
	memcpy(&addr_in2,&addr_in,sizeof(struct sockaddr_in));

	//struct sockaddr_in 与unsigned long 拷贝赋值
	uip = inet_addr("192.168.0.74");
	memcpy(&addr_in.sin_addr, &uip, 4);
	uip = inet_addr("211.168.6.64");
	memcpy(&addr_in2.sin_addr, &uip, 4);

	printf("\n\n");
}
