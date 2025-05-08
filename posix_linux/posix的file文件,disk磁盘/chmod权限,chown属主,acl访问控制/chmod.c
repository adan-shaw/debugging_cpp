//编译:
//		gcc -g3 ./chmod.c -o x 



#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



//创建symlink链接, 和chmod 改变权限测试
int main(void){
	int fd;



	//1.创建test 目录
	if(mkdir("test_dir", S_IRUSR | S_IWUSR | S_IXUSR) == -1){
	//if(mkdir("test_dir", 0600) == -1){
		perror("mkdir()");
		return -1;
	}

	//2.改变程序的运行主目录(确认目录存在, 可以去掉if)
	if(chdir("test_dir") == -1){
		perror("chdir()");
		return -1;
	}

	//3.改变执行权限(确认目录存在, 可以去掉if)
	if(chmod(".", S_IRUSR) == -1){
		perror("chmod()");
		return -1;
	}

	//4.打开文件
	fd = open("myfile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("open()");
		return -1;
	}

	//5.关闭文件
	close(fd);

	return 0;
}

