//编译
//		gcc ./poll监听一个fd.c -g3 -o x 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>

//求fd max
#define MAX_NUM(a,b) ( (a)>(b) ? (a) : (b) )

//s0.设置sfd nonblocking
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}



//只监听一个fd
void select_test_tcp(void);



int main(void){
	select_test_tcp();
	return 0;
}



//只监听一个fd
void select_test_tcp(void){
	int sv[2];
	int tmp, count = 2;
	pid_t id;
	char buf[32];
	struct timeval timeout;
	fd_set rset;
	struct pollfd poll_fd;


	//创建socketpair() socket 对, sv[0],sv[1] socket 关联sfd组
	tmp = socketpair(PF_LOCAL,SOCK_STREAM,0,sv);
	if(tmp == -1){
		perror("socketpair()");
		return ;
	}

	//执行fork() 创建子进程
	id = fork();
	if(id == -1){
		perror("fork()");
		close(sv[0]);
		close(sv[1]);
		return;
	}
	if(id == 0){					//子进程
		//close(sv[0]);			//关闭一个端, 是sv[0],sv[1]都是可写可读的
		close(sv[1]);				//子进程-关闭sv[1]

		__set_nonblocking(sv[0]);										//select()异步模式
		poll_fd.fd = sv[0];
		poll_fd.events = POLLIN;

		while(count > 0){
			tmp = poll(&poll_fd,1,-1);
			if(tmp > 0){															//poll调用ok
				if(poll_fd.revents & POLLIN){						//判断pfds[0] 是否有'io读事件'
					tmp = read(sv[0],&buf,sizeof(buf));
					if(tmp > 0){
						printf("tcp: read() from 父亲(%d): \n%s\n",tmp,buf);
						count--;
					}
				}
			}
			else if(tmp == 0)													//select(): 超时
				continue;
			else{																			//poll(): 错误(tmp == -1)
				perror("poll() failed");
				break;
			}
		}
		close(sv[0]);				//只接收一个回覆便退出
		//close(sv[1]);			//子进程-关闭sv[1]
		exit(EXIT_SUCCESS);	//子进程结束
	}
	else{									//父进程
		//close(sv[1]);
		close(sv[0]);				//父进程-关闭sv[0]

		strncpy(buf,"我是父亲\n",sizeof(buf));

		write(sv[1],&buf,sizeof(buf));							//阻塞write()
		write(sv[1],&buf,sizeof(buf));							//阻塞write()

		close(sv[1]);				//只接收一个回覆便退出
		//close(sv[0]);			//父进程-关闭sv[0]

		sleep(1);
	}
	return;
}



