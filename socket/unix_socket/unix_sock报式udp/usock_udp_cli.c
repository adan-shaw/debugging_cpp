//编译:
//		gcc -g3 unix_sock_frame.h usock_udp_cli.c -o x_cli ; gcc -g3 unix_sock_frame.h usock_udp_srv.c -o x_srv 



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "unix_sock_frame.h"
#include "marr_copy.h"



int main(void){
	int tmp, sfd;
	struct sockaddr_un addr_srv;
	uudp_frame_t frame;



	//创建unix socket(SOCK_DGRAM 流式)
	sfd = socket(AF_UNIX,SOCK_DGRAM,0);
	if(sfd == -1){
		perror("socket()");
		return -1;
	}

	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));		//重用地址
	tmp = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(int));		//重用端口

	//填充unix socket srv 端地址信息
	addr_srv.sun_family = AF_UNIX;
	strncpy(addr_srv.sun_path,UNIX_UDP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	//阻塞发送一帧数据
	frame.type = 0;
	frame.bak = 0;
	frame.num = 0;
	memset(frame.num_arr,0xff,sizeof(frame.num_arr));
	memsetEx2(frame.num_arr2,0,sizeof(unsigned int),8,8);
	frame.num_arr2[3][3] = 3333;
	strncpy(frame.str,"hello world", sizeof(frame.str));

	tmp = sendto(sfd,&frame,sizeof(uudp_frame_t),0,(struct sockaddr*)&addr_srv,sizeof(addr_srv));
	if(tmp == -1)
		perror("sendto()");
	else
		printf("%d byte msg from client(%d):\n",tmp,sfd);

	//结束回收资源
	close(sfd);
	return 0;
}





