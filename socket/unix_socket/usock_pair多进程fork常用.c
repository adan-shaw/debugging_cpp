//编译:
//		gcc -g3 -o x ./usock_pair多进程fork常用.c


//socketpair()创建了一对'无名的'套接字描述符(只能在AF_UNIX域中使用)
/*
	Linux实现了一个源自BSD的socketpair调用,
	在Linux中, socketpair() SOCK_STREAM模式时, 类似于pipe,
	唯一的区别就是这一对文件描述符中的任何一个都可读和可写.

	函数介绍:
		socketpair()函数建立一对匿名的已经连接的套接字,
								其特性由协议族d、类型type、协议protocol决定,
								建立的两个套接字描述符会放在sv[0]和sv[1]中.
		成功返回0, 失败返回-1.

	参数介绍:
		第1个参数d, 表示协议族, 只能为AF_LOCAL或者AF_UNIX;
		第3个参数protocol, 表示协议, 只能为0.
		第2个参数type, 表示类型, 可以是SOCK_STREAM或者SOCK_DGRAM.
			SOCK_STREAM = 无数据边界的数据流, 类似于pipe
			SOCK_DGRAM = 有数据边界的数据报(一般不会用)

		用SOCK_STREAM建立的'unix 套接字对'是管道流, 与'普通管道'相区别的是:
			套接字对建立的通道是双向的, 即每一端都可以进行读写.
			参数sv, 是一个引用返回参数, 用于保存建立的套接字对.
			而'普通管道'是单相工, 单向的.


	fd描述符存储于一个二元一维数组,例如sv[2].
	这对套接字可以进行双工通信, 每一个描述符既可以读也可以写.
	这个在同一个进程中也可以进行通信,
	向sv[0]中写入, 就可以从sv[1]中读取(只能从sv[1]中读取),
	也可以在sv[1]中写入, 然后从sv[0]中读取;

	但是, 若没有在0端写入, 而从1端读取,
	则1端的读取操作会阻塞, 即使在1端写入, 也不能从1读取, 仍然阻塞;


	socketpair 只用做传输数据, 并不能进行数据同步,
	socketpair 数据同步也一般不用信号量, 信号量一般面向'共享内存'.
	socketpair 数据同步,一般使用信号,因为不是构建临界区,
						 而是通知对方去取数据, 用信号函数比较好.
	socketpair 一般只用作fork() 之间通信
*/



#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


#define USOCK_BUF_MAX (64)

struct usock_tcp_t{
	short int type;								//信息类型编号
	short int buf_len;						//信息长读buf_len
	char buf[USOCK_BUF_MAX];
};

//1.socketpair() 简单双向通信测试 -- 传递数据结构体
void socketpair_test_tcp(void);

//2.socketpair() 简单双向通信测试
void socketpair_test_udp(void);



int main(void){
	printf("1.socketpair_test_tcp() 简单双向通信测试 -- 传递数据结构体\n");
	socketpair_test_tcp();
	sleep(3);
	printf("\n\n\n");

	printf("2.socketpair_test_udp() 简单双向通信测试\n");
	socketpair_test_udp();
	return 0;
}



