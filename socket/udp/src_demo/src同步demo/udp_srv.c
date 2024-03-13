//编译:
//		gcc -g3 ./udp_srv.c -o x 



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define rbuf_max (128)
#define srv_port (8888)
#define srv_ip "127.0.0.1"



int main(void){
	char rbuf[rbuf_max];
	const char sbuf[] = "hello client";
	struct sockaddr_in addr, addr_tmp;
	int sfd, sfd_tmp, tmp;
	socklen_t len = sizeof(struct sockaddr_in);

	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_addr.s_addr = inet_addr(srv_ip);
	addr.sin_port = htons(srv_port);

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

		/*
			如果有业务需要, 可以先解析recvfrom() 收到的数据, 再决定是否dup(); 

			什么时候执行dup():
				仅第一次收到该客户端data 时, 执行dup();
				其他recvfrom() 收到的数据, 都是业务数据;

			业务数据:
				可以根据dup() sfd 的number 值, 来做业务数据判别, 从而找回sfd, 进而做下一步操作;
				也可以根据client 的ip+port, 来做业务数据判别, 从而找回sfd, 进而做下一步操作[udp 每次recvfrom() 都会返回client 的ip+port];
		*/
		sfd_tmp = dup(sfd);

		if(connect(sfd_tmp, (struct sockaddr *)&addr_tmp, sizeof(struct sockaddr_in)) == -1){
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

		close(sfd_tmp);
	}

	close(sfd);
	return 0;	
}



