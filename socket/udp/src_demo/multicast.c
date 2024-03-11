//编译:
//		gcc -g3 ./multicast.c -o x 



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>



#define udp_multicast_port (6666)
#define udp_multicast_ip "224.0.0.12"//多播ip 地址(必须为多播段内的ip 地址才生效)
#define max_buf_msg 512



//多播接收端(服务端)
int server(void){
	char msg_r[max_buf_msg] = {0};
	struct sockaddr_in addr;
	struct hostent *host;
	struct ip_mreq mreq;
	int len, tmp, sfd;

	//获取多播地址的host 信息(如果找不到这个IP/host, 则会出错)
	host = gethostbyname(udp_multicast_ip);
	if(host == NULL){
		perror("gethostbyname()");
		return -1;
	}

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));//重用端口

	tmp = 8;//设置sfd 多播的ttl 数值
	setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_TTL, &tmp, sizeof(int));

	tmp = 0;//设置sfd 多播, 不允许给'环回地址'回送数据
	setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_LOOP, &tmp, sizeof(int));

	//初始化udp 多播recvfrom() 的地址info
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//跟广播一样, 多播recvfrom() 端的地址信息, 通常都为: INADDR_ANY, 不需要特殊指定
	addr.sin_port = htons(udp_multicast_port);//多播端口

	if(bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind()");
		close(sfd);
		return -1;
	}

	//将多播地址信息和网卡interface接口ip信息进行绑定, 命令网卡interface 接口ip, 接收该多播地址的ip 报文(一个网卡ip, 只能绑定一个多播ip)
	//###mreq.imr_multiaddr.s_addr = htonl(INADDR_ANY);			//必须指定准确的'多播数据的接收ip', 不能是INADDR_ANY
	mreq.imr_multiaddr.s_addr = inet_addr(udp_multicast_ip);//指定'多播数据的接收ip'
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);					//指定'接收多播数据的绑定网卡ip'(跟广播一样,多播recvfrom()端的地址信息,通常都为:INADDR_ANY,不需要特殊指定)

	//检查多播地址是否填写错误(大可不必, 可以省略)
	if(mreq.imr_multiaddr.s_addr == -1){
		printf("%s not a legal multicast address\n", udp_multicast_ip);
		close(sfd);
		return -1;
	}

	//将sfd 拉入一个多播组
	if(setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) == -1){
		perror("setsockopt()");
		close(sfd);
		return -1;
	}

	//接收多播数据
	len = sizeof(struct sockaddr_in);
	while(1){
		bzero(&msg_r, sizeof(msg_r));

		//根据struct sockaddr_in addr 广播地址info 结构体, 接受广播消息
		tmp = recvfrom(sfd, msg_r, max_buf_msg, 0, (struct sockaddr*)&addr, (socklen_t*)&len);
		if(tmp == -1){
			perror("recvfrom()");
			close(sfd);
			return -1;
		}
		else
			printf("server(%d): sfd-%d recvfrom ok, len = %d, data: %s\n", getpid(), sfd , tmp, msg_r);

		sleep(1);
	}

	//sfd 退出多播组
	if(setsockopt(sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) == -1){
		perror("setsockopt()");
		close(sfd);
		return -1;
	}

	close(sfd);
	return 0;
}



//多播发送端(客户端)
int client(void){
	const char sbuf[] = "hi server [udp-multicast]";
	struct sockaddr_in addr;
	int tmp, sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));//重用端口

	//初始化多播IP 地址
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(udp_multicast_ip);
	addr.sin_port = htons(udp_multicast_port);

	while(1){
		sleep(1);

		//向多播地址, 发送多播数据
		tmp = sendto(sfd, &sbuf, sizeof(sbuf), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
		if(tmp == -1){
			perror("sendto()");
			close(sfd);
			return -1;
		}
		else{
			sleep(1);//多播报文总是延长打印(发送高傲模式)
			printf("*client(%d): sfd-%d sendto() ok, len = %d, data: %s\n", getpid(), sfd, tmp, sbuf);
		}
	}

	close(sfd);
	return 0;
}



//自收自发test
int main(void){
	int ppid = getpid();
	int pid = fork();
	if(pid == 0){//子进程
		printf("client() start: pid = %d, ppid = %d\n", getpid(), ppid);
		if(client() == -1)
			return -1;
		printf("client() quit: pid = %d, ppid = %d\n", getpid(), ppid);
	}
	else{//父进程(两个接收者, 如果只有一条多播报文, 或者一条多播报文都没收到, 即多播接收失败)
		pid = fork();
		if(pid == 0){//子进程
			printf("server() start: pid = %d, ppid = %d\n", getpid(), ppid);
			if(server() == -1)
				return -1;
			printf("server() quit: pid = %d, ppid = %d\n", getpid(), ppid);
		}
		else{
			printf("server() start: pid = %d, ppid = %d\n", getpid(), ppid);
			if(server() == -1)
				return -1;
			printf("server() quit: pid = %d, ppid = %d\n", getpid(), ppid);
		}
	}
	return 0;
}

