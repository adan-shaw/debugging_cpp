//编译:
//		gcc -g3 ./broadcast.c -o x 

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define max_buf_msg (512)

//bind()的port 必须与广播接收recvfrom() 的port 一样, 否则就收不到广播包;
#define udp_broadcast_port (6000)

//发送-广播数据的addr info
#define udp_broadcast_addr_all255 "255.255.255.255"  //受限广播地址          ok (本局域网内有效, 广域网被路由器/交换机拦截)
#define udp_broadcast_addr_all0 "0.0.0.0"            //混杂地址             failed
#define udp_broadcast_addr_c_net255 "192.168.56.255" //C类网192.168.56.255  failed (c类网: 192.168.56.x/24, 子网掩码:255.255.255.0)
#define udp_broadcast_addr_c_net0 "192.168.56.0"     //C类网192.168.56.0    failed
#define udp_broadcast_addr_c_host "192.168.56.2"     //C类地址192.168.56.2  failed

//接收-广播数据的addr info, 统一为: INADDR_ANY / INADDR_BROADCAST 即可, 无论发送广播地址是什么, 这点都不会变;

//总结
/*
	就算发送-广播数据的addr info, 不能指定固定的广播ip 地址, 也没关系, 
	因为广播数据本身就极度容易被拦截, 一般只能在本局域网内广播!! (交换机/路由器, 绝大部分都默认拦截广播数据)

	反正都出不了外网, 使用: 255.255.255.255 作为广播地址, 也可以在本局域网内广播, 不受影响;
	因此, 其实也不需要关太复杂的逻辑, 够用就好, 能在c 类局域网内广播就行;
*/



//接收-广播数据
int server(void){
	char msg_r[max_buf_msg] = {0};
	struct sockaddr_in addr;
	int len, tmp, sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	//设置该套接字为广播类型
	tmp = 1;
	if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(int)) == -1){
		perror("setsockopt()");
		return -1;
	}
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));//重用端口

	addr.sin_family = AF_INET;
	//无论发送广播数据的地址是什么, recvfrom() 接收广播数据的socket 地址, 只需要是INADDR_ANY 即可
	addr.sin_addr.s_addr = htonl(INADDR_ANY);                       //ok
	//addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);               //ok
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_host);  //failed!!
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_net255);//failed!!
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_net0);  //failed!!
	addr.sin_port = htons(udp_broadcast_port);

	//接收广播报文的recvfrom() socket, 必须先bind()
	if(bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1){
		perror("bind()");
		return -1;
	}

	//接收广播数据
	len = sizeof(struct sockaddr_in);
	while(1){
		bzero(&msg_r, sizeof(msg_r));

		//根据struct sockaddr_in addr 广播地址info 结构体, 接受广播消息
		tmp = recvfrom(sfd, msg_r, max_buf_msg, 0, (struct sockaddr*)&addr, (socklen_t*)&len);
		if(tmp == -1){
			perror("recvfrom()");
			return -1;
		}
		else
			printf("server(%d): sfd-%d recvfrom ok, len = %d, data: %s\n", getpid(), sfd , tmp, msg_r);

		sleep(1);
	}

	return 0;
}



//发送-广播数据
int client(void){
	const char msg_s[] = "hello server [udp-broadcast]";
	struct sockaddr_in addr;
	struct linger m_linger = {0,0};
	int len, tmp, sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sfd== -1) {
		perror("socket()");
		return -1;
	}

	//设置该套接字为广播类型
	tmp = 1;
	if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(int)) == -1){
		perror("setsockopt()");
		return -1;
	}
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));//重用端口

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//ok
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//ok
	addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_all255);//ok
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_all0);//failed!!
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_host);//failed!! (只对一个ip 地址广播, 实际上失败!!)
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_net255);//failed!!
	//addr.sin_addr.s_addr = inet_addr(udp_broadcast_addr_c_net0);//failed!!
	addr.sin_port = htons(udp_broadcast_port);

	len=sizeof(struct sockaddr_in);
	while(1){
		sleep(1);

		//根据struct sockaddr_in addr 广播地址info 结构体, 发送广播消息
		tmp = sendto(sfd, msg_s, strlen(msg_s), 0, (struct sockaddr*)&addr, len);
		if(tmp == -1){
			perror("sendto()");
			return -1;
		}
		else{
			sleep(1);//广播报文总是延长打印(发送高傲模式)
			printf("*client(%d): sfd-%d sendto() ok, len = %d, data: %s\n", getpid(), sfd, tmp, msg_s);
		}
	}

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
	else{//父进程(两个接收者, 如果只有一条广播报文, 或者一条广播报文都没收到, 即广播接收失败)
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

