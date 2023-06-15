//编译:
//		gcc -g3 daemon_easy.c -o x

//如何让一个进程变成daemon 进程
/*
	1.执行一个fork(), 父进程退出, 子进程继续执行, 让子进程被init 进程收养;
		ps: 
			daemon 进程最终是需要在后台运行的, 成为被init 进程收养, 是必然选择;
			创建子进程, 结束父进程, 让'主体程序'被init 进程收养, 只是第一步;


	2.由于'主体程序'所在的子进程刚被init 进程收养, 因此'主体程序'需要一个新的会话环境:
		子进程调用setsid()创建一个新会话, 并释放它与控制终端之间的所有关联;
		(让父进程所在的会话环境彻底结束, 子进程被init进程收养, '主体程序'开始新的运行环境;
		 由于子进程自己调用setsid()创建一个新会话, 子进程也成为会长进程, 
		 一个新的, 被init进程收养的, 带会话组的子进程被创建出来, 并且子进程自己就是会长)


	3.对于'终端设备'的处理:
		--如果daemon进程不打算打开'终端设备', 则不需做任何处理;

		--如果daemon进程, 运行过程中, 可能会打开一些终端设备,
			则必须采取措施, 确保该设备不会成为控制终端(确保daemon进程永远不使用tty)
			具体措施,有两种方法实现:
				1.在所有用到'终端设备'上的open()函数,
					 调用open()时, 指定O_NOCTTY 原子操作标记, 让open() 函数打开的fd 彻底脱离tty;

				2.在setsid()调用之后执行第二个fork(), 让子进程退出并让'孙进程'继续执行程序主体;
					这样确保了子进程不会成为会话组长, 而孙进程成为了组长.
					根据SystemV中获取终端规则, '孙进程'永远不会重新请求一个控制终端了.

	4.清除进程的umask(确保daemon进程创建文件和目录时拥有必要的权限)

	5.修改进程当前工作目录chdir()[通常改为根目录'/', 处于安全考虑]

	6.关闭daemon进程从父进程继承而来的所有打开着的文件描述符, 包括stdin, stdout, stderr;

	7.关闭0,1,2文件描述符后, daemon进程通常会打开/dev/null, 并让所有fd都指向该设备,
		这样确保了daemon进程调用这些fd, 执行IO的库函数时, 不会出乎意料的失败;
*/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>

//pid_t 是一个有符号短整形
//typedef short pid_t;

//exec_daemon() 说明:
/*
	由于父进程经过几次exit(0) 退出, 主体程序一旦调用这个exec_daemon()函数, 主体程序必然会自动退出, 
	后续的主体程序, 由exec_daemon()函数, 以daemon 程序的形式继续运行
*/
void exec_daemon(const char *path, const unsigned int argc, ...){
	int maxfd, tmp;
	pid_t sid;
	va_list valist;
	char **pstr, **pstr2;

	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		exit(-1);
	}
	if(tmp != 0)				//父进程结束
		exit(0);

	sid = setsid();
	if(sid == -1){			//子进程调用setsid()开启一个新会话
		perror("setsid()");
		exit(-1);
	}

	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		exit(-1);
	}
	if(tmp != 0)				//子进程结束
		exit(0);

	//孙进程继续执行(根据SystemV中获取终端规则,'孙进程'永远不会重新请求一个控制终端了)


	//清除孙进程的umask(确保孙进程创建文件和目录时, 拥有必要的权限)
	umask(0);

	//修改孙进程的工作目录(程序活动范围, 需要root 权限)
	if(chdir(".") == -1){
		perror("chdir()");
		exit(-1);
	}

	//关闭所有继承得来的fd(可选)
	maxfd = sysconf(_SC_OPEN_MAX);
	if(maxfd == -1){
		perror("sysconf()");
		exit(-1);
	}

	//执行遍历关闭所有fd
	for(tmp = 0; tmp < maxfd; tmp++)
		close(tmp);

	//重新打开stdin stdout stderr 描述符
	/*
		不同的是, 这次重新打开, 是将标准FILE流, 直接引到/dev/null 中, 直接丢弃数据!!
		这样做是为了确保daemon进程'调用某些IO的库函数'时,不会出乎意料的失败;
		在UNIX系统中, STDIN_FILENO表示标准输入, STDOUT_FILENO表示标准输出, STDERR_FILENO表示标准出错;
	*/
	tmp = open("/dev/null", O_RDWR);
	if(tmp == -1){
		perror("open()");
		exit(-1);
	}
	if(tmp > 2){			//新打开的fd, number 大于2, 则不影响stdin,stdout,stderr
		if(dup2(tmp, STDIN_FILENO) == -1){ perror("dup2()"); exit(-1); }
		if(dup2(tmp, STDOUT_FILENO) == -1){ perror("dup2()"); exit(-1); }
		if(dup2(tmp, STDERR_FILENO) == -1){ perror("dup2()"); exit(-1); }
		close(tmp);
	}
	else{							//新打开的fd, number 小于2, 与stdin,stdout,stderr 有重叠的情况
		switch(tmp){
		case 0:
			if(dup2(tmp, STDOUT_FILENO) == -1){ perror("dup2()"); exit(-1); }
			if(dup2(tmp, STDERR_FILENO) == -1){ perror("dup2()"); exit(-1); }
			break;
		case 1:
			if(dup2(tmp, STDIN_FILENO) == -1){ perror("dup2()"); exit(-1); }
			if(dup2(tmp, STDERR_FILENO) == -1){ perror("dup2()"); exit(-1); }
			break;
		case 2:
			if(dup2(tmp, STDIN_FILENO) == -1){ perror("dup2()"); exit(-1); }
			if(dup2(tmp, STDOUT_FILENO) == -1){ perror("dup2()"); exit(-1); }
			break;
		default:
			if(dup2(tmp, STDIN_FILENO) == -1){ perror("dup2()"); exit(-1); }
			if(dup2(tmp, STDOUT_FILENO) == -1){ perror("dup2()"); exit(-1); }
			if(dup2(tmp, STDERR_FILENO) == -1){ perror("dup2()"); exit(-1); }
			close(tmp);
		}
	}

	pstr = alloca(argc*sizeof(char*));
	pstr2 = pstr;

	va_start(valist, argc);
	for(tmp = 0; tmp < argc; tmp++)
		*pstr++ = va_arg(valist, char*);
	//pstr = pstr2;
	//最后调用execv(), 替换掉所有的进程段, 成为新的daemon 进程
	if(execv(path,pstr2) == -1){
		perror("execv()");
		exit(-1);
	}
	write(open("./text", O_RDWR),"love you",sizeof("love you"));
}



int main(void){
	pid_t pid;
	exec_daemon("/bin/ping",3,"-c","16","127.0.0.1");//终止当前进程, 拒绝继续执行, 变成daemon 程序, 调用新的进程



	//
	//不会再继续运行
	//
	//错误, 由于父进程经过'几代的子父孙进程'的exit() 退出, 后续代码根本不可能再向下运行了, 所以下面的代码都不会被执行;
	printf("frok_daemon() = %d\n",pid);
	if(kill(pid, 9) == -1)//杀死daemon 进程
		perror("kill()");
	return 0;
}
