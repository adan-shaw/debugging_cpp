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



//1.打印单个网络协议信息
void printhost(struct hostent* h);

//2.gethostbyname() 测试函数
//	遍历<本地dns解析-记录文件>所有的host 主机信息, 测试<URL dns 解析>出'所有地址信息'
void gethostbyname_test(void);

//3.gethostbyhost() 测试函数
//	根据ip 反向解析出'所有地址信息'(只能解析出/etc/services 中的ip 地址??)
void gethostbyhost_test(void);



int main(void){
	gethostbyname_test();
	gethostbyhost_test();
	return 0;
}



//1.打印单个网络协议信息
void printhost(struct hostent* h){
	char** p;
	int tmp;

	//1.打印URL主机名称
	//official host name(url 地址下的官方主机名)
	printf("URL主机名称(official host name): %s.\n", h->h_name);

	p = h->h_aliases;
	while(*p != NULL){
		printf("	URL主机别名(alias name): %s;\n", *p);
		p++;
	}

	//2.打印通信协议簇类型, AF_INET = 2
	printf("协议簇类型AF_INET(addr type): %d.\n", h->h_addrtype);

	//3.打印ip 地址信息(错误打印方式)
	//	h->h_addr_list 是以"字符串存储"的(struct in_addr*)结构,
	//	并不是真的是ip addr 字符串,
	//	所以, 未经转换, 直接打印, 都是错误的.
	printf("ip地址长度(addr len): %d.\n", h->h_length);
	printf("start print() URL解析出来的所有'网关ip地址:'\n");//即打印ip地址链
	p = h->h_addr_list;
	tmp = 1;
	while(*p != NULL){

		//将h->h_addr_list转换为(struct in_addr*)地址信息描述体, 然后打印出ip地址.
		//你怎么肯定它是ipv4地址？如果是ipv6呢？
		//不重要!!只要是AF_INET, ipv4 or ipv6 都可以直接转换??
		//所以, 需要加判断:
		if(h->h_addrtype == AF_INET)
			printf("	URL网关主机ip addr-%d : %s;\n", \
					tmp, inet_ntoa(*((struct in_addr*)*p)));
		else
			printf("	h->h_addrtype==%d, it's not AF_INET=%d, cant print;\n",\
					h->h_addrtype,AF_INET);

		//直接打印字符串, 是错误的
		//printf("URL网关主机ip-%d: %s\n", tmp, *p);
		p++;
		tmp++;
	}
	return;
}



//2.gethostbyname() 测试函数
void gethostbyname_test(void){
	struct hostent* h;



	//1.遍历<本地dns解析-记录文件>所有的host 主机信息
	printf("1.遍历<本地dns解析-记录文件>所有的host 主机信息\n\n");

	sethostent(1);	//打开并’挟持‘本地dns解析-记录文件
	while(1){
		h = gethostent();
		if(h == NULL)
			break;			//读取结束or 遇到意外终止

		printhost(h);	//打印单个struct hostent 节点
		printf("\n");
	}
	endnetent();		//关闭并‘释放’本地dns解析-记录文件
	printf("\n\n\n");



	//2.测试<URL dns 解析>出'所有地址信息'
	printf("2.测试<URL dns 解析>出'所有地址信息'\n\n");
	//只有第一个hostent 地址有效, 其它都是空的, 访问就会指针越界
	//h = gethostbyname("127.0.0.1");			//环回地址ok
	//h = gethostbyname("192.168.0.100");	//网卡地址ok
	h = gethostbyname("www.baidu.com");		//百度地址测试->ok
	//h = gethostbyname("localhost");			//localhost 测试->ok
	//h = gethostbyname("eva");						//主机名 测试->ok
	//h = gethostbyname("handsome");			//主机别名 测试->失败
	if(h != NULL)
		printhost(h);
	else{
		printf("gethostbyname() failed!!or 找不到目标主机, 输入的URL有误\n");
		printf("hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));
	}
	printf("\n\n\n");



	//3.释放资源(struct hostent* h 是内置静态变量, 不能释放!)
	//free(h);//证明gethostbyname()函数内置了static struct hostent* h;
						//gethostbyname()函数不是线程安全的,会产生覆盖, 第二次的结果覆盖第一次的结果.
	return ;
}



//3.gethostbyhost() 测试函数
void gethostbyhost_test(void){
	struct hostent* h;
	struct in_addr inAddr;



	//根据ip 反向解析出'所有地址信息'
	//(只能解析出/etc/services 中的ip 地址??)
	printf("x.根据ip 反向解析出'所有地址信息'\n\n");
	inAddr.s_addr = inet_addr("127.0.0.1");
	//inAddr.s_addr = inet_addr("192.168.1.102");	//失败？

	//inAddr.s_addr = inet_addr("www.baidu.com");	//非ip地址字符串,肯定失败
	//inAddr.s_addr = inet_addr("localhost");			//失败
	//inAddr.s_addr = inet_addr("eva");						//失败

	h = gethostbyaddr(&inAddr, sizeof(inAddr), AF_INET);
	if(h != NULL)
		printhost(h);
	else{
		printf("gethostbyaddr() failed!!or 找不到目标主机, 输入的URL有误\n");
		printf("hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));
	}
	printf("\n\n\n");

	return ;
}
