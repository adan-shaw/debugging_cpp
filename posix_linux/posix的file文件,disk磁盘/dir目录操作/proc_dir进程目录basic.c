//编译:
//		gcc -g3 ./proc_dir进程目录basic.c -o x



#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <limits.h>
#include <dirent.h>
#include <stddef.h>

#include <libgen.h>



void proc_dir_basic_test(void){
	char cwdbuf[255];
	const char *ptmp;



	//获取进程当前的工作目录(不需要繁琐的询问, 一般不会出错)
	getcwd(cwdbuf, sizeof(cwdbuf));	//线程安全版本
	//getwd(cwdbuf);								//非线程安全版本
	printf("进程当前的工作目录:\n%s\n\n",cwdbuf);

	//(慎用/废弃)直接返回字符串版本(glibc 特有, 容易溢出)
	//printf("进程当前的工作目录2:%s\n", get_current_dir_name());

	//更改进程当前的工作目录(不需要繁琐的询问, 一般不会出错)
	chdir(".");
	//fchdir()

	//更改进程根目录(不需要繁琐的询问, 一般不会出错)
	chroot(".");


	//获取文件名(提取文件名, 不包括路径名)
	printf("basename: %s\n", basename(cwdbuf));

	//获取文件的路径名(提取路径名, 不包括文件名)
	printf("dirname: %s\n", dirname(cwdbuf));

	return ;
}



int main(void){
	proc_dir_basic_test();
	return 0;
}
