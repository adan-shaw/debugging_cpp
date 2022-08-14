//编译:
//		gcc -g3 -o x_cli ./usock_tcp_cli.c; gcc -g3 -o x_srv ./usock_tcp_srv.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>



//'客户端-服务端'约定的unix socket文件名
#define UNIX_DOMAIN "/tmp/UNIX.domain"

#define USOCK_TCP_BUF_MAX (4096)
#define USOCK_TCP_PACKAGE_MAX (8192)

//usock_tcp 消息结构体
/*
	* unix socket 传递消息, 不支持void*内嵌;
		不同进程之间,没办法共享堆内存,别浪费心血想办法共享void*指针了;
	* 本机多进程通信, 可以忽略大小端字节序的问题, 因此可以直接传递整形or 浮点数据;
	* unix socket 约定的文件名, 多进程访问时, 并不需要加锁/加信号灯, 但需要先注册, 再访问;
*/
struct usock_tcp_t{
	short int type;								//信息类型编号
	short int buf_len;						//信息长读buf_len
	char buf[USOCK_TCP_BUF_MAX];
};



//启动unix socket cli端, 链接到srv端(SOCK_STREAM 流式socket)
void start_cli(void){
	int usfd_conn;
	struct sockaddr_un srv_addr_info;
	int tmp;
	char snd_buf[1024];



	//1.创建unix socket(SOCK_STREAM 流式)
	usfd_conn = socket(PF_UNIX,SOCK_STREAM,0);
	if(usfd_conn == -1){
		perror("socket() failed");
		return;
	}

	//2.填充unix socket srv 端地址信息
	srv_addr_info.sun_family = AF_UNIX;
	strncpy(srv_addr_info.sun_path,UNIX_DOMAIN,sizeof(srv_addr_info.sun_path)-1);

	//3.connect 链接到srv 端
	tmp = connect(usfd_conn,(struct sockaddr*)&srv_addr_info,sizeof(srv_addr_info));
	if(tmp == -1){
		perror("connect() failed");
		close(usfd_conn);
		return ;
	}

	//4.send mes to server 发送一次数据到srv端(阻塞发送)
	//memset(snd_buf,'\0',sizeof(snd_buf));
	strncpy(snd_buf,"hello, the unix socket server",sizeof(snd_buf));
	tmp = write(usfd_conn,snd_buf,sizeof(snd_buf));
	if(tmp == -1)
		perror("write() failed(write to server)");
	else
		printf("Message send to server(%d):\n	%s\n",tmp,snd_buf);

	//5.结束回收资源
	shutdown(usfd_conn,2);
	close(usfd_conn);
	return ;
}



int main(void){
	start_cli();
	return 0;
}
