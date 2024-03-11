//编译:
//		gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_srv.c -o x_srv; gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_cli.c -o x_cli 



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "unix_sock_frame.h"



//创建一个unix socket listener
int unix_sock_tcp_listener(void){
	int sfd, tmp;
	struct sockaddr_un addr_srv;

	//1.创建unix socket(SOCK_STREAM 流式)
	sfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));		//重用地址(unix socket 中仍然有效, 可以防止UNIX_UDP_DOMAIN 路径被占用)
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));		//重用端口

	//2.set unix socket srv端的addr 信息
	addr_srv.sun_family=AF_UNIX;
	strncpy(addr_srv.sun_path,UNIX_TCP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	//3.如果unix socket key-name 已经注册, unlink 取消它
	unlink(UNIX_TCP_DOMAIN);

	//4.bind() 绑定unix socket 与unix 地址信息
	if(bind(sfd,(struct sockaddr*)&addr_srv,sizeof(addr_srv)) == -1){
		perror("bind()");
		close(sfd);
		unlink(UNIX_TCP_DOMAIN);
		return -1;
	}

	//5.listen() 开始监听
	/*
	if(listen(sfd,UNIX_TCP_LISTEN_MAX) == -1){
		perror("listen()");
		close(sfd);
		unlink(UNIX_TCP_DOMAIN);
		return -1;
	}
	*/
	listen(sfd,UNIX_TCP_LISTEN_MAX);
	return sfd;
}



int main(void){
	int usfd_li,usfd_acc;
	struct sockaddr_un addr_cli;
	socklen_t addr_len_cli;
	int tmp,tmp2;
	utcp_frame_t frame;

	//创建一个unix socket listener
	usfd_li = unix_sock_tcp_listener();
	if(usfd_li == -1)
		return -1;

	//接受unix socket 链接
	usfd_acc = accept(usfd_li,(struct sockaddr*)&addr_cli,&addr_len_cli);
	if(usfd_acc == -1){
		perror("accept()");
		close(usfd_li);
		unlink(UNIX_TCP_DOMAIN);
		return -1;
	}

	//阻塞读取一帧数据
	tmp = read(usfd_acc,&frame,sizeof(utcp_frame_t));
	if(tmp == -1)
		perror("read()");
	else{
		printf("%d byte msg from client(%d):\n",tmp,usfd_acc);

		//数据检查
		printf("type=%d,bak=%d,num=%d\n",frame.type,frame.bak,frame.num);
		printf("frame.num_arr:\n");
		for(tmp=0; tmp < 16; tmp++)
			printf("%d,",frame.num_arr[tmp]);
		printf("\nframe.num_arr2:\n");
		for(tmp=0; tmp < 8; tmp++)
			for(tmp2=0; tmp2 < 8; tmp2++)
				printf("%d,",frame.num_arr2[tmp][tmp2]);
		printf("\nframe.str: %s\n", frame.str);
	}

	//结束回收资源
	close(usfd_acc);
	close(usfd_li);
	unlink(UNIX_TCP_DOMAIN);
	return 0;
}







