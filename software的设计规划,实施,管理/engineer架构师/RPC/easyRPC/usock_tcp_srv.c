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



//RPC 函数f_adan
void rpc_f_adan(char* pret_buf, int pret_buf_len, const char* data){
	snprintf(pret_buf, pret_buf_len, "%s f_adan(), have a good day", data);
	//printf("RPC pret_buf: %s\n",pret_buf);
}

//RPC 函数f_shaw
void rpc_f_shaw(char* pret_buf, int pret_buf_len, const char* data){
	snprintf(pret_buf, pret_buf_len, "%s f_shaw(), have a good day", data);
}



int main(void){
	int usfd_li,usfd_acc;
	struct sockaddr_un addr_cli;
	socklen_t addr_len_cli;
	int tmp,tmp2;
	utcp_frame_t frame;
	char *pret_buf = malloc(1024);

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
		if(strncmp(frame.API,"f_adan", sizeof(frame.API)) == 0){
			printf("RPC call: %s\n",frame.API);
			memset(pret_buf, '\0', sizeof(pret_buf));
			rpc_f_adan(pret_buf, 1024, frame.data);//调用RPC 函数

			//返回RPC 执行结果
			memset(frame.API, '\0', sizeof(frame.API));
			memset(frame.data, '\0', sizeof(frame.data));
			strncpy(frame.API,"", sizeof(frame.API));//有函数名: ""
			strncpy(frame.data, pret_buf, strnlen(pret_buf, 1024));//参数: pret_buf
			//printf("RPC data: %s\n",frame.data);
			tmp = write(usfd_acc,&frame,sizeof(utcp_frame_t));
			if(tmp == -1)
				perror("write()");
			else
				printf("%d byte msg to client(%d):\n",tmp,usfd_acc);
			goto main_end;
		}

		if(strncmp(frame.API,"f_shaw", sizeof(frame.API)) == 0){
			//第二个RPC demo 函数, 不再演示
			goto main_end;
		}

	}

main_end:
	//结束回收资源
	free(pret_buf);
	close(usfd_acc);
	close(usfd_li);
	unlink(UNIX_TCP_DOMAIN);
	return 0;
}







