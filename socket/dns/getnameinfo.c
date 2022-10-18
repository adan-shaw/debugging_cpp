//编译
//		gcc -g3 ./getnameinfo.c -o x 

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>

void getnameinfo_test(void){
	int tmp;
	struct sockaddr_in addr_in;
	char host_buf[16], port_buf[16];

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(6666);
	inet_pton(AF_INET, "127.0.0.1", &addr_in.sin_addr);

	//执行getnameinfo(), 获取srv 信息, 并打印
	if(getnameinfo((struct sockaddr*)&addr_in,sizeof(struct sockaddr_in),\
			host_buf,sizeof(host_buf),\
			port_buf,sizeof(port_buf),NI_NAMEREQD) != 0)
		perror("getnameinfo()");
	else{
		printf("host name: %s\n",host_buf);
		printf("service port: %s\n",port_buf);
	}
	return;
}

int main(void){
	getnameinfo_test();
	return 0;
}



//1.getnameinfo()函数API
/*
	int getnameinfo(\
		const struct sockaddr* addr,\
		socklen_t alen,\
		char* restrict host,\
		socklen_t hostlen,\
		char* restrict service,\
		socklen_t servlen,\
		int flags);
		//具体的flags参数如下:[getnameinfo()主要靠flags 参数工作]
		标志							描述
		NI_DGRAM				服务基于数据报而非基于流
		NI_NAMEREQD			如果找不到主机名, 将其作为一个错误对待
		NI_NOFQDN				对于本地主机, 仅返回全限定域名的节点名部分
		NI_NUMERICHOST	返回主机地址的数字形式, 而非主机名
		NI_NUMERICSCOPE	对于IPv6, 返回数字形式
		NI_NUMERICSERV	返回服务地址的数字形式(即端口号)

	freeaddrinfo(),gai_strerror(),addrinfo{}结构体等信息和用法,
	可以参考姐妹file: getaddrinfo.cpp; 这里不再详细叙述.
*/



//2.反向获取对方struct sockaddr{}的API和结构体:
/*
	//结构体
	struct sockaddr{
		unsigned short sa_family;	// address family, AF_xxx
		char sa_data[14];					// 14 bytes of protocol address
	};
	//API
	int getsockname();
	int getpeername();

	前提:	需要对方已经完成connect(), 即完成握手操作.
				详情可以查看: ./socket_addr/getpeername.cpp
*/
