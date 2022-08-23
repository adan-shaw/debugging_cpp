//编译:
//		gcc -g3 ./getprotobyname.c -o x 


//注意1:
//	主要用来读取主机支持的网络协议: ipv4,ipv6,路由协议, 等

//注意2:
//	struct netent *h; 是静态变量, getnetbyname()系列函数, 都不是线程安全函数.

/*
	//1.<proto>API函数, 用于操作协议名和协议编号:
	struct protoent* getprotobyname(const char* name);//指定协议名, 测试能不能用.
	struct protoent* getprotobynumber(int proto);			//少用
	struct protoent* getprotoent(void);								//读取并后移, 通常遍历的时候用

	//如果参数stayopen参数值为1,则接下来的getprotobyname()或getprotobynumber()将不会自动关闭此文件.
	void setprotoent(int stayopen);										//打开/etc/protocols
	void endprotoent(void);														//关闭/etc/protocols 文件


	//2.协议存放结构体
	struct protoent{
		char *p_name;			//Official protocol name.
		char **p_aliases;	//Alias list.
		int p_proto;			//Protocol number.
	};

*/


#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>



//1.打印单个网络协议信息
void printproto(struct protoent* proto);

//2.获取<当前主机>所支持的网络协议, 并打印所有的协议信息
void getprotobyname_test(void);



int main(void){
	getprotobyname_test();
	return 0;
}



//1.打印单个网络协议信息
void printproto(struct protoent* proto){
	char** p;

	printf("协议名称 proto name: %s\n", proto->p_name);
	printf("协议编号 proto number: %d\n", proto->p_proto);

	p = proto->p_aliases;
	while(*p != NULL){
		printf("协议别名 alias name: %s\n", *p);
		p++;
	}
	return;
}



//2.获取<当前主机>所支持的网络协议, 并打印所有的协议信息
void getprotobyname_test(void){
	struct protoent* proto;



	//1.遍历本地<记录文件>， 并打印所有的协议信息
	printf("1.遍历本地<记录文件>， 并打印所有的协议信息\n\n");
	setprotoent(1);				//打开并挟持<记录文件>
												//注意: 你不挟持文件,可能只会一直读取到第一个,读不到后面的协议

	while(1){
		proto = getprotoent();
		if(proto == NULL)		//如果读到/etc/protocols 文件结束
			break;						//读取结束or 遇到意外终止

		printproto(proto);	//打印单个struct protoent 节点
		printf("\n");
	}

	endprotoent();				//关闭并释放<记录文件>
	printf("\n\n\n");



	//2.打印指定协议名称, 打印协议信息; 不指定参数, 默认获取第一个协议(单点查询不需要打开/etc/protocols)
	printf("2.打印指定协议名称, 打印协议信息; 不指定参数, 默认获取第一个协议\n\n");
	proto = getprotobyname("ip");	//ip=ipv4, 你可以试试 ipv6
	if(proto != NULL)
		printproto(proto);					//打印测试结果
	else{
		printf("getprotoent() fail, h_errno = %d\n",h_errno);
		printf("%s\n",hstrerror(h_errno));
	}
	printf("\n\n\n");



	//3.不需要释放struct protoent*,证明getprotobyname()实现过程,struct protoent*是内置static变量的.
	//free(proto);

	return ;
}



//协议名称list数组(样板, 提供参考)
/*
const char *const protocol_name[]={
	"ip",
	"icmp",
	"igmp",
	"ggp",
	"ipencap",
	"st",
	"tcp",
	"egp",
	"igp",
	"pup",
	"udp",
	"hmp",
	"xns-idp",
	"rdp",
	"iso-tp4",
	"xtp",
	"ddp",
	"idpr-cmtp",
	"ipv6",
	"ipv6-route",
	"ipv6-frag",
	"idrp",
	"rsvp",
	"gre",
	"esp",
	"ah",
	"skip",
	"ipv6-icmp",
	"ipv6-nonxt",
	"ipv6-opts",
	"rspf",
	"vmtp",
	"eigrp",
	"ospf",
	"ax.25",
	"ipip",
	"etherip",
	"encap",
	"pim",
	"ipcomp",
	"vrrp",
	"l2tp",
	"isis",
	"sctp",
	"fc",
	NULL
};
*/
