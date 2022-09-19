//编译:
//		gcc ./posix有名sem_proc\[失败\].c -g3 -lrt -lpthread -o x 

//失败原因:
/*
	并不是你操作出错, 而是你创建多进程的方式出错了:
		fork() 必然会触发写时赋值, 这时候会赋值两份全局变量G_test_val, 
		这样子父进程之间, 根本不可能实现: 
			对同一资源进行访问
		而是各自访问各自的G_test_val全局变量;

	正确做法是:
		1.继续用pthread 线程实现'有名信号量'同步(没有不可以,更简单)
		2.创建fork() 可以共享的: shm共享内存, mmap映射, 来实现'有名信号量'同步(麻烦, 复杂)
*/



#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



#define TEST_COUNT (9)
//#define TEST_COUNT (99999)
#define SEM_NAME "/chroot_sem_name"	//chroot()默认在'/'根目录,不能修改,修改需要root权限,还会报错;

int G_test_val = 0;
sem_t* sem;



void proc_func_son(void){
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
	return;
}

//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg);

void posix_sem_proc_test(void){
	int tmp, sval, son_ret;
	unsigned long int pid_son, pid_father;
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

	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		sem_close(sem);
		sem_unlink(SEM_NAME);
		return;
	}
	if(tmp == 0){											//子进程
		proc_func_son();
		sem_close(sem);									//fork()必然会写时赋值一份有名sem的fd,不过子进程退出会自动释放,这句可以删掉
		exit(0);
	}
	pid_son = tmp;										//父进程保存子进程pid

	for(tmp = 0,sval = 0; tmp < TEST_COUNT; tmp++){
		sem_wait(sem);									//阻塞等待'有名信号量'的使用权
		G_test_val-=3;
		sem_post(sem);									//释放'有名信号量'使用权
		sem_getvalue(sem,&sval);
		printf("father-%d: tmp=%d, sval=%d, G_test_val=%d\n", pid_father, tmp, sval, G_test_val);
	}

	tmp = waitpid(pid_son,&son_ret,0);//阻塞
	if(tmp == -1)
		perror("waitpid()");
	else{
		printf("son %d exit(%d), waitpid()=%d\n", pid_son, son_ret, tmp);
		print_wait_stat(son_ret,NULL);
	}

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
	posix_sem_proc_test();
	return 0;
}



//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg){
	if(attach_msg != NULL)						//打印附带msg
		printf("%s\n", attach_msg);

	if(WIFEXITED(status)){						//子进程正常退出, 退出statue码=WEXITSTATUS(status)
		printf("child exited, status=%d\n", WEXITSTATUS(status));
		return;
	}

	if(WIFSIGNALED(status)){					//子进程被信号终止, 终止信号=WTERMSIG(status)
		printf("child killed by signal %d (%s)",WTERMSIG(status), strsignal(WTERMSIG(status)));
		#ifdef WCOREDUMP
		if(WCOREDUMP(status))						//产生了核心转存
			printf(" (core dumped)\n");
		#endif
		return;
	}

	if(WIFSTOPPED(status)){						//子进程被信号暂停, 暂停信号=WSTOPSIG(status)
		printf("child stopped by signal %d (%s)\n",WSTOPSIG(status), strsignal(WSTOPSIG(status)));
		return;
	}

	#ifdef WIFCONTINUED								//子进程已从暂停状态转为继续执行continue
	if(WIFCONTINUED(status)){
		printf("child continued\n");
		return;
	}
	#endif

	//未知错误(不应该发生)
	printf("print_wait_stat() failed, child statue=%d\n",status);
	return;
}
