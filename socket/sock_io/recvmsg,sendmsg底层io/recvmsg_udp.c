//编译
//		gcc -g3 ./recvmsg_udp.c -o x 



#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>



#define srv_ip "127.0.0.1"
#define srv_port (6666)

//TCP/UDP 报文长度min, max
#define mtu_udp_min (28)
#define mtu_udp_max (68)
#define mtu_tcp_min (40)
#define mtu_tcp_max (120)

//inet 互联网MTU(ARP 报头是加插的, 在以太局域网中需要考虑, inet互联网中不需要考虑)
#define mtu_inet_udp_min (576-mtu_udp_max)
#define mtu_inet_udp_max (576-mtu_udp_min)
#define mtu_inet_tcp_min (576-mtu_tcp_max)
#define mtu_inet_tcp_max (576-mtu_tcp_min)



//同步: 根据ip & port阻塞发送一次数据(一旦阻塞, 线程会被挂起), 成功返回发送的数据size, 失败返回-1
int udp_send(int sfd, struct sockaddr_in *paddr, const char* ip, unsigned short port, const char* pbuf, int data_len){
	int tmp;
	struct sockaddr_in addr;

	if(paddr == NULL){
		//bzero(&addr, sizeof(struct sockaddr_in));
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		tmp = sendto(sfd, pbuf, data_len, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	}
	else{
		tmp = sendto(sfd, pbuf, data_len, 0, (struct sockaddr*)paddr, sizeof(struct sockaddr));
	}

	if(tmp > 0)
		return tmp;//发送成功(非发送成功, 就应该close(sfd))
	if(tmp == 0){
		//if(data_len == tmp)//data_len == tmp == 0
		if(data_len == 0)
			return tmp;
		else
			return -1;//对端关闭, socket 数据没有发出去
	}
	//此时, tmp == -1
	perror("sendto()");
	return -1;
}

//server recv from client
int udp_recvmsg(int sfd, struct sockaddr_in *addr_peer){
	char buf_msghdr[sizeof(struct msghdr)], rbuf[mtu_inet_udp_max];
	char IP_dst[32], IP_ipi[32];
	struct msghdr mh;//recvmsg()/sendmsg() msg头
	struct iovec iov[1];//io 缓冲链(只有一个缓冲区, 则io 缓冲链就只有一个元素)
	struct cmsghdr *cmsg;//单节ip 协议报头指针
	struct in_pktinfo *pi;//单节ip 协议data 指针
	int tmp;

	//填充io 缓冲链
	iov[0].iov_base = rbuf;
	iov[0].iov_len = sizeof(rbuf);

	//填充recvmsg()/sendmsg() msg头
	mh.msg_name = addr_peer;
	mh.msg_namelen = sizeof(struct sockaddr_in);
	mh.msg_control = buf_msghdr;
	mh.msg_controllen = sizeof(buf_msghdr);
	mh.msg_iov = iov;//指认io 缓冲链
	mh.msg_iovlen = 1;//指认io 缓冲链的元素个数

	//同步接收(第三参数 == 0, 即为同步接收)
	tmp = recvmsg(sfd, &mh, 0);
	if(tmp > 0){
		//接收成功, 遍历所有的msg 控制头, 提取接收数据
		for(cmsg = CMSG_FIRSTHDR(&mh); cmsg == NULL; cmsg = CMSG_NXTHDR(&mh,cmsg)){
			if(cmsg->cmsg_level != IPPROTO_IP || cmsg->cmsg_type != IP_PKTINFO)
				continue;//不是IP 协议数据, 并且不可进行IP_PKTINFO 提取的, 跳过

			pi = (struct in_pktinfo *)CMSG_DATA(cmsg);

			if(inet_ntop(AF_INET,&(pi->ipi_spec_dst),IP_dst,sizeof(IP_dst)) != NULL)
				printf("recvmsg(): destination address: %s\n", IP_dst);

			if(inet_ntop(AF_INET,&(pi->ipi_addr),IP_ipi,sizeof(IP_ipi)) != NULL)
				printf("recvmsg(): source address: %s\n", IP_ipi);
		}
		printf("sfd-%d recvmsg(%d): %s\n", sfd, tmp, rbuf);
		return tmp;
	}
	if(tmp == 0){
		return -1;//对端关闭, socket 数据没有发出去
	}
	//此时, tmp == -1
	perror("recvmsg()");
	return -1;
}



int main(void){
	struct sockaddr_in addr, addr_ret;
	const char sbuf4cli[] = "hello server, i am the client";
	const char sbuf4srv[] = "hello client, i am the server";
	int tmp, sfd_li, sfd_cli, sfd_acc;

	sfd_li = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd_li == -1){
		perror("socket()");
		return -1;
	}
	tmp = 1;
	setsockopt(sfd_li, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_li, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	sfd_cli = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd_cli == -1){
		perror("socket()");
		return -1;
	}
	tmp = 1;
	setsockopt(sfd_cli, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_cli, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	sfd_acc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd_acc == -1){
		perror("socket()");
		return -1;
	}
	tmp = 1;
	setsockopt(sfd_acc, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	setsockopt(sfd_acc, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));


	//双方都设置使用struct in_pktinfo {} 结构体进行读解包, 即使用sendmsg() + recvmsg() 进行io;
	//(但为了测试的便捷性, 使用sendto() 代替sendmsg(), 节约代码量)
	tmp = 1;
	tmp = setsockopt(sfd_li, IPPROTO_IP, IP_PKTINFO, &tmp, sizeof(int));

	tmp = 1;
	tmp = setsockopt(sfd_cli, IPPROTO_IP, IP_PKTINFO, &tmp, sizeof(int));

	tmp = 1;//回发的使用sendto() 的sfd, 其实可以不改setsocketopt()
	tmp = setsockopt(sfd_acc, IPPROTO_IP, IP_PKTINFO, &tmp, sizeof(int));


	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(srv_port);
	if(bind(sfd_li, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1){
		perror("bind()");
		close(sfd_li);
		close(sfd_cli);
		close(sfd_acc);
		return -1;
	}


	//client sendto() server
	tmp = udp_send(sfd_cli, NULL, srv_ip, srv_port, sbuf4cli, sizeof(sbuf4cli));
	if(tmp == -1)
		return -1;
	else
		printf("client socket %d udp_send(%d) data to server: ip=%s,port=%d\n", sfd_cli, tmp, srv_ip, srv_port);

	//server recvmsg() from client(自动打印消息)
	if(udp_recvmsg(sfd_li, &addr_ret) == -1)
		return -1;

	//server sendto() client
	tmp = udp_send(sfd_acc, &addr_ret, NULL, 0, sbuf4srv, sizeof(sbuf4srv));
	if(tmp == -1)
		return -1;
	else
		printf("server socket %d udp_send(%d) data to client: ip=%s,port=%d\n", sfd_acc, tmp, srv_ip, srv_port);

	//client recvmsg() from server(自动打印消息)
	if(udp_recvmsg(sfd_cli, &addr_ret) == -1)
		return -1;


	close(sfd_li);
	close(sfd_cli);
	close(sfd_acc);
	return 0;
}

