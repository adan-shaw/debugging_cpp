//
//现代操作系统的休眠实现方式:
//
/*
	现代操作系统实现休眠, 并不是用信号实现休眠的, 因为SUSv3 规定: 
		程序休眠的本意就是, 拒绝接受OS的其它信号, 只接收SIGALRM 信号, 或者必杀信号9;
		程序休眠的时候, cpu 使用权出让, 程序被挂起.
		(sleep, usleep 的glibc 源码, 实际上都是使用nanosleep 实现的)

	调用nanosleep(), 进程会进入TASK_INTERRUPTIBLE状态(脱离就绪队列),
	这种状态会等待'SIGALRM激活/唤醒'信号, 而返回TASK_RUNNING状态的.



	sleep和usleep不能使用的原因, 主要是:
		被唤醒后, sleep(), usleep() 都不知道自己是否休眠了足够的时间, 
		而nanosleep() 可以通过struct timespec *rem 来获取''剩余休眠时间'', 进而知道休眠进程是: 
			被SIGALRM 信号唤醒的(中断休眠), 还是休眠足够了, 才被唤醒的.

	因此, 如果非要休眠不可, 应考虑nanosleep()/select()


	使用nanosleep()时, 应注意判断struct timespec *rem返回值和错误代码, 
	否则容易造成cpu占用率100%.



总结:
	sleep, usleep, nanosleep都会被信号唤醒,
	只不过nanosleep() 加入被唤醒, 可以知道: 有没有完成你指定的休眠时间;
	如果是被信号唤醒的, '剩余休眠时间'会被记录在struct timespec *rem 中, 
	这样你通过返回值struct timespec *rem, 就知道休眠任务是否被打断了.
*/

#include <unistd.h>





//低分辨率休眠(挂起)<单位:秒>
unsigned int sleep(unsigned int second);


//高分辨率休眠(挂起)<单位:微秒> 1,000 微秒 = 1毫秒;10^6 微妙 = 1秒
int usleep(useconds_t usec);





#include <time.h>
struct timespec {
	time_t tv_sec;	// seconds			秒
	long	 tv_nsec; // nanoseconds	10^9纳秒 = 1秒
};

int nanosleep(const struct timespec *req, struct timespec *rem);



















