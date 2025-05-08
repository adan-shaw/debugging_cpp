//编译:
//		gcc -g3 ./link.c -o x 



#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



//默认定义的文件路径字符串的最大MAX
//#define PATH_MAX
#define TEST_FIELNAME "./link.c"
#define TEST_LINK_NAME "./link.c2"



int main(void){
	struct stat statbuf;
	char buf[PATH_MAX];
	ssize_t numBytes;



	printf("PATH_MAX=%d\n", PATH_MAX);

	//0.创建link
	if(symlink(TEST_FIELNAME, TEST_LINK_NAME) == -1){
		perror("symlink() failed");
		return -1;
	}


	//1.读取路径目标stat info[只要路径存在, 一般不会出错]
	/*
	if(lstat(TEST_LINK_NAME, &statbuf) == -1){
		perror("lstat() failed");
		return -1;
	}
	*/
	lstat(TEST_LINK_NAME, &statbuf);


	//2.判断是否是Link
	if(!S_ISLNK(statbuf.st_mode)){
		printf("%s is not a symbolic link", TEST_LINK_NAME);
		return -1;
	}


	//3.读取link的内容到string buf 中(并非link 指向的目标的内容)
	numBytes = readlink(TEST_LINK_NAME, buf, PATH_MAX - 1);
	if(numBytes == -1){
		perror("readlink() failed");
		return -1;
	}
	buf[numBytes] = '\0';//Add terminating null byte
	printf("readlink: %s --> %s\n", TEST_LINK_NAME, buf);//打印读取到的link 内容


	//4.相对路径转换成绝对路径(一般不会出错)
	/*
	if(realpath(TEST_LINK_NAME, buf) == NULL){
		perror("realpath() failed");
		return -1;
	}
	*/
	realpath(TEST_LINK_NAME, buf);
	printf("打印转换后的realpath: %s --> %s\n", TEST_LINK_NAME, buf);



	//5.unlink() 也有删除功能, 如果是link 快捷方式, 则只会减少一个快捷方式,
	//	而并不会删掉主体文件, 详情自己看下面的说明:
	/*
		执行unlink()函数并不一定会真正的删除文件,
		它先会检查文件系统中此文件的连接数是否为1,
		如果不是1说明此文件还有其他链接对象,
		因此只对此文件的连接数进行减1操作.
		若连接数为1, 并且在此时没有任何进程打开该文件, 此内容才会真正地被删除掉.
		在有进程打开此文件的情况下, 则暂时不会删除,
		直到所有打开该文件的进程都结束时文件就会被删除.
	 */
	if(unlink(TEST_LINK_NAME) == -1){// Remove filename
		perror("unlink() failed");
		return -1;
	}


	return 0;
}

