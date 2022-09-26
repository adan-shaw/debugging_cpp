//编译:
//		gcc -g3 ./nice.c -o x 

//进程'nice值'优先级修改:
/*
	进程'nice值'优先级(非实时调度, 执行优先级的倾向权值)
	'nice值'间接地影响内核对进程的运行优先级调度算法,
	每个进程都有一个'nice值', 其范围[-20,+19],
	-20 为最高优先级, +19 为最低优先级, 默认值为0;

	进程的调度并不是严格按照'nice值'的层次进行划分的,
	相反, 'nice值'只是一个权衡因素

	api 也很简单:
		int getpriority(int which, id_t who);						//失败返回-1
		int setpriority(int which, id_t who, int prio);	//失败返回-1
	which 参数:
		which = PRIO_PROCESS, who = 0, 则作用对象是当前调用进程
		which = PRIO_PGRP,    who = 0, 则作用对象是当前调用进程组
		which = PRIO_USER,    who = 0, 则作用对象是当前调用进程的用户
	prio:
		设置优先级时, 指定的'nice值'
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	int pid, nice;

	pid = getpid();
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
