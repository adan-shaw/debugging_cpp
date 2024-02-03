//编译:
//		gcc -g3 ./addr信息转换old.c -o x 



#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//0.
void inet_addr_basic(void){
	const char *ip = "192.168.0.1";
	unsigned short port = 9213;
	struct sockaddr_in addr_in;
	struct sockaddr addr;



	//struct sockaddr_in 结构体填充demo
	//bzero(&addr_in,sizeof(addr_in));			//bzero 是一个全部置'\0'的函数
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr(ip);//字符串转in_addr_t = uint32_t
	addr_in.sin_port = htons(port);					//网络字节序转换=uint16_t

	//inet4地址结构体, 拷贝到'通用sockaddr地址描述结构体', 直接拷贝
	memcpy(&addr,&addr_in,sizeof(struct sockaddr));

	//打印转换后的情况, 是乱码
	printf("inet_addr(%s)=%d=uint32_t\nhtons(%d)=%d=uint16_t\n\n",
		ip,addr_in.sin_addr.s_addr,port,addr_in.sin_port);

	//证明addr_in, addr 可以随意地互相memcpy() 拷贝
	memcpy(&addr_in,&addr,sizeof(struct sockaddr_in));

	printf("struct sockaddr_in:\n\
		[%d](inet4协议簇), [%d](地址指针值=整形式0.0.0.0)\n\
		[%d](网络字节序), [%d](整形式0.0.0.0)\n",\
		addr_in.sin_family,addr_in.sin_addr,\
		addr_in.sin_port,addr_in.sin_addr.s_addr);

	printf("struct sockaddr:\n[%d](inet4协议簇), [%s](无字符串结束符)\n",addr.sa_family,addr.sa_data);
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
	char ip_tmp[INET_ADDRSTRLEN];
	char* ret;



	//struct sockaddr_in 结构体填充demo
	//bzero(&addr_in,sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr("192.168.1.101");
	addr_in.sin_port = htons(666);

	//正确使用demo:
	strncpy(ip_tmp,inet_ntoa(addr_in.sin_addr),INET_ADDRSTRLEN);
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
	char ip_tmp[INET_ADDRSTRLEN],ip_tmp2[INET_ADDRSTRLEN];
	char* ret;



	//struct sockaddr_in 结构体填充demo
	bzero(&addr_in,sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = inet_addr("192.168.1.101");
	addr_in.sin_port = htons(666);

	//正确使用demo:
	memset(ip_tmp,'\0',INET_ADDRSTRLEN);
	//strncpy(ip_tmp,inet_ntop(AF_INET,&addr_in,ip_tmp,INET_ADDRSTRLEN),INET_ADDRSTRLEN);
	inet_ntop(AF_INET,&addr_in.sin_addr,ip_tmp,INET_ADDRSTRLEN);
	printf("inet_ntop(AF_INET,%d,ip_tmp,INET_ADDRSTRLEN)=%s\n",&addr_in.sin_addr,ip_tmp);

	//struct sockaddr_in 同类拷贝赋值
	memcpy(&addr_in2,&addr_in,sizeof(struct sockaddr_in));

	//struct sockaddr_in 与unsigned long 拷贝赋值
	uip = inet_addr("192.168.0.74");
	memcpy(&addr_in.sin_addr, &uip, 4);
	uip = inet_addr("211.168.6.64");
	memcpy(&addr_in2.sin_addr, &uip, 4);

	printf("\n\n");
}



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



