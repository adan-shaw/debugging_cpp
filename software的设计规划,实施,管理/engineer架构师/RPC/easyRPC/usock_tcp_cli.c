//编译:
//		gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_srv.c -o x_srv; gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_cli.c -o x_cli 



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "unix_sock_frame.h"



int main(void){
	int tmp, sfd;
	struct sockaddr_un addr_srv;
	utcp_frame_t frame;



	//创建unix socket(SOCK_STREAM 流式)
	sfd = socket(AF_UNIX,SOCK_STREAM,0);
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
	strncpy(addr_srv.sun_path,UNIX_TCP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	//connect 链接到srv 端
	tmp = connect(sfd,(struct sockaddr*)&addr_srv,sizeof(addr_srv));
	if(tmp == -1){
		perror("connect()");
		close(sfd);
		return -1;
	}

	//阻塞发送一帧数据
	strncpy(frame.API,"f_adan", sizeof(frame.API));//有函数名: f_adan
	strncpy(frame.data,"taylor swift", sizeof(frame.data));//参数: taylor swift

	tmp = write(sfd,&frame,sizeof(utcp_frame_t));
	if(tmp == -1)
		perror("write()");
	else
		printf("%d byte msg to server(%d):\n",tmp,sfd);

	memset(frame.API, '\0', sizeof(frame.API));
	memset(frame.data, '\0', sizeof(frame.data));
	tmp = read(sfd,&frame,sizeof(utcp_frame_t));
	if(tmp == -1)
		perror("read()");
	else{
		printf("%d byte msg from server(%d):\n",tmp,sfd);
		printf("RPC return: %s\n",frame.data);
	}

	//结束回收资源
	close(sfd);
	return 0;
}





