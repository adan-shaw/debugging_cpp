//编译:
//		gcc -g3 ./gethostbyname.c -o x 


//注意1:
//	主要用来解析dns 域名, host主机名下的ip地址.

//注意2:
//	struct hostent *h; 是静态变量; gethostbyname()系列函数, 都不是线程安全函数.

/*
	//1.<host>地址查询API:
	extern int h_errno;
	struct hostent* gethostbyname(const char *name);
	struct hostent* gethostbyaddr(const void *addr,socklen_t len,int type);
	struct hostent* gethostent(void); //返回下一个文件
	void sethostent(int stayopen);		//打开文件
	void endhostent(void);						//关闭文件


	//2.返回的<dns host>信息载体
	struct hostent{
		char *h_name;									//主机名
		char **h_aliases;							//可选的别名列表
		int h_addrtype;								//地址类型, 一般为AF_INET
		int h_length;									//地址长度
		char **h_addr_list;						//网络地址列表
		#define h_addr h_addr_list[0]; //第一个网络地址
	};
	//说明:
		之所以主机的地址是一个链表的形式, 其原因是:
			一个主机可能有多个网络接口.
			而且, 返回的地址为网络字节序的'整形数'.
*/



#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <malloc.h>



void printhost(struct hostent* h){
	char** p;

	printf("URL主机名称(official host name): %s.\n", h->h_name);
	printf("协议簇类型AF_INET(addr type): %d.\n", h->h_addrtype);
	printf("ip地址长度(addr len): %d.\n", h->h_length);

	if(h->h_addrtype == AF_INET){
		printf("打印解析结果-URL主机别名链表(alias name):'\n");
		p = h->h_aliases;
		while(*p != NULL)
			printf("	URL主机别名(alias name): %s;\n", *p++);

		printf("打印解析结果-URL主机ip链表:'\n");
		p = h->h_addr_list;
		while(*p != NULL)
			printf("	URL网关主机ip: %s;\n",inet_ntoa(*((struct in_addr*)*p++)));
	}
	printf("\n");
	return;
}



//gethostent() 查询本地dns记录文件
void gethostent_test(void){
	struct hostent* h;

	printf("* 遍历'本地dns记录文件'所有的host 主机信息\n");
	sethostent(1);		//打开并'挟持'本地dns记录文件
	while(1){
		h = gethostent();
		if(h == NULL)
			break;				//读取结束or 遇到意外终止
		else
			printhost(h);	//打印单个struct hostent 节点
	}
	endnetent();			//关闭并'释放'本地dns记录文件
	return;
}



//gethostbyname() 测试函数
void gethostbyname_test(void){
	struct hostent* h;

	printf("* 测试<URL dns 解析>出'所有地址信息'\n");
	//只有第一个hostent 地址有效, 其它都是空的, 访问就会指针越界
	//h = gethostbyname("127.0.0.1");			//环回地址ok
	//h = gethostbyname("192.168.0.100");	//网卡地址ok
	h = gethostbyname("www.baidu.com");		//百度地址测试->ok
	//h = gethostbyname("localhost");			//localhost 测试->ok
	//h = gethostbyname("eva");						//主机名 测试->ok
	//h = gethostbyname("handsome");			//主机别名 测试->失败
	if(h != NULL)
		printhost(h);
	else
		printf("gethostbyname() failed, hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));

	//释放资源( struct hostent* h 是内置静态变量, 不能释放! 
	//free(h); 同时说明gethostbyname()函数不是线程安全函数,第二次的操作结果, 会覆盖第一次的结果)
	return ;
}



//gethostbyhost() 测试函数
void gethostbyhost_test(void){
	struct hostent* h;
	struct in_addr inAddr;

	//根据ip 反向解析出'所有地址信息'
	//(只能解析出/etc/services 中的ip 地址??)
	printf("* 根据ip 反向解析出'所有地址信息'\n");
	inAddr.s_addr = inet_addr("127.0.0.1");
	//inAddr.s_addr = inet_addr("192.168.1.102");	//失败?

	//inAddr.s_addr = inet_addr("www.baidu.com");	//非ip地址字符串,肯定失败
	//inAddr.s_addr = inet_addr("localhost");			//失败
	//inAddr.s_addr = inet_addr("eva");						//失败

	h = gethostbyaddr(&inAddr, sizeof(inAddr), AF_INET);
	if(h != NULL)
		printhost(h);
	else
		printf("gethostbyaddr() failed, hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));

	return ;
}



int main(void){
	gethostent_test();
	gethostbyname_test();
	gethostbyhost_test();
	return 0;
}



