//编译:
//		gcc -g3 ./scheduler进程调度策略修改.c -o x 

//实时调度策略:
/*
	策略					描述
	SCHED_FIFO	 实时-抢占式策略(先进先出)
	SCHED_RR		 实时-时间片轮转

	(下面都是不常用, 不是实时调度策略, 但也是posix标准的调度策略)
	SCHED_BATCH/SCHED_OTHER 频繁调用则不会衰减, 热度策略(两个别名都是同一种功能)
	SCHED_IDLE							忽略'nice值'(降到最低),适合于运行超级低优先级的进程

	一般系统并不需要修改'实时调度策略', 更不需要用到非'实时调度策略'
	但也有例外:
		当外部输入的互交进程数据显得很重要的情况下, 外部一个情况就可以影响整个主进程的工作状态的系统,
		如交通系统, 抢占性能就显得很重要.
		操作台抢占系统控制权, 以应对突发事件, 优先分配等需求, 这时候就需要做实时优先级抢占了.

	Linux allows the static priority range 1 to 99 for the SCHED_FIFO and SCHED_RR policies

	实时调度策略api:
		* 根据调度策略type, 获取调度权重
		int sched_get_priority_min(int policy);//获取'调度权重'起点min
		int sched_get_priority_max(int policy);//获取'调度权重'终点max
				policy参数(指定调度策略type):
					#define SCHED_NORMAL		0		//Invalid argument linux不允许使用这个值
					#define SCHED_FIFO			1
					#define SCHED_RR				2
					#define SCHED_BATCH			3
					// SCHED_ISO: reserved but not implemented yet(保留但尚未实现)
					#define SCHED_IDLE			5
					#define SCHED_DEADLINE	6

		* 根据调度策略type, 修改'实时调度策略结构体'(修改操作需要root权限)
		int sched_setscheduler(pid_t pid,int policy,const struct *sched_param);
		int sched_getscheduler(pid_t pid);		 //获取当前的权重值

		* 直接修改'实时调度策略结构体'(废弃, 少用)
		int sched_setparam(pid_t pid, const struct *sched_param);
		int sched_getparam(pid_t pid, const struct *sched_param);


	默认版的实时调度策略结构体struct sched_param:
		struct sched_param {
			int sched_priority;
		};
	sizeof(struct sched_param)=4; 只有一个int 了, 现在禁止手动修改线程调度策略;

	***************************废弃***************************
	实时调度策略结构体(废弃, 最新版linux已禁用这个结构体):
		struct sched_param{
			int32_t sched_priority;								//设置的调度权重(可获取, 可设置, 范围:1-99)
			int32_t sched_curpriority;						//当前调度权重(设置时忽略该参数, 只能获取时返回)
			union{
				int32_t reserved[8];
				struct{
					int32_t __ss_low_priority;				//线程的最低优先级(不能高于sched_priority)
					int32_t __ss_max_repl;						//线程降级的最大阻塞次数(一个线程多次阻塞之后,会自动降级运行,直到后续运行热度了,才会恢复正常的优先级)
					struct timespec __ss_repl_period;	//降级恢复的等待时间
					struct timespec __ss_init_budget;	//最高优先级使用时间
				}__ss;
			}__ss_un;
		}
		sched_priority			必须总是高于sched_ss_low_priority
		sched_ss_max_repl		必须小于SS_REPL_MAX
		sched_ss_init_budget必须大于sched_ss_repl_period

		struct timespec{
			time_t tv_sec; // seconds秒
			long tv_nsec;	 // nanoseconds纳秒(1s=10^9ns)
		};

	实时调度策略宏定义:
		#define sched_ss_low_priority   __ss_un.__ss.__ss_low_priority
		#define sched_ss_max_repl       __ss_un.__ss.__ss_max_repl
		#define sched_ss_repl_period    __ss_un.__ss.__ss_repl_period
		#define sched_ss_init_budget    __ss_un.__ss.__ss_init_budget
	***************************废弃***************************
*/



#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	int min,max,pid;
	struct sched_param m_param;
	struct timespec tp;

	pid = getpid();

	min = sched_get_priority_min(SCHED_FIFO);
	max = sched_get_priority_max(SCHED_FIFO);
	printf("SCHED_FIFO调度策略下, 当前权值: min=%d, max=%d\n",min,max);


	m_param.sched_priority = 18;													//设置线程正常权重(默认为最低值1)
	if(sched_setscheduler(pid,SCHED_FIFO,&m_param) == -1){//需要root权限
		perror("sched_setscheduler()");
		return -1;
	}

	printf("sched_getscheduler() = %d\n",sched_getscheduler(pid));


	printf("SCHED_FIFO调度策略下, 当前权值: min=%d, max=%d\n",\
		sched_get_priority_min(SCHED_FIFO),sched_get_priority_max(SCHED_FIFO));

	printf("sizeof(struct sched_param)=%d\n",sizeof(struct sched_param));


	sched_yield();																				//主动挂起线程

	sched_rr_get_interval(pid, &tp);											//获取cpu 时间片的信息(SCHED_FIFO为0)
	printf("cpu 的时间片长度: %ds,%dns\n",tp.tv_sec,tp.tv_nsec);

	return 0;
}
