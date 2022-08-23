//编译
//		gcc -g3 ./getnameinfo.c -o x 



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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>



#define BACKLOG (32)



//1.getnameinfo()测试函数
void getnameinfo_test(void);



int main(void){
	getnameinfo_test();
	return 0;
}



//1.getnameinfo()测试函数
void getnameinfo_test(void){
	int tmp;
	unsigned int sock_opt;
	int sfd_srv,sfd_cli,sfd_conn;				//socket fd
	struct sockaddr_in in4_srv,in4_cli;	//socket地址信息
	const char *srv_ip = "127.0.0.1";		//ip 字符串缓冲区
	const short srv_port = 6666;				//port 存储器
	char host_buf[64];
	char serv_buf[64];



	//1.快速创建srv socket && cli socket, 不考虑太多失败因素.
	sfd_srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd_srv == -1)
		return;
	sfd_cli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd_cli == -1)
		return;

	//1.1 设置地址重用
	sock_opt = 1;
	setsockopt(sfd_srv, SOL_SOCKET, SO_REUSEADDR,\
			&sock_opt, sizeof(unsigned int));

	//1.2 填充地址信息
	bzero(&in4_srv, sizeof(in4_srv));
	in4_srv.sin_family = AF_INET;
	in4_srv.sin_port = htons(srv_port);
	inet_pton(AF_INET, srv_ip, &in4_srv.sin_addr);
	//in4_srv.sin_addr.s_addr = inet_addr(srv_ip);//旧式

	//1.3 执行bind
	tmp=bind(sfd_srv,(struct sockaddr*)&in4_srv,sizeof(in4_srv));
	if(tmp == -1)
		return ;

	//1.4 执行listen
	tmp = listen(sfd_srv, BACKLOG);
	if(tmp == -1)
		return ;

	//1.5 执行connect
	tmp = connect(sfd_cli,(struct sockaddr*)&in4_srv,sizeof(in4_srv));
	if(tmp == -1)
		return ;

	//1.6 执行accept, 并获取客户端struct sockaddr* 信息
	sock_opt = sizeof(in4_cli);
	tmp = accept(sfd_srv, (struct sockaddr*)&in4_cli, &sock_opt);
	if(tmp == -1)
		return ;



	//2.执行getnameinfo(), 获取srv 信息, 并打印
	sock_opt = sizeof(in4_srv);
	tmp = getnameinfo((struct sockaddr*)&in4_srv,sock_opt,\
			host_buf,sizeof(host_buf),serv_buf,sizeof(serv_buf),NI_NAMEREQD);
	printf("server host name: %s\n",host_buf);
	printf("server service port: %s\n",serv_buf);

	//2.2 根据host name + port 端口号, 利用getaddrinfo(),
	//		反向求addrinfo{}结果链表. passed 省略



	//3.执行getnameinfo(), 获取cli 信息, 并打印
	memset(host_buf,'\0',sizeof(host_buf));
	memset(serv_buf,'\0',sizeof(serv_buf));
	sock_opt = sizeof(in4_cli);
	tmp = getnameinfo((struct sockaddr*)&in4_cli,sock_opt,\
			host_buf,sizeof(host_buf),serv_buf,sizeof(serv_buf),NI_NAMEREQD);
	printf("client host name: %s\n",host_buf);
	printf("client service port: %s\n",serv_buf);

	//3.2 根据host name + port 端口号, 利用getaddrinfo(),
	//		反向求addrinfo{}结果链表. passed 省略



	//4.释放资源
	close(sfd_conn);
	close(sfd_cli);
	shutdown(sfd_srv,2);
	close(sfd_srv);

	return;
}


