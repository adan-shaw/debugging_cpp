//编译:
//		gcc ./mque_posix简单.c -lrt -g3 -o x 

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

//结构体:
/*
	typedef int mqd_t;

	//[一旦mq_open()确定了多大的消息队列,后面无法通过mq_setattr()修改]
	struct mq_attr {
		long mq_flags;  //消息队列flag(后续可修改)
		long mq_maxmsg; //消息队列的消息数量上限,必须大于0(后续不可修改)
		long mq_msgsize;//每条消息的max大小,必须大于0(后续不可修改)
		long mq_curmsgs;//设置当前消息队列中消息数量(后续可修改)
	};
*/

//函数原型:
/*
	mq_open():    创建/打开一个'msg消息队列'
	mq_send():    向'msg消息队列'写入一条信息
	mq_receive(): 从'msg消息队列'读取一条信息
	mq_close():   关闭进程的'msg消息队列'mqd_t
	mq_unlink():  解除当前进程与'msg消息队列'的关联(当没有进程使用'msg消息队列'时, 最后一个进程负责释放'msg消息队列')
	mq_getatrr(): 获取'msg消息队列'的特性
	mq_setattr(): 设置'msg消息队列'的特性[mq_open() 创建'msg消息队列'时, 也可以设置'msg消息队列'特性]
	mq_notify():  将进程的信号系统与'msg消息队列'绑定, 绑定后, msg消息可用时(来消息时), 会通过发送一个信号;


	mqd_t mq_open(const char *name, int oflag);
	mqd_t mq_open(const char *name, int oflag, mque_flag_t mque_flag, struct mq_attr *attr);
		oflag:
			O_CREAT			队列不存在时创建队列
			O_EXCL			与O_CREAT搭配使用, 用于创建新队列
			O_RDONLY		只读打开
			O_WRONLY		只写打开
			O_RDWR			读写打开
			O_NONBLOCK	以非阻塞形式打开
		mque_flag:
			为了供其他进程读写队列, 一般是: 0666
	int mq_close(mqd_t mqdes);
	int mq_unlink(const char *name);

	int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
	int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);

	int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);
			msg_prio: 设置消息优先级, 消息的优先级从0到31[数字越大,优先级越高]
	ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);
					*msg_prio:获取消息优先级

	int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct timespec *abs_timeout);
	ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio, const struct timespec *abs_timeout);

	int mq_notify(mqd_t mqdes, const struct sigevent *notification);
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


//为了方便信号函数共用G_mque_fd, 所以设置为全局变量
mqd_t G_mque_fd = -1;


//注册64 号实时信号(作为posix消息队列的通知)
void sig_realtime64(int sig){
	int tmp, prio;
	char buf_recv[MQUEUE_MSG_MAX];				//接收缓冲区, 必须足够容纳整条消息, 小一点都会报错!!
	tmp=mq_receive(G_mque_fd,buf_recv,MQUEUE_MSG_MAX,&prio);
	if(tmp == -1){												//阻塞接收mque消息
		perror("mq_receive()");
		mq_close(G_mque_fd);
		mq_unlink(MQUEUE_NAME);
	}
	else
		printf("sig_realtime64(%d)->[priority%ld]mq_receive(): %s\n",sig,prio,buf_recv);
}



void mqueue_posix_test(void){
	int mque_flag, tmp, prio;
	struct mq_attr attr;
	struct sigevent sigev;
	const char *buf_send = "fuck you idiot!!";
	const char *buf_send2 = "bitches need to get fuck 2!!";
	char buf_recv[MQUEUE_MSG_MAX];
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


	tmp = mq_send(G_mque_fd, buf_send, strlen(buf_send)+1, 0);
	if(tmp == -1){												//阻塞发送mque消息, 优先级=0
		perror("mq_send()");								//(要用异步,只能在mqueue创建时修改属性,后续不能修改)
		goto mqueue_posix_test_failed;
	}
	printf("msg_len=%d, mq_send()=%d\n",strlen(buf_send)+1,tmp);//for test only

	timeout_send.tv_sec = 1;
	timeout_send.tv_nsec = 0;
	tmp = mq_timedsend(G_mque_fd, buf_send2, strlen(buf_send2)+1, 1, &timeout_send);
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
	else
		printf("[priority%ld]mq_timedreceive(): %s\n",prio,buf_recv);

mqueue_posix_test_failed:
	mq_close(G_mque_fd);
	mq_unlink(MQUEUE_NAME);
	return;
}


int main(void){
	mqueue_posix_test();
	return 0;
}




