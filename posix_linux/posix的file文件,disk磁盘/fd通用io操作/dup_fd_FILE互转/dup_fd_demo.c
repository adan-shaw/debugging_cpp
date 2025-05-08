//编译:
//		gcc -g3 dup_fd_demo.c -o x 

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



/*
	一般情况下, fd 也不推荐dup 拷贝的!!
	否则同一个进程里, 有两个fd 指向同一个文件, 
	这时候要考虑'多线程/多进程'的同步问题, 十分麻烦!!(最好不要这样做)
*/
void dupfd_test(void){
	int idup,idup2;
	int fd;

	fd = open("./fd_dup_demo.c", O_RDONLY, 0600);
	if(fd == -1){
		perror("open()");
		return;
	}


	//dup()
	idup = dup(fd);
	printf("dup(fd)=%d\n",dup(fd));


	//dup2(): 指定生成后的fd number(默认fd max=1024, 可修改linux 内核配置增大范围)
	idup2 = dup2(fd,555);
	printf("dup2(fd,555)=%d\n",idup2);


	//清空所有fd, 不清空fd 也是内存泄漏的一种(dup 需要重复释放, FILE* 文件流不需要重复释放)
	close(fd);
	close(idup);
	close(idup2);
	return;
}



int main(void){
	dupfd_test();
	return 0;
}
