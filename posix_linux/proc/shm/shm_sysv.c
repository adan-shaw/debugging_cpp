//编译:
//		gcc -g3 ./shm_sysv.c -o x 



#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define FTOK_PATH "./x"
#define PAGE_SIZE (4096)

void shm_sysv_test(void){
	key_t key_shm;
	int shmid;
	float *addr_f;

	//创建ftok()->key
	key_shm = ftok(FTOK_PATH,1000);
	if(key_shm == -1){
		perror("ftok()");
		return;
	}

	//创建一个共享内存
	shmid = shmget(key_shm, PAGE_SIZE, IPC_CREAT | 0600);
	if(shmid == -1){
		perror("shmget()");
		return;
	}
	printf("shmid=%ld\n",shmid);

	//获取shm共享内存的起始地址, 且为可读可写
	addr_f = (float*)shmat(shmid,0,0);
	if(*addr_f == -1){
		perror("shmat()");
		return;
	}

	addr_f[0] = 1.1;
	addr_f[1] = 2.2;
	addr_f[2] = 3.3;
	printf("shm to float point, [0]=%f, [1]=%f, [2]=%f\n",addr_f[0],addr_f[1],addr_f[2]);

	//释放shm共享内存
	if(shmdt(addr_f) == -1){
		perror("shmdt()");
		return;
	}
	addr_f = NULL;

	//删除shm共享内存
	if(shmctl(shmid,IPC_RMID,0)==-1){
		perror("shctl()");
		return;
	}
	return ;
}

int main(void){
	shm_sysv_test();
	return 0;
}



//System V共享内存 结构体:
/*
	struct shmid_ds{
		struct ipc_perm  shm_perm;		//操作权限
		size_t           shm_segsz;		//段大小
		pid_t            shm_lpid;		//最后一个操作的进程pid
		pid_t            shm_cpid;		//属主pid
		short            shm_nattch;	//使用者进程的个数
		time_t           shm_atime;		//最后一个进程附加到该段的时间
		time_t           shm_dtime;		//最后一个进程离开该段的时间
		time_t           shm_ctime;		//最后一个进程修改该段的时间
		//下面是私有的
		unsigned short shm_npages;		//段的大小(以页为单位)
		unsigned long *shm_pages;			//指向frames->SHMMAX的指针数组
		struct vm_area_struct *attaches;//对共享段的描述
	};
*/



//System V共享内存API:
/*
	int shmget(key_t key, size_t size, int oflag);
	void *shmat(int shmid, const void *shmaddr, int flag);
	int shmdt(const void *shmaddr);
	int shmctl(int shmid, int cmd, struct shmid_ds *buf);

	//创建一个新的共享内存或打开一个已存在的共享内存
	int shmget(key_t key, size_t size, int oflag);
			key:   ftok()-key;
			size:  shm初始化的大小(一般按照PAGE页大小来初始化)
			oflag: IPC_CREAT | IPC_EXCL | SHM_R | SHM_W; [0644 仍然适用]
			成功返回shm_id, 失败返回-1;

	//根据shmid, 提取shm的起始地址
	void *shmat(int shmid, const void *shmaddr, int flag);
				shmid:   shmget() 返回值;
				shmaddr: 一般为NULL, 即shm指针偏移量=0;
				flag:    0=读写访问, SHM_RDONLY=只读访问
				成功返回shm指针, 失败返回-1;

	//释放由shmat提取的shm指针[成功返回0,失败返回-1]
	int shmdt(const void *shmaddr);

	//对shm共享内存进行各种控制操作[成功返回0,失败返回-1]
	int shmctl(int shmid, int cmd, struct shmid_ds *buf);
			shmid: shmget() 返回值;
			cmd:   IPC_RMID: 从系统中删除shm共享内存(此时buf应为NULL)
						 IPC_STAT: 通过struct shmid_ds结构体返回shm共享内存的参数
						 IPC_SET:  通过struct shmid_ds结构体设置shm共享内存的参数
			buf:	 struct shmid_ds结构体
*/



//shm共享内存系统限制:
/*
	SHMMAX的默认值是: 32MB

	# shell 查看SHMMAX 的命令
	cat /proc/sys/kernel/shmmax
*/
