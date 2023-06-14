//编译:
//		gcc ./limit查看\&修改.c -g3 -o x 



#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/resource.h>



//注意:
//	limit修改系统sysconf参数, 是'本用户,本次登录有效', 重启即被重置.
//	而且修改sysconf 需要root 权限运行程序, 否则操作不被允许: setrlimit(): Operation not permitted



//1.打印某个resource资源limit'变量宏'的值
void printRlimit(const char *msg, int resource);

//2.修改某个resource资源limit'变量宏'的值(还未实现)



int main(void){
	struct rlimit rl;
	int j;
	pid_t childPid;
	int proc_max_s = 512;							//设置最大的进程数量-软件限制=512
	int proc_max_h = 20480;						//设置最大的进程数量-硬件限制=20480



	//1.打印某个resource limit'变量宏'的值
	printRlimit("Initial RLIMIT_NPROC limits: ", RLIMIT_NPROC);



	//2.修改软件限制和硬件限制
	//	(不想修改的设置为RLIM_INFINITY - 不限制需要root 权限才能做)
	rl.rlim_cur = proc_max_s;
	rl.rlim_max = proc_max_h;
	if(setrlimit(RLIMIT_NPROC, &rl) == -1){
		perror("setrlimit()");
		return -1;
	}
	//打印修改结果
	printRlimit("New RLIMIT_NPROC limits: ", RLIMIT_NPROC);



	//3.单一修改'软件限制'
	rl.rlim_cur = proc_max_s;
	rl.rlim_max = RLIM_INFINITY;			//RLIM_INFINITY表示不对资源限制(root权限)
	if(setrlimit(RLIMIT_NPROC, &rl) == -1){
		perror("setrlimit()");
		return -1;
	}
	//打印修改结果
	printRlimit("New RLIMIT_NPROC limits: ", RLIMIT_NPROC);



	//4.单一修改'硬件限制'
	rl.rlim_cur = proc_max_s;
	rl.rlim_max = proc_max_h;
	if(setrlimit(RLIMIT_NPROC, &rl) == -1){
		perror("setrlimit()");
		return -1;
	}
	//打印修改结果
	printRlimit("New RLIMIT_NPROC limits: ", RLIMIT_NPROC);



	//5.创建无限多的子进程, 用作测试'RLIMIT_NPROC'限制值
	for(j = 1; ; j++){
		switch(childPid = fork()){
		case -1:
			perror("fork()");
			printf("at last, j = %d\n",j);//一般情况下, 只有父进程会出错(最后一次调用fork()越界limit 限制)

			exit(EXIT_FAILURE);						//fork循环创建失败,退出程序
			// NOT REACHED //
		case 0:
			//sleep(10);									//卡死子进程(不需要卡死子进程, 因为子进程的尸体, 仍然会占用'一个限制数',
			_exit(EXIT_SUCCESS);					//				 因此测试系统限制数极限, 不需要卡死子进程)
		default:
			printf("Child %d (PID=%ld) started\n", j, (long) childPid);
			continue;											//父亲进程, 死循环创建子进程
		}
	}

	return 0;
}





//打印某个resource limit'变量宏'的值
void printRlimit(const char *msg, int resource){
	struct rlimit rlim;



	//1.获取'目标宏resource'的系统限制, 返回到struct rlimit 结构体中
	if(getrlimit(resource, &rlim) == -1){
		perror("getrlimit()");
		return;
	}



	//2.打印软件限制:rlim.rlim_cur
	printf("%s soft=", msg);
	if(rlim.rlim_cur == RLIM_INFINITY)
		printf("infinite");
#ifdef RLIM_SAVED_CUR// Not defined on some implementations
	else if(rlim.rlim_cur == RLIM_SAVED_CUR)
		printf("unrepresentable");
#endif
	else
		printf("%lld", (long long) rlim.rlim_cur);



	//3.打印硬件限制:rlim.rlim_max
	printf("; hard=");
	if(rlim.rlim_max == RLIM_INFINITY)
		printf("infinite\n");
#ifdef RLIM_SAVED_MAX// Not defined on some implementations
	else if(rlim.rlim_max == RLIM_SAVED_MAX)
		printf("unrepresentable");
#endif
	else
		printf("%lld\n", (long long) rlim.rlim_max);

	return ;
}





