//编译:
//		gcc -g3 ./setaffinity线程绑定cpu内核.c -o x



//setaffinity线程绑定cpu内核, 提高cpu 亲和力:
/*
	CPU 亲和力(非实时调度优化手段, cpu缓存倾向调度, 作用: 减少进程/线程切换时的消耗)
	ps: 不同进程的线程, 都可以绑定到cpu 内核上, 一个内核允许多次绑定;
	#define _GNU_SOURCE (1)
	#define __USE_GNU (1)
	#include <sched.h>
	int sched_setaffinity(pid_t pid,size_t cpusetsize,const cpu_set_t *mask);
	int sched_getaffinity(pid_t pid,size_t cpusetsize,cpu_set_t *mask);//失败返回-1

	void CPU_ZERO(cpu_set_t *set);
	void CPU_SET(int cpu, cpu_set_t *set);
	void CPU_CLR(int cpu, cpu_set_t *set);
	int CPU_ISSET(int cpu, cpu_set_t *set);//成功返回1, 失败返回0
*/



//"setaffinity线程绑定cpu内核 && SMP对称多处理器"绑定的实现:
/*
	Linux 内核的'SMP对称多处理器'的调度, 就是将进程迁移到合适的处理器上, 并保持各个处理器的负载均衡;
	本质上, Linux会自动兼容'SMP对称多处理器', 这是操作系统层面的工作;
	所以, 软件程序员, 只需关注:
		Linux 是以进程为单位, 进行cpu 调度管理的;
		Linux 并没有真正意义上的pthread线程, Linux 的线程, 是通过进程减配得来的;
		只要线程拥有pid, 它就可以通过sched_getaffinity() 函数, 决定这条线程跑在哪个编号的cpu上面;

		无论Linux 系统有多少个处理器, 其cpu 内部子内核+超线程, 反正就是: 一条物理指令流水线, 一个SMP cpu编号;
		(这时候, 就方便你做sched_getaffinity()绑定操作)

	所以, 通过sched_getaffinity()绑定操作, 可以决定进程绑定到哪个SMP 子内核, 运行在哪个子内核上面;
*/



//setaffinity线程绑定cpu内核的优势:
/*
	优势1, 高效利用CPU 的缓存系统:
		setaffinity线程绑定cpu内核后, 可以提高二级缓存, 三级缓存的命中率;
		(但不能提高一级缓存的命中率, 因为每次切换线程, 一级缓存都会清空)

	优势2, 合理划分linux 系统的cpu 资源: 
		如果linux 系统只有一个cpu, 这种setaffinity()绑定操作意义不大;
		但也可以对内做功能划分:
			把繁忙任务, 单独绑定到一个cpu;
			把闲时任务, 共享绑定到一个cpu;
		这样做的好处:
			至少可以提高2 级缓存的命中率(intel 二级缓存是每个内核独占的);
			让繁忙cpu 的实时效率更高;
			让软件系统整体的cpu计算吞吐量更大, 性能更好;
			让'0号cpu子内核'的更空闲, 提高io效率;(坊间传闻: intel CPU io操作, 只折腾'0号cpu子内核')

	详情, 需要看:
		cpu处理器的数量, cpu类型和架构, cpu缓存设计机制, 内存容量, 业务需求等综合信息, 决定是否值得绑定;

	最终确认:
		setaffinity线程绑定cpu内核后, 可以让程序只跑在这个cpu 子内核上面, 这是一个超级酷的功能;(超级有用)
*/



#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define _GNU_SOURCE (1)
#define __USE_GNU (1)
#include <sched.h>



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
