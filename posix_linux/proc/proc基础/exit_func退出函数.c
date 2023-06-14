//编译:
//		gcc ./exit_func退出函数.c -g3 -o x 



//进程退出函数:
/*
	int on_exit(void (*function)(int , void *), void *arg);	//传递参数版
	int atexit(void (*function)(void));											//无参数传递,直接执行'atexit()函数指针'指向的函数
	void _exit(int status);																	//参数status的值一般情况下不超过128
	void _Exit(int status);																	//_exit()总是会成功的,不会失败的

	void exit(int status);																	//常见用法: exit(EXIT_SUCCESS); exit(EXIT_FAILURE);
*/



//进程终止行为概述:
/*
	1.进程有两种终止方式:
		1.异常终止(abnormal):
				这种终止方式是由内核发出信号, 通知进程终止的, 可能会产生核心转储(core dump)

		2.进程调用_exit() 的相关函数进程终止:
			exit()函数是c 语言标准库函数, 它会在调用_exit()前, 执行如下步骤:
				* 调用通过atexit() / on_exit()注册函数, 注册的退出清理程序
				* 刷新stdio 流, 将数据冲刷干净(或者说将有用数据保存到磁盘中)
				* 使用由status提供的值, 执行_exit(), 将status传入_exit()
			ps:
				main() 主函数return 0;, return -1; 相当于_exit(0);, _exit(-1);
				main() 主函数最终会自动调用exit(), 也就是最终会调用_exit();

			进程终止强制规则:
			(无论你是调用_exit(), 或者任何异常信号终止, 内核都会回收一下资源)
				* 关闭所有'已经打开的文件描述符', 目录流, 目录描述符, 字符集转换描述符等;
				* 关闭所有描述符, 也包括释放进程所持有的任何'文件锁';
				* 进程为每个system v 信号量, 所设置的semadj值, 将会被加到信号量值中;
				* 关闭进程打开的所有posix有名信号量[sem_close()];
				* 关闭进程打开的所有posix消息队列[mq_close()];
				* 移除任何mlock(),mlockall()建立的内存锁;
				* 取消进程所有内存映射mmap();
				* 分离(detach)任何已链接的system v 共享内存段,
					且对于各段的shm_nattch计数减1.
				* 如果该进程是tty 管理进程(进程组长),
					进程组长终止时, 会向所有组员发送SIGHUP,
					如果不做特殊处理, 组员收到SIGHUP, 默认也会跟着组长结束进程,
					除非你篡改组员的SIGHUP信号处理函数.
				* 如果进程组成为了孤儿进程组, 被init 收养,
					那么,只要这个进程组内有任何进程结束(包括子进程),
					组内就会广播SIGHUP信号, 随后又会广播SIGCONT信号



	2.return 与 exit()的区别:
		main() 函数执行return n; 会出发调用exit(n),
		只有一种情况下, return 与exit 是不一样的.
		(如果return 之后, 退出处理过程中, 所执行的任何步骤,
		 需要访问main()函数的本地变量, 那么main()函数返回会导致未定义的行为!!
		 例如:
			 调用setvbuf(),setbuff()时,引用了main()的本地变量,就会发生这种情况)
			 [即将某些默认的标准流buf, 改成main()内的本地变量, return!=exit了]

		编译标准导致return 与exit的差异:
			c99或更高版本的标准, return 的最后, 肯定会调用exit()
			c89/ansi c, return与return 存在差异,
			由于c89 标准下, 程序的退出状态, 取自于栈或特定CPU寄存器中的随机值,
			所以c89 标准下, 尽量不要用exit() 函数.
			(ps: c89 太小了, c89 时代也没有太多unix api, 所以可以理解.)



	3.atexit()与on_exit() 函数:
		有时候, 应用程序在进程终结的时候, 需要自动执行一些退出操作,
		这时候可以用atexit()与on_exit() 函数;

		但是, 当进程收到信号而终止时, 将不会调用退出处理程序,
		这一事实, 一定程序上限制了atexit()与on_exit() 函数的使用;

		如果要应对sig 退出导致的影响, 只能从信号处理函数入手,
		atexit()与on_exit() 函数会被架空;
*/



#define _BSD_SOURCE// Get on_exit() declaration from <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void atexit_func1(void){
	printf("atexit function1() called\n");
	return;
}

void atexit_func2(void){
	printf("atexit function2() called\n");
	return;
}

void on_exit_func(int exit_stat, void *arg){
	printf("on_exit_func() called: status=%d, arg=%ld\n", exit_stat, (long) arg);
	return;
}



void exit_func_test(void){
	if(on_exit(on_exit_func, (void *) 10) != 0){
		perror("on_exit()");
		return;
	}

	if(atexit(atexit_func1) != 0){
		perror("atexit()");
		return;
	}

	if(atexit(atexit_func2) != 0){
		perror("atexit()");
		return;
	}

	if(on_exit(on_exit_func, (void *) 2000000) != 0){
		perror("on_exit()");
		return;
	}
	return;
}



int main(void){
	exit_func_test();
	return 0;
}




