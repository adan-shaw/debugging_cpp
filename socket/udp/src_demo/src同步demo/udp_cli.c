//编译
//		gcc -g3 ./udp_cli.c -o x 



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define rbuf_max (128)
#define srv_port (8888)
#define srv_ip "127.0.0.1"



int main(void){
	struct sockaddr_in addr;
	char rbuf[rbuf_max];
	const char sbuf[] = "hello server";
	int sfd, tmp;

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

	if(connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
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

	//printf("[client] debugging\n");
	tmp = read(sfd, &rbuf, sizeof(rbuf));
	//printf("[client] debugging\n");
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
