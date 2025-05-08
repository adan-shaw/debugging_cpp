//编译:
//		gcc ./posix无名sem_pth.c -lpthread -lrt -g3 -o x 


//posix'无名信号量'
/*
	//创建or初始化'无名信号量'
	int sem_init(sem_t *sem,int pshared,unsigned int value);
			pshared: pshared=0(线程间共享); pshared!=0(多进程共享);
			value:   '无名信号量'的使用者个数(有多少个线程抢占,就配置为n,保证每个线程都可以抢占一次)

	//摧毁'无名信号量'
	int sem_destroy(sem_t *sem);

	//释放'无名信号量'使用权[sem_getvalue()+1]
	int sem_post(sem_t *sem);

	//等待抢占'无名信号量'使用权, 如果抢到使用权, 则[sem_getvalue()-1]
	int sem_wait(sem_t *sem);																					//阻塞等待版
	int sem_trywait(sem_t *sem);																			//非阻塞等待版
	int sem_timedwait(sem_t *sem,const struct timespec *abs_timeout);	//等待超时版

	//获取'无名信号量'当前有多少个用户排队等待
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


//#define TEST_COUNT (9)
#define TEST_COUNT (99999)

int G_test_val = 0;
sem_t sem;



void *pth_func_son(void* arg){
	int tmp, sval;
	pid_t pid = getpid();
	for(tmp = 0,sval = 0; tmp < TEST_COUNT; tmp++){
		if(sem_trywait(&sem) == -1)			//尝试等待'无名信号量'的使用权(非阻塞)
			continue;
		else{
			G_test_val+=3;
			sem_post(&sem);								//释放'无名信号量'使用权
		}
		sem_getvalue(&sem,&sval);
		printf("son-%d: tmp=%d, sval=%d, G_test_val=%d\n", pid, tmp, sval, G_test_val);
	}
	return NULL;
}

void posix_sem_pth_test(void){
	int tmp, sval;
	unsigned long int pid_son, pid_father;
	void* son_ret;

	pid_father = getpid();

	if(sem_init(&sem, 0, 2) == -1){		//创建or初始化'无名信号量'
		perror("sem_init()");
		return ;
	}

	sem_getvalue(&sem,&sval);					//获取sem 里面的value
	if(sval < 2){											//当value不足2时,强制加2个(宁可多,不可少,n个用户就加n)
		sem_post(&sem);
		sem_post(&sem);
	}

	tmp = pthread_create(&pid_son,NULL,pth_func_son,NULL);
	if(tmp != 0){
		perror("pthread_create()");
		sem_destroy(&sem);
		return;
	}

	for(tmp = 0,sval = 0; tmp < TEST_COUNT; tmp++){
		sem_wait(&sem);									//阻塞等待'无名信号量'的使用权
		G_test_val-=3;
		sem_post(&sem);									//释放'无名信号量'使用权
		sem_getvalue(&sem,&sval);
		printf("father-%d: tmp=%d, sval=%d, G_test_val=%d\n", pid_father, tmp, sval, G_test_val);
	}

	if(pthread_join(pid_son, &son_ret) != 0)
		perror("pthread_join()");

	sem_destroy(&sem);								//摧毁'无名信号量'

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





