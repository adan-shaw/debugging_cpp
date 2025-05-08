//编译:
//		gcc -g3 pth多线程,proc多进程共享和拷贝FILE文件流.c -o x 

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



//1.文件流FILE* 的共享
/*
多线程文件流FILE* 的共享:
	一个进程打开一个文件, 只需要有一个FILE* 文件流!!
	多个线程共享这个FILE* 文件流, 就需要加mutex 互斥锁进行互斥访问;
	否则就会有'多线程'抢夺系统资源的问题!!

多进程文件流FILE* 的共享:
	多个进程打开一个文件, 可以使用信号量来进行资源访问同步, 只是信号灯操作比较复杂, 多进程之间实现同步, 比较麻烦, 但性能较好;

多进程文件流FILE* 的共享:
	多个进程打开一个文件, 可以加文件锁!! 实现起来比较简单, 但性能较差;
	(文件锁需要对整个文件上锁, 性能十分差, 性能不如: 多线程抢夺mutex 互斥锁资源, or 多进程抢夺信号量资源)
*/



//2.文件流FILE* 的拷贝
/*
多线程之间的文件流FILE* 的拷贝:
	多线程之间基本上没有文件流FILE* 拷贝的需求, 
	一个进程打开一个文件, 多线程基本上都是共享文件流FILE* 的,
	因此不建议进行文件流FILE* 的拷贝操作;

多进程之间的文件流FILE* 的拷贝:
	只有子父进程在fork() 的时候, 会自动dup() 拷贝部分文件流FILE* 资源,
	非子父进程, 不可能dup() 拷贝文件流FILE* 资源,
	因此, dup() 拷贝文件流FILE* 资源的操作, 其实很少用的, 至少在编程操作上, 很少当成是拷贝操作来用!!
	更多的是socket 套接字资源的拷贝备份, 但其实现也是在内部实现的, 不需要程序员手动实现!!
	如:
		tcp 协议中, accept() 会自动dup() 拷贝sfd->listen() 的文件句柄, 作为服务client 的原始sfd, 
		也会继承sfd->listen() 文件句柄的所有socketopt() 属性;
		但这些操作, 不想要你自己实现, 系统内部实现的;

小结:
	所以基本上, 拷贝FILE* 本身就是愚蠢的做法!!
	FILE* 拷贝, 只是多创建一个FILE* 实体而已,
	而且在释放资源的时候, 只要其中一个FILE* 调用fclose(), 也会默认调用close(),
	那么目标fd 被释放, 其余的FILE* 就会崩溃, 全部变成无效FILE*文件流;

	一个进程, 同一个文件, 有两个FILE*, 是极其愚蠢的做法;
	比较优秀的是多线程抢夺FILE* 的使用权, 这样还可靠一点;

	所以, 使用FILE* 打开文件, 一般也推荐独占式打开!!
*/



#define FILE_PATH "./pth多线程,proc多进程共享和拷贝FILE文件流.c"



void cpFILE_test(void){
	FILE *pfs,*pfs2,*pfs_dup,*pfs_err;
	int fd,tmp,tmp2;
	char buf[32];

	fd = open(FILE_PATH, O_RDONLY, 0600);
	if(fd == -1){
		perror("open()");
		return;
	}

	//资源申请下来了, 一般转换不会出错的
	pfs = fdopen(fd,"r");



	//1.'兜弯子'的方式, 拷贝FILE*
	//	实际意义不大,因为两个fs,FILE*都指向同一个文件,结果都是会访问冲突,完全就是脱了裤子放屁
	tmp = fileno(pfs);		//FILE* to int
	tmp2 = dup(tmp);			//dup fd
	pfs_err = fdopen(tmp2,"r");


	//2.重新调用fopen()
	//	如果已经用'独占的方式'打开FILE*, 第二次调用fopen() 会报错
	pfs_dup = fopen(FILE_PATH,"r");


	//3.两个FILE* 指针, 指向同一个FILE* 实体
	pfs2 = pfs;



	//清空所有FILE* 实体资源
	/*
		fclose() 会自动调用close(), 基本情况下, 只需要调用一次, 就足够了;
		一旦close(), 其他FILE* 都会成为内存泄漏实体, 所以, 重新一次: 
			不要在一个进程内, 同一个文件, 打开两个FILE*, 这是极其愚蠢的做法
	*/
	fclose(pfs);
	fclose(pfs_err);
	fclose(pfs_dup);

	return;
}



int main(void){
	cpFILE_test();
	return 0;
}

