//编译:
//		gcc -g3 ./nice.c -o x 

//进程'nice值'优先级修改:
/*
	进程'nice值'优先级(非实时调度, 执行优先级的倾向权值)
	'nice值'间接地影响内核对进程的运行优先级调度算法,
	每个进程都有一个'nice值', 其范围[-20,+19], -20 为最高优先级, +19 为最低优先级, 默认值为0;

	进程的调度并不是严格按照'nice值'的层次进行划分的('nice值'只是一个权衡因素);

	//修改当前进程的nice值(只修改一个进程), 失败返回-1[该函数貌似已经被弃用, 慎用]
	int nice(int incr);


	//修改nice值Ex(升级版, 实际也是用nice 值实现的, prio 的范围也是[-20,19])
	int getpriority(int which, id_t who);						//失败返回-1
	int setpriority(int which, id_t who, int prio);	//失败返回-1
		which 参数:
			which = PRIO_PROCESS, who = 0, 则作用对象是当前调用进程
			which = PRIO_PGRP,    who = 0, 则作用对象是当前调用进程组
			which = PRIO_USER,    who = 0, 则作用对象是当前调用进程的用户
		prio:
			设置优先级时, 指定的'nice值', 对SCHED_OTHER/SCHED_RR 有效;
			当nice 值小于0 时, 需要root权限;


	SCHED_OTHER分时进程:
		通过nice和counter值决定权值, 权值计算公式:(counter+20-nice)
		nice越小(负值), counter越大, 被调度的概率越大;
		nice越小(负值), 时间片越长, cpu占用时间越长;
		nice 值:
			每个进程手动设置;
		counter 值:
			每次'就绪队列'中选择一个任务执行, 被选中的任务, 执行完之后, counter就会被重置为0;
			没有被选中的任务, counter += 1;
			使用了cpu最少的进程, counter值会越来越大, 强得cpu使用权的权值, 也会越来越大;
			一旦抢到使用权, 使用结束后, counter=0;
			counter 值越大, 被执行的概率越大, 但不影响进程本身拥有的时间片大小;


	SCHED_RR实时进程:
		nice值(nice值将会转换为该任务的时间片的长度)
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	int pid, nice;

	pid = getpid();

	//被弃用? gcc报错:error: called object ‘nice’ is not a function or function pointer
	//if(nice(9) == -1){ perror("nice()"); return -1;}

	nice = getpriority(PRIO_PROCESS, pid);
	printf("getpriority() = %d\n", nice);

	nice += 1;
	if(setpriority(PRIO_PROCESS, pid, nice) == -1){
		perror("setpriority()");
		return -1;
	}

	printf("getpriority() = %d\n", getpriority(PRIO_PROCESS, pid));
	return 0;
}
