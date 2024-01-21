//编译:
//		gcc -g3 ./getnetbyname.c -o x 



//注意1:
//	主要用来获取网络配置, 网卡信息

//注意2:
//	struct netent *h; 是静态变量, getnetbyname()系列函数, 都不是线程安全函数.

/*
	//1.<net>API函数, 用来获得<网络名字>和<网络编号>:
	struct netent* getnetbyaddr(uint32_t net, int type);
	struct netent* getnetbyname(const char* name);
	struct netent* getnetent(void);	//读取并后移, 通常遍历的时候用
	void setnetent(int stayopen);		//打开记录文件, 遍历需要挟持
	void endnetent(void);						//关闭记录文件

	//2.返回<网络名字>和<网络编号>的信息载体
	struct netent{
		char* n_name;									// network name
		char** n_aliases;							// alternate network name array pointer
		int n_addrtype;								// net address type
		uint32_t n_net;								// network number
	};
*/


#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>



void printnet(struct netent* net){
	char** p;

	printf("net name(网络名称/类型): %s\n", net->n_name);
	printf("address type(网络地址类型): %d\n", net->n_addrtype);
	printf("net number(网络识别码): %u\n", net->n_net);

	p = net->n_aliases;
	while(*p != NULL)
		printf("alias name(网络别名): %s\n", *p++);

	return;
}



void getnetent_test(void){
	struct netent* net;

	printf("遍历'本地记录文件'里面所有的网络信息\n");
	setnetent(1);										//打开<记录文件> 并挟持

	while(1){
		net = getnetent();
		if(net == NULL)
			break;											//读取结束or 遇到意外终止.
		else
			printnet(net);							//打印单个struct netent 节点
	}

	endnetent();										//关闭<记录文件> 并释放
	return;
}



void getnetbyname_test(void){
	struct netent* net;
	const char *net_name = "loopback";

	printf("单次获取测试: getnetbyname(\"loopback\")\n");
	net = getnetbyname(net_name);
	if(net != NULL)
		printnet(net);
	else
		printf("getnetbyname(%s) failed, hstrerror(%d)=%s\n",net_name,h_errno,hstrerror(h_errno));

	//不需要释放struct netent*, 证明getnetbyname()实现过程, struct netent*是内置static变量的.
	//free(net);
	return ;
}



int main(void){
	getnetent_test();
	getnetbyname_test();
	return 0;
}
