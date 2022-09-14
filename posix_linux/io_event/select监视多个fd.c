//编译
//		gcc ./select监视多个fd.c -g3 -w -o x 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/select.h>



//'客户端&服务端'约定的unix socket文件名
#define UNIX_TCP_DOMAIN "/tmp/UNIX_TCP.domain"

//由于unix socket几乎不用等待,这个值可以很小,8/16 都可以
#define UNIX_TCP_LISTEN_MAX (8)



//fds buf max(最多用select 监视n 个fd)
//[一般select 只监视1个fd, 也有监视几个fd 的需求, client 很常用]
#define MAX_FD_BUF (8)

//求fd max
#define MAX_NUM(a,b) ( (a)>(b) ? (a) : (b) )

//s0.设置sfd nonblocking
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}

//unix socket 对的sfd 池
static int usfd_li = -1;									//监视sfd
static int fds[2][MAX_FD_BUF];						//fds[0][]=客户端sfd, fds[1][]=服务器接待者
static unsigned short fds_count = 0;			//实际初始化的unix socket对(应当等于MAX_FD_BUF)

//函数前置声明
void select_test(void);
int get_ready_all_sock(void);
void get_ready_all_sock_failed(void);
int unix_sock_tcp_listener(void);
int unix_sock_tcp_connecter(void);


int main(void){
	select_test();
	return 0;
}



//主逻辑
void select_test(void){
	int tmp, tmp2, count, sfd_max;
	char buf[32];
	pid_t id;
	struct timeval timeout;
	fd_set rset;

	//准备所有的socket: srv & cli
	if(get_ready_all_sock() == -1)
		return;

	FD_ZERO(&rset);

	//执行fork()
	id = fork();
	if(id == -1){
		perror("fork()");
		return;
	}
	if(id == 0){//子进程
		timeout.tv_usec = 0;											//填充timeout 结构体
		timeout.tv_sec = 0;

		for(sfd_max = 0,tmp = 0; tmp < MAX_FD_BUF; tmp++)
			sfd_max = MAX_NUM(sfd_max,fds[0][tmp]);	//计算出最大的sfd 值

		for(tmp = 0; tmp < MAX_FD_BUF; tmp++)//for test only(打印关键信息)
			printf("sfd_max=%d,fds[0][%d]=%d\n",sfd_max,tmp,fds[0][tmp]);

		count = 0;
		sleep(1);																	//等父进程开始后, 再执行
		while(1){
			/*
				这里很有意思,如果不执行count++,父进程退出之后,
				子进程被init进程收养之后,就会在init进程的监管下,继续陷入死循环, 
				这样子会很卡机, 永远退出不了, 一定不停死循环里面的操作;
			*/
			//printf("%d\n",count++);
			if(count++ > 1024)											//最多循环1024次
				break;

			for(tmp = 0; tmp < MAX_FD_BUF; tmp++)		//将所有客户端sfd 压入fd_set
				FD_SET(fds[0][tmp],&rset);

			tmp = select(sfd_max+1,&rset,NULL,NULL,&timeout);
			if(tmp > 0){														//select(): 有io 事件
				for(tmp = 0; tmp < MAX_FD_BUF; tmp++){
					if(FD_ISSET(fds[0][tmp],&rset)){		//sfd 还在fd_set 中, 表明有io 事件发生
						tmp2 = read(fds[0][tmp],&buf,sizeof(buf));
						if(tmp2 > 0)
							printf("tcp-%d: read() from 父亲(%d bit): \n%s\n",fds[0][tmp],tmp2,buf);
					}
				}
			}
			else if(tmp == 0)												//select(): 超时
				continue;
			else{																		//select(): 错误(tmp == -1)
				perror("select()");
				break;
			}
		}//while end
	}
	else{				//父进程
		strncpy(buf,"我是父亲\n",sizeof(buf));
		for(tmp = 0; tmp < MAX_FD_BUF; tmp++)
			write(fds[1][tmp],&buf,sizeof(buf));		//阻塞write()
		for(tmp = 0; tmp < MAX_FD_BUF; tmp++)
			write(fds[1][tmp],&buf,sizeof(buf));
		sleep(3);
	}

	get_ready_all_sock_failed();								//回收资源
	return;
}



//准备所有的socket: srv & cli
int get_ready_all_sock(void){
	int tmp;
	struct sockaddr_un addr_cli;
	socklen_t addr_len_cli;

	memset(fds,-1,MAX_FD_BUF*2*sizeof(int));

	//创建一个unix socket listener
	usfd_li = unix_sock_tcp_listener();
	if(usfd_li == -1)
		return -1;

	//创建unix socket 对
	for(tmp = 0; tmp < MAX_FD_BUF; tmp++){
		fds[0][tmp] = unix_sock_tcp_connecter();
		if(fds[0][tmp] == -1){
			get_ready_all_sock_failed();
			return -1;
		}

		fds[1][tmp] = accept(usfd_li,(struct sockaddr*)&addr_cli,&addr_len_cli);
		if(fds[1][tmp] == -1){
			perror("accept()");
			get_ready_all_sock_failed();
			return -1;
		}
		fds_count++;
	}
	return 0;
}



void get_ready_all_sock_failed(void){
	int tmp;
	if(usfd_li != -1){
		shutdown(usfd_li,2);
		close(usfd_li);
		unlink(UNIX_TCP_DOMAIN);
	}
	//清空所有unix socket对
	for(tmp = 0; tmp < MAX_FD_BUF; tmp++){
		if(fds[0][tmp] != -1){
			shutdown(fds[0][tmp],2);
			close(fds[0][tmp]);
		}
		if(fds[1][tmp] != -1){
			shutdown(fds[1][tmp],2);
			close(fds[1][tmp]);
		}
	}
	return;
}



//创建一个unix socket listener
int unix_sock_tcp_listener(void){
	int usfd_li;
	struct sockaddr_un addr_srv;

	//1.创建unix socket(SOCK_STREAM 流式)
	usfd_li = socket(PF_UNIX,SOCK_STREAM,0);
	if(usfd_li == -1){
		perror("socket()");
		return 0;
	}

	//2.set unix socket srv端的addr 信息
	addr_srv.sun_family=AF_UNIX;
	strncpy(addr_srv.sun_path,UNIX_TCP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	//3.如果unix socket key-name 已经注册, unlink 取消它
	unlink(UNIX_TCP_DOMAIN);

	//4.bind() 绑定unix socket 与unix 地址信息
	if(bind(usfd_li,(struct sockaddr*)&addr_srv,sizeof(addr_srv)) == -1){
		perror("bind()");
		close(usfd_li);
		unlink(UNIX_TCP_DOMAIN);
		return 0;
	}

	//5.listen() 开始监视
	listen(usfd_li,UNIX_TCP_LISTEN_MAX);
	return usfd_li;
}



//创建一个cli socket, 链接到srv
int unix_sock_tcp_connecter(void){
	int usfd_conn;
	struct sockaddr_un addr_srv;

	usfd_conn = socket(PF_UNIX,SOCK_STREAM,0);
	if(usfd_conn == -1){
		perror("socket()");
		return -1;
	}

	addr_srv.sun_family = AF_UNIX;
	strncpy(addr_srv.sun_path,UNIX_TCP_DOMAIN,sizeof(addr_srv.sun_path)-1);

	if(connect(usfd_conn,(struct sockaddr*)&addr_srv,sizeof(addr_srv)) == -1){
		perror("connect()");
		close(usfd_conn);
		return -1;
	}

	//客户端nonblocking
	__set_nonblocking(usfd_conn);
	return usfd_conn;
}
