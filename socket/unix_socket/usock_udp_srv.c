//编译:
//		gcc -g3 -o x_cli ./usock_udp_cli.c; gcc -g3 -o x_srv ./usock_udp_srv.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>



//'客户端-服务端'约定的unix socket文件名
#define UNIX_DOMAIN "/tmp/UNIX.domain"

//由于unix socket几乎不用等待,这个值可以很小,8/16 都可以
#define LISTEN_LIST_MAX (8)

/*
	由于socket默认接收缓冲区是8196, 一般不要超出8196比较恰当;
	否则你需要修改socket默认缓冲区大小, 否则udp读报可能会出现缓冲区溢出丢失.
	udp和tcp不一样, socket缓冲区机制, 详情请看tcp/ip 协议详解
*/
#define USOCK_UDP_BUF_MAX (4096)
#define USOCK_UDP_PACKAGE_MAX (8192)

//usock_tcp 消息结构体
/*
	* unix socket 传递消息, 不支持void*内嵌;
		不同进程之间,没办法共享堆内存,别浪费心血想办法共享void*指针了;
	* 本机多进程通信, 可以忽略大小端字节序的问题, 因此可以直接传递整形or 浮点数据;
	* unix socket 约定的文件名, 多进程访问时, 并不需要加锁/加信号灯, 但需要先注册, 再访问;
*/
struct usock_udp_t{
	short int type;								//信息类型编号
	short int buf_len;						//信息长读buf_len
	char buf[USOCK_TCP_BUF_MAX];
};



//启动unix socket srv端(SOCK_DGRAM 报式socket)
void start_srv(void){
	int usfd_li,usfd_acc;
	struct sockaddr_un srv_addr_info;
	struct sockaddr_un cli_addr_info;
	socklen_t cli_addr_info_len;
	int tmp;
	const char* msg = "i'm the unix_sock_srv\n";
	usock_udp_t re_mbuf;



	//1.创建unix socket
	usfd_li = socket(PF_UNIX,SOCK_DGRAM,0);
	if(usfd_li<0){
		perror("socket() failed");
		return;
	}

	//2.set unix socket srv端的addr 信息
	srv_addr_info.sun_family=AF_UNIX;
	strncpy(srv_addr_info.sun_path,UNIX_DOMAIN,sizeof(srv_addr_info.sun_path)-1);

	//3.如果unix socket key-name 已经注册, unlink 取消它
	unlink(UNIX_DOMAIN);

	//4.bind() 绑定unix socket 与unix 地址信息
	tmp = bind(usfd_li,(struct sockaddr*)&srv_addr_info,sizeof(srv_addr_info));
	if(tmp == -1){
		perror("bind() failed");
		close(usfd_li);
		unlink(UNIX_DOMAIN);
		return;
	}

	//5.读取client 发送过来的一条信息(阻塞读取)
	printf("\n=====mess from client=====\n");
	//memset(&re_mbuf,'\0',sizeof(re_mbuf));
	tmp = recvfrom(usfd_li,&re_mbuf,sizeof(re_mbuf),0,NULL,NULL);
	if(tmp == -1)
		perror("recvfrom() failed(recvfrom() from client)");
	else{
		printf("Message recv to client(%d):\n	%s\n",tmp,re_mbuf.buf);
		printf("type = %d, id = %d\n",re_mbuf.type,re_mbuf.id);
		printf("sizeof(mbuf) = %d\n",sizeof(re_mbuf));
	}
	shutdown(usfd_acc,2);
	close(usfd_acc);

	//6.结束回收资源
	shutdown(usfd_li,2);
	close(usfd_li);
	unlink(UNIX_DOMAIN);
	return ;
}



int main(void){
	start_srv();
	return 0
}