//1.rlimit 简介:
/*
	函数原型:
		int getrlimit(int resource, struct rlimit *rlim);
		int setrlimit(int resource, const struct rlimit *rlim);

	所需结构体:
	struct rlimit{
		rlim_t rlim_cur;			// The soft limit 软件限制
		rlim_t rlim_max;			// The hard limit 硬件限制
	};

	参数说明:
		resource            = 限制类型, 执行限制哪个'变量宏';
		struct rlimit *rlim = 输入/返回的'限制描述体';

	resource可以指定的'变量宏':
		RLIMIT_SIGPENDING			用户可拥有的最大挂起信号数.
		RLIMIT_STACK					最大的进程堆栈, 以字节为单位.
		RLIMIT_MEMLOCK				进程可锁定在内存中的最大数据量, 字节为单位.
		RLIMIT_MSGQUEUE				进程可为POSIX消息队列分配的最大字节数.

		RLIMIT_AS							进程的最大虚内存空间, 字节为单位.
		RLIMIT_CORE						内核转存文件的最大长度.
		RLIMIT_DATA						进程数据段的最大值.
		RLIMIT_LOCKS					进程可建立的锁和租赁的最大值.
		RLIMIT_NICE						进程可通过setpriority()或nice()调用设置的最大完美值.
		RLIMIT_NPROC					用户可拥有的最大进程数.

		RLIMIT_RTPRIO					进程可通过sched_setscheduler()和sched_setparam()设置的最大实时优先级.
		RLIMIT_NOFILE					指定比进程可打开的最大文件描述词大一的值, 超出此值, 将会产生EMFILE错误.
		RLIMIT_FSIZE					进程可建立的文件的最大长度.
													如果进程试图超出这一限制时, 核心会给其发送SIGXFSZ信号, 默认情况下将终止进程的执行.
		RLIMIT_CPU						最大允许的CPU使用时间, 秒为单位.
													当进程达到软限制, 内核将给其发送SIGXCPU信号, 这一信号的默认行为是'终止进程'的执行.
													然而, 可以捕捉信号, 处理句柄可将控制返回给主程序.
													如果进程继续耗费CPU时间, 核心会以'每秒一次的频率'给其发送SIGXCPU信号,
													直到达到硬限制, 那时将给进程发送SIGKILL信号终止其执行.

	返回值说明:
		成功返回0.
		失败返回-1, errno被设为以下的某个值:
			EFAULT = rlim指针指向的空间不可访问
			EINVAL = 参数无效
			EPERM  = 增加资源限制值时, 权能不允许



	延伸阅读:
		ulimit 命令和setrlimit()轻松修改task进程资源上限值.
		在linux系统中, Resouce limit指在一个进程的执行过程中,
		它所能得到的资源的限制, 比如进程的core file的最大值, 虚拟内存的最大值等.

		Resouce limit的大小可以直接影响进程的执行状况.
		其有两个最重要的概念: soft limit和 hard limit.

		soft limit是指内核所能支持的资源上限.
			比如对于RLIMIT_NOFILE
			(一个进程能打开的最大文件数, 内核默认是1024),
			soft limit最大也只能达到1024.
			对于RLIMIT_CORE(core文件的大小, 内核不做限制),
			soft limit最大能是unlimited.

		hard limit在资源中只是作为soft limit的上限.
			当你设置hard limit后, 你以后设置的soft limit只能小于hard limit.
			要说明的是, hard limit只针对非特权进程,
			也就是进程的有效用户ID(effective user ID)不是0的进程.
			具有特权级别的进程(具有属性CAP_SYS_RESOURCE),
			soft limit则只有内核上限.



	setrlimit() 与getrlimit() 的使用细节:
		需要注意的是你在setrlimit(),
		需要检查是否成功来判断新值有没有超过hard limit.
		如Linux系统中在应用程序运行过程中经常会遇到程序突然崩溃,
		提示: Segmentation fault,
		这是因为应用程序收到了SIGSEGV信号.
		这个信号提示当进程发生了无效的存储访问,
		当接收到这个信号时, 缺省动作是:终止w/core.
		终止w/core的含义是:
			在进程当前目录生成core文件, 并将进程的内存映象复制到core文件中,
			core文件的默认名称就是"core"(这是 Unix类系统的一个由来已久的功能).

		事实上, 并不是只有SIGSEGV信号产生coredump,
		还有下面一些信号也产生coredump:
			SIGABRT(异常终止)、SIGBUS(硬件 故障)、SIGEMT(硬件故障)、
			SIGFPE(算术异常)、SIGILL(非法硬件指令)、SIGIOT(硬件故 障),
			SIGQUIT, SIGSYS(无效系统调用), SIGTRAP(硬件故障)等.

	对于resouce limit的读取修改,有两种方法.
		使用shell内建命令ulimit
		使用getrlimit和setrlimit APIsetrlimit


	if(getrlimit(RLIMIT_CORE, &rlim)==0){
	　　rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
	　　if(setrlimit(RLIMIT_CORE, &rlim_new)!=0){
	　　　　rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
	　　　　(void) setrlimit(RLIMIT_CORE, &rlim_new);
	　　}
	}
*/
