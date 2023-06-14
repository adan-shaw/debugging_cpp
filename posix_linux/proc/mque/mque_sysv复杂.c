//编译:
//		gcc ./mque_sysv复杂.c -lrt -g3 -o x 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>

#define mque_sysv_path "./x"

//结构体app_body{}
typedef struct app_body{
	int i1;
	double d2;
	char c3[64];//多余的字节位,继续写入; 虽然不能做到最小化拷贝,但胜在简单,数据对齐,拷贝速度快;
}body_t;



void mque_sysvw(void){
	key_t kfd;
	int mque_fd, tmp;
	body_t buf = {1,2.2,"fuck you asshole!!"};

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

	tmp = msgsnd(mque_fd, &buf, sizeof(buf), IPC_NOWAIT);
	if(tmp == -1){
		perror("msgsnd()");
		return;
	}
	//msgsnd()发送成功只会返回0,不会返回发送了多少数据
	printf("msgsnd() %d size data\n", sizeof(buf));

	close(mque_fd);
	return;
}

void mque_sysvr(void){
	key_t kfd;
	int mque_fd, tmp;
	body_t buf;

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

	tmp = msgrcv(mque_fd, &buf, sizeof(buf), 0, IPC_NOWAIT|MSG_NOERROR);
	if(tmp == -1){
		perror("msgrcv()");
		return;
	}
	printf("struct body_t{} data[%d]: {%ld, %lf, %s}\n",tmp,buf.i1,buf.d2,buf.c3);

	close(mque_fd);
	return;
}

int main(void){
	key_t kfd;
	int mque_fd;
	struct msqid_ds buf_info;

	mque_sysvw();//写
	mque_sysvr();//读

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
