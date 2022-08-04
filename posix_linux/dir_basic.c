//编译:
//		gcc -g3 ./dir_basic.c -o x



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> // for 原子操作O_RDONLY 之类的
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#include <limits.h>
#include <dirent.h>
#include <stddef.h>

#include <libgen.h>



void dir_basic_test(void){
	int tmp;
	char cwdbuf[255];
	const char *ptmp;



	//0.获取当前路径的'指定信息', 如_PC_NAME_MAX=255, 允许的最大文件名长度
	tmp = pathconf(".", _PC_NAME_MAX);
	if(tmp == -1){
		perror("pathconf() failed");
		return;
	}
	printf("_PC_NAME_MAX=%d\n",tmp);


	printf("dir基础测试:\n");
	//1.重命名-文件
	tmp = rename("./x","./x_new");
	if(tmp == -1){
		perror("rename() failed");
		return ;
	}


	//2.创建link(目标, ‘生成的link 存放位置’和名称)
	tmp = link("./x_new","./x");
	if(tmp == -1){
		perror("link() failed");
		return;
	}


	//3.移除link
	tmp = unlink("./x");
	if(tmp == -1){
		perror("unlink() failed");
		return;
	}


	//4.创建文件夹
	tmp = mkdir("./.myMkdir",0600);
	if(tmp == -1){
		perror("mkdir() failed");
		return;
	}


	//5.移除文件夹
	tmp = rmdir("./.myMkdir");
	if(tmp == -1){
		perror("rmdir() failed");
		return;
	}


	//6.删除-文件
	tmp = remove("./x_new");
	if(tmp == -1){
		perror("remove() failed");
		return;
	}


	//7.1: 获取进程当前的工作目录
	/*
		//不需要繁琐的询问
		if(getcwd(cwdbuf,sizeof(cwdbuf)) == NULL){
			perror("getcwd() failed");
			return;
		}
	*/
	getcwd(cwdbuf,sizeof(cwdbuf));//线程安全版本
	//getwd(cwdbuf);							//非线程安全版本
	printf("进程当前的工作目录:\n%s\n\n",cwdbuf);

	//(慎用/废弃)直接返回字符串版本(glibc 特有, 容易溢出)
	/*
		ptmp = get_current_dir_name();
		printf("进程当前的工作目录2:\n%s\n\n",ptmp);
	*/


	//7.2: 更改进程当前的工作目录
	chdir(".");
	//fchdir()

	//7.3：更改进程根目录
	chroot(".");


	//8.1 获取文件名(提取文件名, 不包括路径名)
	printf("basename: %s\n", basename(cwdbuf));

	//8.2 获取文件的路径名(提取路径名, 不包括文件名)
	printf("dirname: %s\n", dirname(cwdbuf));

	return ;
}



int main(void){
	dir_basic_test();
	return 0;
}
