//编译:
//		g++ ./popen.cpp -g3 -o x 



//函数原型和介绍:
/*
	函数原型:
		#include <stdio.h>
		FILE* popen(const char *command, const char *type);
		int pclose(FILE *stream);


	函数说明:
		popen()函数通过创建一个管道,
		然后调用fork()产生一个子进程,
		再用子进程来执行一个system("") shell命令, 又创建多一个进程.
		一个进程调用popen(), 即创建两个子进程, 一共三个进程.

		这个管道必须由pclose()函数关闭, 而不是fclose()函数.
		(因为还要释放额定的管道资源)

		popen() 有点类似socketpair(),
		是一套集成关联API的新API, 主要还可以避免产生'原子操作的问题'.
		但是消耗不会减少太多, 基本操作消耗,
		调用shell 和管道通信都不会少.

		pclose()函数关闭标准I/O流, 等待命令执行结束,
		然后返回shell的终止状态.
		如果shell不能被执行,
		则pclose()返回的终止状态与shell已执行exit()一样.
		(即结束状态, 都是与exit() 函数相关, 无论错误与否.)


	参数说明:
		command参数:
			command是一个指向以NULL结束的shell命令字符串的指针.
			这行命令将被传到bin/sh并使用-c标志, shell将执行这个命令.

		type参数:
			type只能是读或者写中的一种,
			得到的返回值(标准I/O流)也具有和type相应的只读或只写类型.
			如果type是"r"则文件指针连接到command的标准输出;
			如果type是"w"则文件指针连接到command的标准输入.

			前面提到这个流是单向的(只能用于读或写).
			向这个流'写内容'相当于'写入该命令的标准输入';

			'命令的标准输出'和调用popen()的进程相同;

			与之相反的, 从流中'读数据'相当于'读取命令的标准输出',
			'命令的标准输入'和调用popen()的进程相同.


	返回值:
		如果调用fork()或pipe()失败,
		或者不能分配内存将返回NULL,
		否则返回标准I/O流.

		popen()没有为内存分配失败设置errno值.
		如果调用fork()或pipe()时出现错误, errno被设为相应的错误类型.
		如果type参数不合法, errno将返回EINVAL.


	缺点:
		每个popen()调用, 不仅要启动一个被请求的程序,
		还要启动一个shell, 即每个popen()调用将多启动两个进程.
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>



//打印popen() 执行的结果
void print_result(FILE *fp);



int main(void){
	FILE *fp = NULL;

	//1.调用popen() 执行shell 命令
	fp = popen("sleep 1", "r");
	//fp = popen("ls -l", "r");
	//fp = popen("ping www.baidu.com", "r");
	if(!fp) {
		perror("popen() failed");
		exit(EXIT_FAILURE);
	}

	//2.打印popen() 执行的结果
	print_result(fp);

	//3.关闭popen() 打开的管道资源和fork()子进程资源
	pclose(fp);


	exit(EXIT_SUCCESS);
}



//打印popen() 执行的结果
void print_result(FILE *fp){
	char buf[64];



	if(!fp)
		return;

	printf("\n>>> ---\n\n");
	while(memset(buf, 0, sizeof(buf)), \
		fgets(buf, sizeof(buf) - 1, fp) != 0 ){
			printf("%s", buf);
	}
	printf("\n--- <<<\n");

	return;
}
