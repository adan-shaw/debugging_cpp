//编译:
//		gcc -g3 ./udp_connect_old.c -o x 



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>



#define rbuf_max (128)
#define srv_port (8888)
#define srv_ip "127.0.0.1"



//客户端: write() + read()
int client(struct sockaddr_in *addr){
	char rbuf[rbuf_max];
	const char sbuf[] = "hello server";
	int sfd, tmp;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	tmp = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
	tmp = 1;
	tmp = setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

	if(connect(sfd, (struct sockaddr *)addr, sizeof(struct sockaddr_in)) == -1){
		perror("connect()");
		close(sfd);
		return -1;
	}

	tmp = write(sfd, &sbuf, sizeof(sbuf));
	if(tmp == -1){
		perror("write()");
		close(sfd);
		return -1;
	}
	else
		printf("[client] udp socket %d write(%d)\n", sfd, tmp);

	printf("[client] debugging\n");				//为什么这里死活read() 阻塞呢? 理由很简单: 
	tmp = read(sfd, &rbuf, sizeof(rbuf));	//	你在上面connect() 是server bind() 的地址信息, 
	printf("[client] debugging\n");				//	而你新创建的sfd_tmp, 指向的不应该是addr_peer, 而是原来绑定的addr
	if(tmp == -1){
		perror("read()");
		close(sfd);
		return -1;
	}
	else
		printf("[client] udp socket %d read(%d): %s\n", sfd, tmp, rbuf);

	close(sfd);
	return 0;
}



//服务端: write() + read()
int main(void){
	char rbuf[rbuf_max];
	const char sbuf[] = "hello client";
	struct sockaddr_in addr, addr_tmp;
	int sfd, sfd_tmp, tmp;
	socklen_t len = sizeof(struct sockaddr_in);

	//填充测试所需的addr 信息
	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_port = htons(srv_port);

	tmp = fork();
	if(tmp == 0){//子进程: 客户端
		//sleep(1);//for test only

		if(client(&addr) == -1){
			printf("client() failed!!");
			close(sfd);
			return -1;
		}
	}
	else{//父进程: 服务器
		sfd = socket(AF_INET, SOCK_DGRAM, 0);
		if(sfd == -1){
			perror("socket()");
			return -1;
		}

		tmp = 1;
		tmp = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
		tmp = 1;
		tmp = setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

		if(bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1){
			perror("bind()");
			close(sfd);
			return -1;
		}

		//udp socket 不支持执行listen() 函数!!
		/*
		if(listen(sfd,32) == -1){
			perror("listen()");
			close(sfd);
			return -1;
		}
		*/

		tmp = recvfrom(sfd, rbuf, rbuf_max, 0, (struct sockaddr*)&addr_tmp, &len);
		if(tmp == -1){
			perror("recvfrom()");
			close(sfd);
			return -1;
		}
		else{
			printf("[server] client_ip:%s, client_port:%d\n", inet_ntoa(addr.sin_addr), addr.sin_port);
			printf("[server] sfd=%d recvfrom(%d) data: %s\n", sfd, tmp, rbuf);
		}

		sfd_tmp = socket(AF_INET, SOCK_DGRAM, 0);
		if(sfd_tmp == -1){
			perror("socket()");
			close(sfd);
			return -1;
		}

		tmp = 1;
		tmp = setsockopt(sfd_tmp, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
		tmp = 1;
		tmp = setsockopt(sfd_tmp, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));

		if(connect(sfd_tmp, (struct sockaddr *)&addr_tmp, sizeof(struct sockaddr_in)) == -1){
		//if(connect(sfd_tmp, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){//无效, 你必须告诉客户端, 这个新的sfd_tmp 的ip+port, 客户端才能进行正确connect()
			perror("connect()");
			close(sfd_tmp);
			close(sfd);
			return -1;
		}

		tmp = write(sfd_tmp, &sbuf, sizeof(sbuf));
		if(tmp == -1){
			perror("write()");
			close(sfd_tmp);
			close(sfd);
			return -1;
		}
		else
			printf("[server] udp socket %d write(%d)\n", sfd_tmp, tmp);

		//wait(&tmp);//等待子进程结束(debugging ??)
		//printf("[server] son process quit: %d\n", tmp);

		//close(sfd_tmp);
	}

	close(sfd);
	return 0;	
}
