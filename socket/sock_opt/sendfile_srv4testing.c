//编译:
//		gcc -g3 ./sendfile_srv4testing.c -o x 

//for test:
//		curl 127.0.0.1:9999



#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<errno.h>
#include"opt_basic.h"
#include"opt_ip.h"
#include"opt_tcp.h"
#include"opt_udp.h"



#define ip "127.0.0.1"
#define port (9999)
#define file_name "sendfile_srv4testing.c"



int main(void){
	struct sockaddr_in addr,addr_cli;
	struct stat stat_buf;
	int fd, sfd, sfd_cli;
	socklen_t addr_cli_len = sizeof(struct sockaddr_in);

	fd = open(file_name,O_RDONLY);
	if(fd == -1){
		perror("open");
		return -1;
	}

	fstat(fd,&stat_buf);

	//bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&addr.sin_addr);
	addr.sin_port = htons(port);

	sfd = socket(PF_INET,SOCK_STREAM,0);
	if(sfd == -1){
		perror("socket");
		close(fd);
		return -1;
	}

	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) == -1){
		perror("bind");
		close(sfd);
		close(fd);
		return -1;
	}

	listen(sfd,5);



	sfd_cli = accept(sfd,(struct sockaddr*)&addr_cli,&addr_cli_len);
	if(sfd_cli == -1){
		perror("accept");
		close(sfd);
		close(fd);
		return -1;
	}
	else{
		sendfile(sfd_cli,fd,NULL,stat_buf.st_size);
		shutdown(sfd_cli,2);
		close(sfd_cli);
	}

	shutdown(sfd,2);
	close(sfd);
	close(fd);
	return 0;
}
