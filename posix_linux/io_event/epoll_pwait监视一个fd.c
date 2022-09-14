//编译
//		gcc ./epoll_pwait监视一个fd.c -g3 -o x 

//epoll_pwait() 的应用场景不多

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/time.h>


//fds buf max(最多用epoll 监视n 个fd)
//[一般epoll 只监视1个fd, 也有监视几个fd 的需求, client 很常用]
#define MAX_FD_BUF (8)

//求fd max
#define MAX_NUM(a,b) ( (a)>(b) ? (a) : (b) )

//s0.设置sfd nonblocking
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}



//SIGALRM 信号处理函数
void print_sig_type(int sig){
	(sig == SIGALRM) ? printf("signal() SIGALRM=%d\n",sig) : printf("unknow signal\n") ;
}

//只监视一个fd
void epoll_test_tcp(void);



int main(void){
	epoll_test_tcp();
	return 0;
}



//只监视一个fd
void epoll_test_tcp(void){
	int sv[2];
	int tmp, count = 2;
	int epfd, epfd_alive_count;
	pid_t id;
	char buf[32];
	sigset_t sig_set;
	struct itimerval timer_val;
	struct epoll_event ev,*pevs;



	signal(SIGALRM,print_sig_type);								//启用SIGALRM信号

	timer_val.it_interval.tv_sec = 1;
	timer_val.it_interval.tv_usec = 0;
	timer_val.it_value.tv_sec = 1;
	timer_val.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL,&timer_val,NULL);				//启用定时器

	sigemptyset(&sig_set);												//清空信号集
	sigaddset(&sig_set,SIGALRM);									//向信号集中添加SIGALRM 信号

	//创建socketpair() socket 对, sv[0],sv[1] socket 关联sfd组
	tmp = socketpair(PF_LOCAL,SOCK_STREAM,0,sv);
	if(tmp == -1){
		perror("socketpair()");
		return ;
	}

	//创建epoll映射块
	epfd = epoll_create(MAX_FD_BUF);
	if(epfd == -1){
		perror("epoll_create()");
		return;
	}
	epfd_alive_count = 0;												//epfd 计数器=0(初始化)
	//初始化epoll_wait() 返回的容器
	pevs=malloc(sizeof(struct epoll_event)*MAX_FD_BUF);

	//执行fork() 创建子进程
	id = fork();
	if(id == -1){
		perror("fork()");
		close(sv[0]);
		close(sv[1]);
		free(pevs);
		return;
	}
	if(id == 0){					//子进程
		//close(sv[0]);			//关闭一个端, 是sv[0],sv[1]都是可写可读的
		close(sv[1]);				//子进程-关闭sv[1]

		//__set_nonblocking(sv[0]);										//epoll()异步模式
		ev.data.fd = sv[0];
		ev.events = EPOLLIN|EPOLLPRI;
		epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);	//把fd 加入epfd 映射块中
		epfd_alive_count++;

		while(count > 0){
			tmp = epoll_pwait(epfd,pevs,epfd_alive_count,-1,&sig_set);
			if(tmp > 0){															//epoll(): 有io 事件
				tmp = read(sv[0],&buf,sizeof(buf));
				if(tmp > 0){
					printf("tcp: read() from 父亲(%d bit): \n%s\n",tmp,buf);
					count--;
				}
			}
			else if(tmp == 0){												//epoll(): 超时
				count--;
				continue;
			}
			else{																			//epoll(): 错误(tmp == -1)
				perror("epoll_pwait()");
				break;
			}
		}
		close(sv[0]);				//只接收一个回覆便退出
		//close(sv[1]);			//子进程-关闭sv[1]
		free(pevs);
		exit(EXIT_SUCCESS);	//子进程结束
	}
	else{									//父进程
		//close(sv[1]);
		close(sv[0]);				//父进程-关闭sv[0]

		//strncpy(buf,"我是父亲\n",sizeof(buf));
		//write(sv[1],&buf,sizeof(buf));						//不写入任何数据, 让信号中断epoll_pwait()
		//write(sv[1],&buf,sizeof(buf));

		close(sv[1]);				//只接收一个回覆便退出
		//close(sv[0]);			//父进程-关闭sv[0]

		sleep(16);
	}
	free(pevs);
	return;
}


