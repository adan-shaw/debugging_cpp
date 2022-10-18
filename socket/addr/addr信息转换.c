//编译:
//		gcc -g3 ./addr信息转换.c -o x 



#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//非线程安全函数, 而且需要你自己保证"输入字符串"的正确性, 
//传入野指针or 字符串不正确, 返回错误的值, 那么则会产生错误值, 甚至指针访问崩溃;

//快速填充struct sockaddr_in (不安全的)
#define full_sockaddr_in(addr_in, ip, port) { addr_in.sin_family = AF_INET; \
	addr_in.sin_addr.s_addr = inet_addr(ip); addr_in.sin_port = htons(port); }

//快速填充struct sockaddr_in (安全的)
#define full_sockaddr_inEx(addr_in, ip, port) { addr_in.sin_family = AF_INET; \
	if(!inet_pton(AF_INET, "192.168.1.101", &addr_in.sin_addr)) perror("inet_pton()"); addr_in.sin_port = htons(port); }


int main(void){
	struct sockaddr addr;
	struct sockaddr_in addr_in;
	const char* host_ip = "192.168.123.111";
	const char* net_ip = "192.168.0.0";
	unsigned short port = 9999;
	char* ptmp = NULL;
	unsigned int uip = 0;
	char ip_str[16];



	//快速填充inet地址信息结构体
	full_sockaddr_in(addr_in, host_ip, port);



	//addr转换string -> ulong: 无格式检查, 无法保证值是否有效
	uip = inet_network(net_ip);													//网络地址转换
	uip = inet_addr(host_ip);														//主机地址转换



	//addr转换string -> ulong: 带格式检查, 保证值是否有效[成功返回1, 失败返回0] -- 非线程安全, AF_INET only
	//assert(inet_aton("999.0.0.0",&addr_in.sin_addr));	//转换出错一定返回0
	if(!inet_aton(host_ip,&addr_in.sin_addr))
		perror("inet_aton()");

	//addr转换ulong -> string
	ptmp = inet_ntoa(addr_in.sin_addr);
	if(ptmp)
		printf("inet_ntoa() = %s\n",ptmp);



	//addr转换string -> ulong: 带格式检查, 保证值是否有效[成功返回1, 失败返回0] -- 线程安全, 其他socket协议簇也可以用
	if(!inet_pton(AF_INET, host_ip, &addr_in.sin_addr))
		perror("inet_pton()");

	//addr转换ulong -> string
	inet_ntop(AF_INET, &addr_in.sin_addr, ip_str, 16);	//带字符串截断
	printf("inet_ntop() = %s\n",ip_str);



	//安全的inet地址信息结构体填充方案:
	full_sockaddr_inEx(addr_in, host_ip, port);
	/*
		addr_in.sin_family = AF_INET;
		if(!inet_pton(AF_INET, "192.168.1.101", &addr_in.sin_addr))
			perror("inet_pton()");
		addr_in.sin_port = htons(666);
	*/
	return 0;
}


