//编译:
//		gcc -g3 ./getpostbyname.c -o x 


//注意1:
//	主要用来读取<当前主机>目前已经记录<服务名>和<对应的端口号>

//注意2:
//	struct netent *h; 是静态变量, getnetbyname()系列函数, 都不是线程安全函数.


/*
	//1.<serv>API函数, 用于操作服务于端口号:
	struct servent* getservbyname(const char* name, const char* proto);
	struct servent* getservbyport(int port, const char* proto);
	struct servent* getservent(void);	//读取并后移, 通常遍历的时候用
	void setservent(int stayopen);		//打开/etc/services, 遍历需要挟持
	void endservent(void);						//关闭/etc/services


	//2.协议存放结构体
	struct servent{
		char *s_name;			// Official service name.
		char **s_aliases;	// Alias list.
		int s_port;				// Port number.
		char *s_proto;		// Protocol to use.
	};
*/



#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>



//1.打印单个服务对应的端口信息, 所使用的协议信息
void printservent(struct servent* serv);

//2.getservbyname() + getservbyport()测试函数
void getservbyname_byport_test(void);



int main(void){
	getservbyname_byport_test();
	return 0;
}



//1.打印单个服务对应的端口信息, 所使用的协议信息
void printservent(struct servent* serv){
	char** p;

	printf("服务名称 serv_name: %s\n", serv->s_name);
	printf("服务端口 port number: %d\n", serv->s_port);
	printf("协议类型 serv_proto: %s\n", serv->s_proto);

	p = serv->s_aliases;
	while(*p != NULL){
		printf("服务别名 alias name: %s\n", *p);
		p++;
	}
	return;
}



//2.getservbyname() + getservbyport()测试函数
void getservbyname_byport_test(void){
	struct servent* serv;
	const char* service_name = "service_adan";
	const char* service_type = "tcp";
	const int service_port = 9213;



	printf("1.遍历/etc/services 里面的信息\n\n");
	//1.遍历/etc/services 里面的信息
	setservent(1);				//打开/etc/services 并挟持

	while(1){
		serv = getservent();
		if(serv == NULL)
			break;						//读取结束or 遇到意外终止.

		printservent(serv);	//打印单个struct servent 节点
		printf("\n");
	}

	endservent();					//关闭/etc/services 并释放
	printf("\n\n\n");



	//2.根据服务名和协议类型, 获取端口号
	printf("2.根据服务名和协议类型, 获取端口号\n\n");
	serv = getservbyname(service_name,service_type);
	if(serv != NULL)
		printf("getservbyname(%s,%s) = %d\n",\
				serv->s_name, serv->s_proto, serv->s_port);
	else{
		printf("cant found [ 类型为:%s, 名为:%s ] 的服务\n",\
				service_name, service_type);
		printf("hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));
	}
	printf("\n\n\n");



	//3.根据端口号和协议名, 获取服务名(这个可能需要注册才能查得到)
	printf("3.根据端口号和协议名, 获取服务名(这个可能需要注册才能查得到)\n\n");
	serv = getservbyport(service_port,service_type);
	if(serv != NULL)
		printf("getservbyport(%d,%s) = %s\n",\
				serv->s_port, serv->s_proto, serv->s_name);
	else{
			printf("cant found [ 类型为:%s, 端口为:%d ] 的服务\n",\
					service_type, service_port);
			printf("hstrerror(%d)=%s\n",h_errno,hstrerror(h_errno));
		}
	printf("\n\n\n");



	//4.不需要释放struct servent*,证明getservbyname()实现过程, struct servent*是内置static变量的.
	//free(serv);
	return ;
}