//2.socketpair() 简单双向通信测试 -- 传递数据结构体
void socketpair_test_tcp(void){
	int sv[2];
	int tmp;
	pid_t id;
	const char *msg2son = "我是父亲\n";
	const char *msg2father = "我是孩子\n";
	struct usock_tcp_t mbuf,mbuf2,re_mbuf,re_mbuf2;
	ssize_t size;



	//0.填充usock_tcp_t 数据
	mbuf.type = 1;
	mbuf.buf_len = 1;
	memset(mbuf.buf,'\0',sizeof(mbuf.buf));
	strncpy(mbuf.buf,msg2father,sizeof(mbuf.buf));

	mbuf2.type = 1;
	mbuf2.buf_len = 999;
	memset(mbuf2.buf,'\0',sizeof(mbuf2.buf));
	strncpy(mbuf2.buf,msg2son,sizeof(mbuf2.buf));


	//1.创建socketpair() socket 对, sv[0],sv[1] socket 关联sfd组
	tmp = socketpair(PF_LOCAL,SOCK_STREAM,0,sv);
	if(tmp == -1){
		perror("socketpair() failed");
		return ;
	}

	//3.执行fork() 创建子进程
	id = fork();
	if(id == -1){
		perror("fork() failed");
		close(sv[0]);
		close(sv[1]);
		return;
	}
	if(id == 0){			//子进程
		//close(sv[0]);	//关闭一个端, 是sv[0],sv[1]都是可写可读的
		close(sv[1]);		//子进程-关闭sv[1]

		while(1){
			write(sv[0],&mbuf2,sizeof(mbuf2));//阻塞write()
			write(sv[0],&mbuf2,sizeof(mbuf2));//阻塞write()
			write(sv[0],&mbuf2,sizeof(mbuf2));//阻塞write()
			sleep(1);

			memset(&re_mbuf2,0,sizeof(re_mbuf2));
			size = read(sv[0],&re_mbuf2,sizeof(re_mbuf2)*3);//阻塞read()
			if(size > 0){
				printf("tcp: read() from 父亲 : \n%s\n",re_mbuf2.buf);
				printf("tcp: type = %d, buf_len = %d\n",re_mbuf2.type,re_mbuf2.buf_len);
				close(sv[0]);		//只接收一个回覆便退出
				//close(sv[1]);	//子进程-关闭sv[1]
				break;
			}
		}
		exit(EXIT_SUCCESS);
	}
	else{						//父进程
		//close(sv[1]);
		close(sv[0]);	//父进程-关闭sv[0]

		while(1){
			memset(&re_mbuf,0,sizeof(re_mbuf));
			size = read(sv[1],&re_mbuf,sizeof(re_mbuf)*2);//阻塞read()
			if(size > 0){
				printf("tcp: read() from 孩子 : \n%s\n",re_mbuf.buf);
				printf("tcp: type = %d, buf_len = %d\n",re_mbuf.type,re_mbuf.buf_len);
				sleep(1);
			}
			write(sv[1],&mbuf,sizeof(mbuf));//阻塞write()
			write(sv[1],&mbuf,sizeof(mbuf));//阻塞write()
			write(sv[1],&mbuf,sizeof(mbuf));//阻塞write()
			close(sv[1]);		//只接收一个回覆便退出
			//close(sv[0]);	//父进程-关闭sv[0]
			break;
		}
	}
	return;
}





//3.socketpair() 简单双向通信测试
void socketpair_test_udp(void){
	int sv[2];
	int tmp;
	pid_t id;
	const char *msg2son = "我是父亲\n";
	const char *msg2father = "我是孩子\n";
	struct usock_tcp_t mbuf,mbuf2,re_mbuf,re_mbuf2;
	ssize_t size;



	//0.填充usock_tcp_t 数据
	mbuf.type = 1;
	mbuf.buf_len = 1;
	memset(mbuf.buf,'\0',sizeof(mbuf.buf));
	strncpy(mbuf.buf,msg2father,sizeof(mbuf.buf));

	mbuf2.type = 1;
	mbuf2.buf_len = 999;
	memset(mbuf2.buf,'\0',sizeof(mbuf2.buf));
	strncpy(mbuf2.buf,msg2son,sizeof(mbuf2.buf));


	//1.创建socketpair() socket 对, sv[0],sv[1] socket 关联sfd组
	tmp = socketpair(PF_LOCAL,SOCK_DGRAM,0,sv);
	if(tmp == -1){
		perror("socketpair() failed");
		return ;
	}


	//2.执行fork() 创建子进程
	id = fork();
	if(id == -1){
		perror("fork() failed");
		close(sv[0]);
		close(sv[1]);
		return;
	}
	if(id == 0){			//子进程
		//close(sv[0]);	//关闭一个端, 是sv[0],sv[1]都是可写可读的
		close(sv[1]);		//子进程-关闭sv[1]

		while(1){
			write(sv[0],&mbuf2,sizeof(mbuf2));						//阻塞write()
			sleep(1);
			memset(&re_mbuf2,0,sizeof(re_mbuf2));
			size = read(sv[0],&re_mbuf2,sizeof(re_mbuf2));//阻塞read()
			if(size > 0){
				printf("udp: read() from 父亲 : \n%s\n",re_mbuf2.buf);
				printf("udp: type = %d, buf_len = %d\n",re_mbuf2.type,re_mbuf2.buf_len);
				close(sv[0]);		//只接收一个回覆便退出
				//close(sv[1]);	//子进程-关闭sv[1]
				break;
			}
		}
		exit(EXIT_SUCCESS);
	}
	else{						//父进程
		//close(sv[1]);
		close(sv[0]);	//父进程-关闭sv[0]
		while(1){
			memset(&re_mbuf,0,sizeof(re_mbuf));
			size = read(sv[1],&re_mbuf,sizeof(re_mbuf));//阻塞read()
			if(size > 0){
				printf("udp: read() from 孩子 : \n%s\n",re_mbuf.buf);
				printf("udp: type = %d, buf_len = %d\n",re_mbuf.type,re_mbuf.buf_len);
				sleep(1);
			}
			write(sv[1],&mbuf,sizeof(mbuf));						//阻塞write()
			close(sv[1]);		//只接收一个回覆便退出
			//close(sv[0]);	//父进程-关闭sv[0]
			break;
		}
	}

	return;
}
