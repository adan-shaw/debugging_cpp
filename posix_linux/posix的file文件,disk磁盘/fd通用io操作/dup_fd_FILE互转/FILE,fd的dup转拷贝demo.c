//编译:
//		gcc -g3 FILE,fd的dup转拷贝demo.c -o x 

//多线程之间共享FILE 的经验:
/*
	多线程之间, 共享FILE* 不能直接传递FILE* 指针;
	多进程之间, 连fd 也没办法共享, 只能打开同一个文件, 会有多进程竞争性问题;

	多线程之间, 没办法共享'FILE*文件流', 只能共享'fd文件句柄';
	所以传递'FILE*文件流'的时候, 需要转换为'fd文件句柄', 用作多线程之间传递;
*/



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



#define FILE_PATH "./FILE&fd的dup转拷贝demo.c"



//fd to FILE* 测试
void fd2FILE_test(void){
	FILE *pfs;
	int fd;

	fd = open(FILE_PATH, O_RDONLY, 0600);
	if(fd == -1){
		perror("open()");
		return;
	}

	//资源申请下来了, 一般转换不会出错的
	pfs = fdopen(fd,"r");

	//清空资源
	fclose(pfs);
	//不需要再次调用close(),因为fclose()会自动调用close(),强行再次调用,只会报错:"Bad file descriptor"
	//fd = close(fd);
	return;
}



//FILE* to fd 测试
void FILE2fd_test(void){
	FILE *pfs;
	int fd;

	pfs = fopen(FILE_PATH,"r");
	if(!pfs){
		perror("fopen()");
		return;
	}

	//FILE* to int
	fd = fileno(pfs);

	//释放资源(释放资源不需要检查返回结果, 就像操作已经申请下来的资源一样, 不需要再操作)
	fclose(pfs);
	//close(fd);
	return;
}



int main(void){
	fd2FILE_test();
	FILE2fd_test();
	return 0;
}



