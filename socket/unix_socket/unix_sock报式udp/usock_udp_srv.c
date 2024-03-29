//编译:
//		gcc -g3 unix_sock_frame.h usock_udp_cli.c -o x_cli ; gcc -g3 unix_sock_frame.h usock_udp_srv.c -o x_srv 



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "unix_sock_frame.h"



//创建一个unix socket listener
int unix_sock_udp_listener(void){
	int sfd, tmp;
	struct sockaddr_un addr_srv;

	//1.创建unix socket(SOCK_DGRAM 流式)
	sfd = socket(AF_UNIX,SOCK_DGRAM,0);
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
	strncpy(addr_srv.sun_path,UNIX_UDP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	//3.如果unix socket key-name 已经注册, unlink 取消它
	unlink(UNIX_UDP_DOMAIN);

	//4.bind() 绑定unix socket 与unix 地址信息
	if(bind(sfd,(struct sockaddr*)&addr_srv,sizeof(addr_srv)) == -1){
		perror("bind()");
		close(sfd);
		unlink(UNIX_UDP_DOMAIN);
		return -1;
	}

	return sfd;
}



int main(void){
	int usfd_li,usfd_acc;
	struct sockaddr_un addr_cli;
	socklen_t addr_len_cli;
	int tmp,tmp2;
	uudp_frame_t frame;

	//创建一个unix socket listener
	usfd_li = unix_sock_udp_listener();
	if(usfd_li == -1)
		return -1;

	//阻塞读取一帧数据
	tmp = recvfrom(usfd_li,&frame,sizeof(uudp_frame_t),0,NULL,NULL);
	if(tmp == -1)
		perror("recvfrom()");
	else{
		printf("%d byte msg from client(%d):\n",tmp,usfd_li);

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
	close(usfd_li);
	unlink(UNIX_UDP_DOMAIN);
	return 0;
}







