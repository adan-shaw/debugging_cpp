//编译:
//		gcc ./system\:execl+fork的原子操作版.c -g3 -o x 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//system() 函数简介:
/*
	system()函数执行了三步操作:
		1.fork一个子进程;
		2.在子进程中调用exec函数去执行command;
		3.在父进程中调用wait去等待子进程结束;
	system()成功返回fork() 子进程的pid, 失败返回-1;


	wait()/waitpid()把system() 子进程的结束状态返回后存于status, 有几个宏可判别结束情况:
		WIFEXITED(status):
			判断子进程是否为正常退出(通过_exit()正常退出), 如果是, 它会返回一个非零值;

		WEXITSTATUS(status):
			当WIFEXITED()返回非零值时, 我们可以用这个宏来提取子进程的返回值:
					如果子进程调用exit(5), WEXITSTATUS(status)就会返回5; 
					如果子进程调用exit(7), WEXITSTATUS(status)就会返回7; 
			请注意:
					如果进程不是正常退出的, WIFEXITED返回0(这个值就毫无意义)

		WIFSIGNALED(status):
			如果子进程是因为信号而结束, 则此宏值为真;

		WTERMSIG(status):
			如果子进程因信号而中止, 取得触发退出的信号代码,
			一般会先用WIFSIGNALED()来判断后, 才使用此宏;

		WIFSTOPPED(status):
			如果子进程处于暂停执行情况则此宏值为真,
			一般只有使用WUNTRACED 时才会有此情况;

		WSTOPSIG(status):
			如果子进程已暂停, 取得引发子进程暂停的信号代码,
			一般会先用WIFSTOPPED()来判断后才使用此宏;
*/



//打印子进程的退出状态(显示子进程为什么退出)
void print_wait_stat(int status, const char *attach_msg){
	if(attach_msg != NULL)						//打印附带msg
		printf("%s", attach_msg);

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



int main(void){
	int status = system("ls -l .");
	if(status == -1){
		perror("system()");
		return -1;
	}
	else{
		if(WIFEXITED(status) && WEXITSTATUS(status) == 127){
			printf("(Probably) could not invoke shell.(可能)无法调用该shell命令\n");
			return -1;
		}
		else
			//shell 执行成功, 打印子进程的statue
			print_wait_stat(status,NULL);
	}
	return 0;
}

