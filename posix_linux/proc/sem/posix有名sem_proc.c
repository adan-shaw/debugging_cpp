//编译:
//		gcc ./posix有名sem_proc.c -g3 -lrt -lpthread -o x 


//posix'有名信号量'
/*
	//打开/创建'有名信号量'
	sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
		name:
			'有名信号量'信号量指向的文件名(路径名)
		oflag:
			一般用: O_CREAT|O_EXCL
				O_CREAT = 信号量不存在时, 则自动创建信号量; 信号量存在时, 则打开信号量;
				O_EXCL  = 无论信号量是否已存在, 重新初始化信号量的value;
		mode:
			用户的文件权限控制, 属主所有即可, 0600 / 0660
		value:
			'有名信号量'的使用者个数(有多少个线程抢占,就配置为n,保证每个线程都可以抢占一次)
	ps:
		一般情况下, 进程使用'有名信号量', 最好是创建时独占(清场), 运行时与子进程共享;
		如果是兄弟进程之间共享'有名信号量', 或者n个无关进程共享'有名信号量', 这样会比较棘手;
		这样就需要使用O_EXCL + 一个预设value(较大的value 值), 
		防止'有名信号量'出现value 不足导致的抢占缺陷问题;


	//关闭'有名信号量'(进程结束会自动关闭, 跟fd/FILE 类似)
	int sem_close(sem_t *sem);


	//通知内核释放'有名信号量'
	int sem_unlink(const char *name);
	由于有名信号量是多进程共享的, 一个进程终止了,并不能影响其它进程继续使用;
	所以'有名信号量'实际上会自动登记到内核,
	当没有任何进程使用这个'有名信号量'时, 需要彻底释放:
		你可以使用sem_unlink() 彻底释放;
		也是可以使用ipcs && ipcrm 来查询sem 的id/key 来进行删除



	//释放'有名信号量'使用权[sem_getvalue()+1]
	int sem_post(sem_t *sem);

	//等待抢占'有名信号量'使用权, 如果抢到使用权, 则[sem_getvalue()-1]
	int sem_wait(sem_t *sem);																					//阻塞等待版
	int sem_trywait(sem_t *sem);																			//非阻塞等待版
	int sem_timedwait(sem_t *sem,const struct timespec *abs_timeout);	//等待超时版

	//获取'有名信号量'当前有多少个用户排队等待
	int sem_getvalue(sem_t *sem, int *sval);
			成功返回0, 回调返回等待用户数:sval

	//编译选项:
		-lrt -lpthread
*/

//posix'无名信号量'&&posix'有名信号量'的区别:
/*
	仅创建和销毁方式不一样, 操作方式是一样的;

	无名信号量只能用在'带有CLONE_VM属性的进程'中, 或者线程中(线程默认自带CLONE_VM属性);
	否则无名信号量没有'共同可见的内存区域'[即CLONE_VM属性],
	则无法'协作多进程进行同步操作', 不能达到同步or互斥的作用(失效, 等于没用);
	因此, 如果是多进程操作, 一般多进程不具备CLONE_VM属性, 所以多进程一般不用无名信号量;
	所以一般情况下, 多进程用有名信号量, 多线程用无名信号量, 这种说明并没有太大问题;

	有名信号量, 需要inode文件key, 不一定需要读写权限, 
	但需要inode key 指向的文件存在, 用来生成唯一通信key;

	有名信号量, 受进程chroot() 约束!!
	如果你使用root+chroot(), 尝试修改chroot(), 修改成功之后, debian 马上会拒绝报错:
		sem_unlink(): Function not implemented (功能未实现)
		sem_open(): Function not implemented
	因此, 暂时不要逃脱chroot(), 这可能是出于对系统安全的考虑, 故意不让你修改chroot()的;
*/



#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>


//#define TEST_COUNT (9)
#define TEST_COUNT (99999)
#define SEM_NAME "/chroot_sem_name" //chroot()默认在'/'根目录,不能修改,修改需要root权限,还会报错;

int G_test_val = 0;
sem_t* sem;



void *pth_func_son(void* arg){
	int tmp, sval;
	pid_t pid = getpid();
	for(tmp = 0,sval = 0; tmp < TEST_COUNT; tmp++){
		if(sem_trywait(sem) == -1)			//尝试等待'有名信号量'的使用权(非阻塞)
			continue;
		else{
			G_test_val+=3;
			sem_post(sem);								//释放'有名信号量'使用权
		}
		sem_getvalue(sem,&sval);
		printf("son-%d: tmp=%d, sval=%d, G_test_val=%d\n", pid, tmp, sval, G_test_val);
	}
	return NULL;
}

void posix_sem_pth_test(void){
	int tmp, sval;
	unsigned long int pid_son, pid_father;
	void* son_ret;
	mode_t mask = umask(0);						//暂时屏蔽系统的文件权限(防止创建有名sem失败)

	pid_father = getpid();

	if(sem_unlink(SEM_NAME) == -1)		//如果内核存在'有名信号量', 先解除
		perror("sem_unlink()");

	sem = sem_open(SEM_NAME, O_CREAT|O_EXCL, 0660, 2);
	if(sem == SEM_FAILED){						//创建'有名信号量'(SEM_FAILED=NULL=0)
		perror("sem_open()");
		return ;
	}

	umask(mask);											//恢复umask的值(恢复系统的文件权限)

	sem_getvalue(sem,&sval);					//获取sem 里面的value
	if(sval < 2){											//当value不足2时,强制加2个(宁可多,不可少,n个用户就加n)
		sem_post(sem);
		sem_post(sem);
	}

	tmp = pthread_create(&pid_son,NULL,pth_func_son,NULL);
	if(tmp != 0){
		perror("pthread_create()");
		sem_close(sem);
		sem_unlink(SEM_NAME);
		return;
	}

	for(tmp = 0,sval = 0; tmp < TEST_COUNT; tmp++){
		sem_wait(sem);									//阻塞等待'有名信号量'的使用权
		G_test_val-=3;
		sem_post(sem);									//释放'有名信号量'使用权
		sem_getvalue(sem,&sval);
		printf("father-%d: tmp=%d, sval=%d, G_test_val=%d\n", pid_father, tmp, sval, G_test_val);
	}

	if(pthread_join(pid_son, &son_ret) != 0)
		perror("pthread_join()");

	sem_close(sem);										//摧毁'有名信号量'
	sem_unlink(SEM_NAME);							//由父进程负责unlink(), 子进程只需sem_close()

	//
	/*
		如果不加sem 信号量同步, 经过99999次测试后, G_test_val 的值不一定是0;
		因为可能是父进程多跑了, 可能是子进程多跑了, 导致数值+- 次数不对称, 先跑多跑的, +/-的次数多;
		加了sem 信号量, 则大部分时间都消耗在等待sem 信号量上, 但+-次数可以得到公平保证, 两边都完全对称;

		如果两个进程都是累加, 而且累加的值一样, 都是+1 or +n, 则不需要同步;
		(如果不看计算过程, 不看计算顺序的话, 只看计算结果的话; 但sem 也没办法保证计算顺序, 只能保证访问次数对等)

		如果两个进程有加有减, 则必须使用同步操作;
	*/
	printf("main pthread-%d end, G_test_val shoule be zero[0]: %d\n", pid_father, G_test_val);
	return ;
}



int main(void){
	posix_sem_pth_test();
	return 0;
}






