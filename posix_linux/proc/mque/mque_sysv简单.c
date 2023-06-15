//编译:
//		gcc ./mque_sysv简单.c -lrt -g3 -o x 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>

//函数原型:
/*
	//创建systemv 消息队列
	int msgget(key_t key, int msgflg);
			msgflg:  IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP  (一般用法)

	//修改systemv 消息队列的属性
	int msgctl(int msqid, int cmd, struct msqid_ds *buf);
			IPC_RMID:删除systemv消息队列,
							 队列中所有消息会丢失,所有阻塞读/写的进程立即醒来,msgsnd/msgrcv会失败返回EIDRM
			IPC_STAT:获取systemv消息队列的msgid_ds数据结构
			IPC_SET: 设置systemv消息队列的msgid_ds数据结构

	//发送systemv 消息队列(发送成功只会返回0,不会返回发送了多少数据)
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
			msgflg: IPC_NOWAIT:非阻塞操作

	//接收systemv 消息队列(接收成功,会返回接收了多少数据)
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
					msqid:
					msgp:
					msgsz:
					msgtyp:
						msgtyp=0:删除队列中第一条消息, 并且返回给调用进程
						msgtyp>0:将队列中第一条mtype>=msgtyp的消息删除, 并将其返回给调用进程(优先读取)
						msgtyp<0:将队列中第一条mtype<=msgtyp的消息删除, 并将其返回给调用进程(优先读取)
					msgflg:
						IPC_NOWAIT:非阻塞操作
						MSG_EXCEPT:当msgtyp>0时候,将队列中mtype!=msgtyp的消息强制删除并且返回给调用者(Linux特有,筛选删除)
						MSG_NOERROR:阶段操作, 防止消息大于mque创建时定义的消息大小
*/



#define mque_sysv_path "./x"



void mque_sysv_easyw(void){
	key_t kfd;
	int mque_fd, tmp;
	const char* pbuf = "love  you man";

	kfd = ftok(mque_sysv_path, 0x57);
	if(kfd == -1){
		perror("ftok()");
		return;
	}

	mque_fd = msgget(kfd, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP);
	if(mque_fd == -1){
		perror("msgget()");
		return;
	}

	tmp = msgsnd(mque_fd, pbuf, strlen(pbuf)+1, IPC_NOWAIT);
	if(tmp == -1){
		perror("msgsnd()");
		return;
	}
	//msgsnd()发送成功只会返回0,不会返回发送了多少数据
	printf("msgsnd() %d size data\n", strlen(pbuf)+1);

	close(mque_fd);
	return;
}

void mque_sysv_easyr(void){
	key_t kfd;
	int mque_fd, tmp;
	char buf[64];

	kfd = ftok(mque_sysv_path, 0x57);
	if(kfd == -1){
		perror("ftok()");
		return;
	}

	mque_fd = msgget(kfd, S_IRUSR|S_IWUSR);
	if(mque_fd == -1){
		perror("msgget()");
		return;
	}

	tmp = msgrcv(mque_fd, buf, sizeof(buf), 0, IPC_NOWAIT|MSG_NOERROR);
	if(tmp == -1){
		perror("msgrcv()");
		return;
	}
	printf("msgrcv() %d size data: %s\n", tmp, buf);

	close(mque_fd);
	return;
}

int main(void){
	key_t kfd;
	int mque_fd;
	struct msqid_ds buf_info;

	mque_sysv_easyw();//写
	mque_sysv_easyr();//读

	//删除
	kfd = ftok(mque_sysv_path, 0x57);
	if(kfd == -1){
		perror("ftok()");
		return -1;
	}

	mque_fd = msgget(kfd, S_IRUSR|S_IWUSR);
	if(mque_fd == -1){
		perror("msgget()");
		return -1;
	}

	if(msgctl(mque_fd, IPC_RMID, NULL) == -1){
		perror("msgctl()");
		return -1;
	}

	close(mque_fd);
	return 0;
}
