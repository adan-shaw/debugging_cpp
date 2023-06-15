//编译:
//		gcc ./mque_posix复杂.c -lrt -g3 -o x 

//查看系统对mqueue posix 的限制:
/*
	//消息队列的消息数量max, 等价于mq_maxmsg, 默认是10
	cat /proc/sys/fs/mqueue/msg_max

	//消息队列中, 每条消息的大小max, 等价于mq_msgsize, 默认是8192
	cat /proc/sys/fs/mqueue/msgsize_max

	//系统中可以创建多少个消息队列, 默认是512
	cat /proc/sys/fs/mqueue/queues_max

	mqueue消息队列创建的参数, 必须与系统配置的匹配, 否则容易出错!!
	要修改mqueue消息队列的参数, 必须先修改系统配置!!
	否则极度容易报错:
		mq_receive(): Message too long
		(消息过大, 让人摸不着头脑, 很烧脑)

	接收缓冲区, 必须足够容纳整条消息, 小一点都会报错!!
	(毫无讨价还价的余地)不能缩减消息接收buf 的大小, 必须规正为:
		#define MQUEUE_MSG_MAX (struct mq_attr{}->mq_msgsize)
		char buf[MQUEUE_MSG_MAX];
*/

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


#define MQUEUE_NAME "/mq_test"
#define MQUEUE_MODE (0600)
#define MQUEUE_SIGNAL (64)
#define MQUEUE_MAX (10)
#define MQUEUE_MSG_MAX (8192)

//结构体app_body{}
typedef struct app_body{
	int i1;
	double d2;
	char c3[64];//多余的字节位,继续写入; 虽然不能做到最小化拷贝,但胜在简单,数据对齐,拷贝速度快;
}body_t;


//为了方便信号函数共用G_mque_fd, 所以设置为全局变量
mqd_t G_mque_fd = -1;


//注册64 号实时信号(作为posix消息队列的通知)
void sig_realtime64(int sig){
	int tmp, prio;
	char buf_recv[MQUEUE_MSG_MAX];
	body_t body_buf;
	tmp=mq_receive(G_mque_fd,buf_recv,MQUEUE_MSG_MAX,&prio);
	if(tmp == -1){												//阻塞接收mque消息
		perror("mq_receive()");
		mq_close(G_mque_fd);
		mq_unlink(MQUEUE_NAME);
	}
	else{
		printf("sig_realtime64(%d)->[priority%ld]mq_receive():\n",sig,prio);
		memcpy(&body_buf,buf_recv,sizeof(body_buf));
		printf("body_t body_buf={%d,%lf,%s}\n",body_buf.i1,body_buf.d2,body_buf.c3);
	}
}



void mqueue_posix_test(void){
	int mque_flag, tmp, prio;
	struct mq_attr attr;
	struct sigevent sigev;
	body_t buf_send;
	char buf_recv[MQUEUE_MSG_MAX];
	body_t body_buf;
	struct timespec timeout_send,timeout_recv;


	mque_flag = O_CREAT|O_EXCL|O_RDWR;		//与attr.mq_flags一致
	attr.mq_flags = O_CREAT|O_EXCL|O_RDWR;//消息队列flag(后续可修改)
	attr.mq_maxmsg = MQUEUE_MAX;					//消息队列的消息数量上限,必须大于0(后续不可修改)
	attr.mq_msgsize = MQUEUE_MSG_MAX;			//每条消息的max大小,必须大于0(后续不可修改)
	attr.mq_curmsgs = 0;									//设置当前消息队列中消息数量(后续可修改)

	G_mque_fd = mq_open(MQUEUE_NAME, mque_flag, MQUEUE_MODE, NULL);
	if(G_mque_fd == -1){
		perror("mq_open()");
		return;
	}

	if(mq_getattr(G_mque_fd, &attr) == -1){//获取消息队列属性
		perror("mq_getattr()");
		goto mqueue_posix_test_failed;
	}


	if(signal(MQUEUE_SIGNAL, sig_realtime64) == SIG_ERR){
		perror("signal()");									//激活信号函数
		goto mqueue_posix_test_failed;
	}

	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = MQUEUE_SIGNAL;
	if(mq_notify(G_mque_fd, &sigev) == -1){
		perror("mq_notify()");							//将mque_posix 与64实时信号绑定
		goto mqueue_posix_test_failed;
	}

	buf_send.i1 = 999;
	buf_send.d2 = 888.888;
	strncpy(buf_send.c3, "you idiot, lover!!", sizeof("you idiot, lover!!"));

	tmp = mq_send(G_mque_fd, &buf_send, sizeof(buf_send), 0);
	if(tmp == -1){												//阻塞发送mque消息, 优先级=0
		perror("mq_send()");								//(要用异步,只能在mqueue创建时修改属性,后续不能修改)
		goto mqueue_posix_test_failed;
	}
	printf("msg_len=%d, mq_send()=%d\n",sizeof(buf_send),tmp);//for test only

	timeout_send.tv_sec = 1;
	timeout_send.tv_nsec = 0;
	tmp = mq_timedsend(G_mque_fd, &buf_send, sizeof(buf_send), 1, &timeout_send);
	if(tmp == -1){												//超时发送mque消息(阻塞), 优先级=1
		perror("mq_timedsend()");
		if(errno == EAGAIN)
			printf("mq_timedsend() timeout!!");
		else
			goto mqueue_posix_test_failed;
	}

	timeout_recv.tv_sec = 1;
	timeout_recv.tv_nsec = 0;							//(1秒=10^9纳秒)
	tmp=mq_timedreceive(G_mque_fd,buf_recv,MQUEUE_MSG_MAX,&prio,&timeout_recv);
	if(tmp == -1){												//超时接收mque消息(阻塞)
		perror("mq_timedreceive()");
		if(errno == EAGAIN)
			printf("mq_timedreceive() timeout!!");
		else
			goto mqueue_posix_test_failed;
	}
	else{
		printf("[priority%ld]mq_timedreceive():\n",prio);
		memcpy(&body_buf,buf_recv,sizeof(body_buf));
		printf("body_t body_buf={%d,%lf,%s}\n",body_buf.i1,body_buf.d2,body_buf.c3);
	}

mqueue_posix_test_failed:
	mq_close(G_mque_fd);
	mq_unlink(MQUEUE_NAME);
	return;
}


int main(void){
	mqueue_posix_test();
	return 0;
}




