//编译:
//		gcc -g3 ./sysv_sem.c -o x


//System V信号量简介
/*
	System V信号量看上去API 很简单, 实际都是需要填充结构体, 十分麻烦, 不如posix 信号量好用;
	System V信号量是c 语言风格, 需要填充结构体的, 不好用;
*/

//System V信号量详解
/*
	结构体:
		struct sembuf{
			short sem_num;//除非使用一组sysv信号量,否则该值为0
			short sem_op;	//信号量在一次操作中需要改变的数据,通常是两个数,一个是-1,即P(等待)操作,一个是+1,即V(发送信号)操作
			short sem_flg;//通常为SEM_UNDO,使操作系统跟踪信号,并在进程没有释放该信号量而终止时,操作系统释放信号量
		};
		union semun {
			int val;
			struct semid_ds *buf;
			unsigned short *arry;
		};

	函数原型:
		//创建'sysv 信号量'
		int semget(key_t key, int num_sems, int sem_flags);
				key:       ftok()-key;
				num_sems:  (除非使用一组sysv信号量,否则该值为1)
				sem_flags: IPC_CREAT | IPC_EXCL;
				成功返回sem_id, 失败返回-1;

		//修改'sysv 信号量'的值
		int semop(int sem_id, struct sembuf *sem_opa, size_t num_sem_ops);
				sem_id:      semget() 返回值;
				sem_opa:     struct sembuf{} 结构体;
				num_sem_ops: struct sembuf{} 结构体的个数(除非使用一组sysv信号量,否则该值为1)

		//初始化or删除'sysv 信号量'(只有初始化和删除两个操作)
		int semctl(int sem_id, int sem_num, int command, ...);
				sem_id:   semget() 返回值;
				sem_num:  (除非使用一组sysv信号量,否则该值为1)
				command:
					IPC_STAT	读取一个信号量集的数据结构semid_ds,并将其存储在semun中的buf参数中
					IPC_SET		设置信号量集的数据结构semid_ds中的元素ipc_perm,其值取自semun中的buf参数
					IPC_RMID	将信号量集从内存中删除
					GETALL		用于读取信号量集中的所有信号量的值
					GETNCNT		返回正在等待资源的进程数目
					GETPID		返回最后一个执行semop操作的进程的PID
					GETVAL		返回信号量集中的一个单个的信号量的值
					GETZCNT		返回这在等待完全空闲的资源的进程数目
					SETALL		设置信号量集中的所有的信号量的值
					SETVAL		设置信号量集中的一个单独的信号量的值
				...
				例子:
					semctl(semid,1,IPC_RMID,NULL);	//删除'sysv 信号量'
					union semun m_semun;
					m_semun.val = 2;
					semctl(semid,1,SETVAL,&m_semun);//初始化'sysv 信号量'的value值为2
*/

//System V信号量 && posix 信号量的API 对比:
/*
	Systm V					POSIX
	semctl()				sem_getvalue()	*
	semget()				sem_post()			*
	semop()					sem_timedwait()	*
									sem_trywait()		*
									sem_wait()			*

									sem_destroy()
									sem_init()

									sem_close()
									sem_open()
									sem_unlink()

	总结:
		<进程>
			System V的信号量一般用于进程同步, 且是内核持续的, api为:
			semget()
			semctl()
			semop()

			Posix的有名信号量一般用于进程同步,有名信号量是内核持续的.有名信号量的api为:
			sem_open()
			sem_close()
			sem_unlink()

		<线程>
			Posix的无名信号量一般用于线程同步,无名信号量是进程持续的.无名信号量的api为:
			sem_init()
			sem_destroy()
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun{
	int              val;    // Value for SETVAL
	struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET
	unsigned short  *array;  // Array for GETALL, SETALL
	struct seminfo  *__buf;  // Buffer for IPC_INFO(Linux-specific)
};

#define FTOK_PATH "./x"
#define PAGE_SIZE (4096)
#define TEST_COUNT (9)
//#define TEST_COUNT (40960)

int main(void){
	int tmp, shmid, semid;
	float *addr = NULL;															//共享内存变量指针
	float ftmp;
	key_t key_shm, key_sem;
	struct sembuf sem_opt_p = {0, -1, SEM_UNDO};		//定义P(等待-1)操作的semop()结构体
	struct sembuf sem_opt_v = {0, 1, SEM_UNDO};			//定义V(发送信号+1)操作的semop()结构体
	union semun m_semun;


	//创建ftok()->key
	key_shm = ftok(FTOK_PATH,1000);
	if(key_shm == -1){
		perror("ftok()");
		return -1;
	}
	key_sem = ftok(FTOK_PATH,1001);
	if(key_sem == -1){
		perror("ftok()");
		return -1;
	}

	//创建一个共享内存
	shmid = shmget(key_shm, PAGE_SIZE, IPC_CREAT | 0600);
	if(shmid == -1){
		perror("shmget()");
		return -1;
	}
	printf("shmid=%ld\n",shmid);

	//获取shm共享内存的起始地址, 且为可读可写
	addr = (float*)shmat(shmid,0,0);
	if(*addr == -1){
		perror("shmat()");
		return -1;
	}

	//创建sysv信号量
	semid = semget(key_sem, 1, IPC_CREAT|IPC_EXCL|0600);
	if(semid == -1){
		perror("semget()");
		return -1;
	}
	printf("semid=%ld\n", semid);

	//设置sysv信号量的value值
	m_semun.val = 2;
	if(semctl(semid,0,SETVAL,m_semun) == -1){
		perror("semctl()");
		return -1;
	}

	//获取sysv信号量的value值
	m_semun.val = 0;
	if(semctl(semid,0,GETVAL,m_semun) == -1){
		perror("semctl()");
		return -1;
	}
	printf("sizeof(m_semun)=%d, m_semun.val=%ld\n", sizeof(m_semun), m_semun.val);

	addr[0] = 0;
	if(fork() == 0){									//子进程
		//printf("start fork(), sem_opt_p->sem_op=%d, sem_opt_v->sem_op=%d\n",sem_opt_p.sem_op,sem_opt_v.sem_op);
		for(tmp=0; tmp<TEST_COUNT; tmp++){
			semop(semid, &sem_opt_p, 1);		//sysv信号量P(等待-1)操作
			addr[0] = addr[0] + 1.1;
			ftmp = addr[0];
			semop(semid, &sem_opt_v, 1);		//sysv信号量V(发送信号+1)操作
			printf("son: %f\n",ftmp);
		}
		exit(0);												//子进程结束
	}
	for(tmp=0; tmp<TEST_COUNT; tmp++){//父进程
		semop(semid, &sem_opt_p, 1);
		addr[0] = addr[0] - 1.1;
		ftmp = addr[0];
		semop(semid, &sem_opt_v, 1);
		printf("father: %f\n",ftmp);
	}

	//删除sysv信号量
	if(semctl(semid,0,IPC_RMID,NULL) == -1){
		perror("semctl()");
		return -1;
	}

	//释放shm共享内存
	if(shmdt(addr) == -1){
		perror("shmdt()");
		return -1;
	}
	addr = NULL;

	//删除shm共享内存
	if(shmctl(shmid,IPC_RMID,0)==-1){
		perror("shctl()");
		return -1;
	}

	return 0;
}
