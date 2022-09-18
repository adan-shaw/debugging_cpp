//编译:
//		gcc ./fork\&vfork.c -g3 -o x 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



//0.vfork()的特点:
/*
	vfork()子进程必定先运行, 等到子进程调用exit()/execl()后, 父进程才能继续运行;
	vfork()其它特性, 跟fork()一样, 子进程与父进程共享数据(写时复制,读时共享)

	fork() 创建子进程, 父子进程, 谁先运行是随机的;
*/


//1.fork() 的语义:
/*
	fork()对父进程的'程序段、数据段、堆段以及栈段'实行: 读时共享, 写时拷贝;
	fork()+exec(), 可让新程序替换旧进程的代码段, 并重新初始化其'程序段、数据段、堆段以及栈段';

	大部分现代UNIX的实现(包括Linux)采用两种技术来fork():
		1.内核(Kernel)将每一进程的代码段标记为只读, 从而使进程无法修改自身代码, 方便父、子进程共享代码段;
		2.对于父进程数据段、堆段和栈段中的各页, 内核采用写时复制(copy-on-write)技术来处理.
			fork()之后, 内核会捕获'父进程&子进程'所有针对这些页面的修改企图, 
			并为将要修改的(about-to-be-modified)页面创建拷贝;

	代码段		.text			低内存地址
	数据段		.data
	BSS段		.bss
	堆				.heap
	栈				.stack		高内存地址

	fork()一旦结束, 写时复制出去的'程序段、数据段、堆段以及栈段', 会被init 进程正常释放, 
	仅保留少量进程信息, 等待wait() 释放[不释放就会成为僵尸进程];

	若子进程先结束时:
		如果父进程没有调用wait()/waitpid()函数, 子进程就会进入僵死状态, 成为僵尸进程;
	若父进程先结束时:
		子进程被init 进程收养, 直至子进程结束;
*/


//2.fork()子父进程共享'文件描述符'的问题:
/*
	读时共享, 写时拷贝, 也有难搞的地方, 那就是'文件描述符'共享问题;
	fork() 之后, 会马上对所有fd/FILE 进行拷贝复制, 
	这也是为什么socketpair(), pipe() 管道等操作, fork() 之后, 都要关闭一端的fd(将不用的fd进行close()关闭);

	fork() 之后, 子父进程共用文件描述符号, 必须加'fd锁/FILE锁', 否则容易产生冲突;
	详细如何使用'fd锁/FILE锁'文件锁, 你自己查看: file文件&disk磁盘/file_lock文件锁/*.c
	如果fd/FILE 太多, fork() 的成本消耗会很大;
*/



//3.fork()子父进程之间的CPU优先级, 线程调度, smp多核CPU架构标准, 并发的安全性等问题:
/*
	子父进程默认获取CPU优先级的机会是平均的, fork() 同样也可以进行线程调度:
		* 抢占策略修改;
		* nice 值修改;
		* 进程的main 线程绑定cpu, 或者进程内多条线程绑定cpu, 以此来提高二级cache 命中率;
		...


	fork()与smp多核CPU架构标准:
		fork()多进程, 天生就是完美迎合smp标准(smp的英文实际是:Symmetric Multi-Processor翻译过来就是:对称多进程);

		操作系统本身就是以进程为单位, 进行任务分配的;
		至于一个linux 下, 有多少个cpu, 应用层无需理会, 
		系统管理员(运维人员)通过命令, 将进程分发到不同cpu 进行(或者你用shell脚本实现自动分发启动也行);
		对于应用层c/c++ 设计来说, 跑在哪个cpu 都一样, 都是同一个linux 内核;
		(当然0 号cpu 主要负责io syscall 的执行, 实际应尽量让1,2,3 号cpu 更多地执行计算任务, 是真理, 但应用层不用考虑)


	'多线程/多进程'并发的安全性:
		子进程异常崩溃结束, 并不会影响父进程继续工作, 多进程之间的安全性隔绝, 做得比多线程好;
		就算你屏蔽了大部分信号, 减少了信号终止进程的可能性,
		但子线程指针崩溃, 会导致整个进程退出;
		而子进程指针崩溃, 父进程不会有任何影响(只是会多了一个僵尸子进程而已)
		因此, 多进程模型安全性略高一点;
		(虽然指针崩溃是低级错误, 实际项目中是不允许出现的, 
		 但仍有其他致命错误, 会导致子线程/子进程终止,
		 因此, 多进程模型实际上安全性要稍高一点, 性能会稍低一点)
*/



//4.进程环境变量访问指针
extern char **environ;



//快速创建子进程(不管对错, 默认为一定成功)
void quick_vfork(void){
	if(vfork() == 0){	//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	return;
}

void normally_vfork(void){
	int tmp;
	tmp=vfork();
	if(tmp == -1){
		perror("vfork()");
		return;
	}
	if(tmp == 0){			//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	return;
}



//快速创建子进程(不管对错, 默认为一定成功)
void quick_fork(void){
	if(fork() == 0){	//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	sleep(1);					//等待子进程结束, 再继续执行
	return;
}

void normally_fork(void){
	int tmp;
	tmp=fork();
	if(tmp == -1){
		perror("fork()");
		return;
	}
	if(tmp == 0){			//子进程
		printf("hi i am son process\n");
		exit(EXIT_SUCCESS);
	}
	sleep(1);					//等待子进程结束, 再继续执行
	return;
}



int main(void){
	quick_vfork();
	normally_vfork();
	quick_fork();
	normally_fork();
	return 0;
}
