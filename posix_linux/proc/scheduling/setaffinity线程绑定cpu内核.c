//编译:
//		gcc -g3 ./setaffinity线程绑定cpu内核.c -o x

//setaffinity线程绑定cpu内核, 提高cpu 亲和力:
/*
	CPU 亲和力(非实时调度优化手段, cpu缓存倾向调度, 作用: 减少进程/线程切换时的消耗)
	ps: 不同进程的线程, 都可以绑定到cpu 内核上, 一个内核允许多次绑定;
	#define _GNU_SOURCE (1)
	#define __USE_GNU (1)
	#include <sched.h>
	sched_setaffinity(pid_t pid,size_t cpusetsize,const cpu_set_t *mask);//失败返回-1
	sched_getaffinity(pid_t pid,size_t cpusetsize,cpu_set_t *mask);

	void CPU_ZERO(cpu_set_t *set);
	void CPU_SET(int cpu, cpu_set_t *set);
	void CPU_CLR(int cpu, cpu_set_t *set);
	int CPU_ISSET(int cpu, cpu_set_t *set);//成功返回1, 失败返回0

	cpu绑定时, 需要看cpu类型和架构, 看看缓存设计机制, 是否值得绑定;
	一般绑定之后, 只能提高二级缓存, 三级缓存的命中率, 并不能提高一级缓存的命中率;
	一级缓存, 每次切换线程都会清空;
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define _GNU_SOURCE (1)
#define __USE_GNU (1)
#include <sched.h>

#define CPU_COUNT (1)

int main(void){
	int pid;
	cpu_set_t cpu_set;

	pid = getpid();
	CPU_ZERO(&cpu_set);								//使用前清空

	if(sched_getaffinity(pid,sizeof(cpu_set),&cpu_set) == -1){
		perror("sched_getaffinity()");	//进程默认跑在所有cpu上面, 所以'所有cpu子内核', 都会是1
		return -1;
	}
	if(CPU_ISSET(0,&cpu_set) == 1)
		printf("cpu[0] = 1\n");
	if(CPU_ISSET(1,&cpu_set) == 1)
		printf("cpu[1] = 1\n");
	if(CPU_ISSET(2,&cpu_set) == 1)		//超线程技术, 也可以虚拟出一个子内核
		printf("cpu[2] = 1\n");
	if(CPU_ISSET(3,&cpu_set) == 1)
		printf("cpu[3] = 1\n");
	/*
	if(CPU_ISSET(4,&cpu_set) == 1)		//没有的内核, 不会显示
		printf("cpu[4] = 1\n");
	*/


	CPU_ZERO(&cpu_set);
	CPU_SET(0,&cpu_set);
	if(sched_setaffinity(pid,sizeof(cpu_set),&cpu_set) == -1){
		perror("sched_setaffinity()");	//让进程只跑在0 号cpu子内核上
		return -1;
	}

	printf("\n");
	if(sched_getaffinity(pid,sizeof(cpu_set),&cpu_set) == -1){
		perror("sched_getaffinity()");	//重新打印一次进程在cpu 子内核的分布运行情况(发现进程现在只跑在0 内核上)
		return -1;
	}
	if(CPU_ISSET(0,&cpu_set) == 1)
		printf("cpu[0] = 1\n");
	if(CPU_ISSET(1,&cpu_set) == 1)
		printf("cpu[1] = 1\n");
	if(CPU_ISSET(2,&cpu_set) == 1)
		printf("cpu[2] = 1\n");
	if(CPU_ISSET(3,&cpu_set) == 1)
		printf("cpu[3] = 1\n");

	return 0;
}
