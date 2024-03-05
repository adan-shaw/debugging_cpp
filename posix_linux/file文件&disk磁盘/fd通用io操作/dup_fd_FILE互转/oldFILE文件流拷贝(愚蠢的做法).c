//编译:
//		gcc -g3 oldFILE文件流拷贝\(愚蠢的做法\).c -o x 

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



//FILE* 拷贝
/*
	任何时候, 一个进程打开一个文件, 只需要有一个FILE*,
	否则就会有'多线程/多进程'抢夺的问题, 需要加文件锁!!
	(这不是多线程抢夺FILE* 的使用权, 而是有两个FILE*, 
	 上锁需要对整个文件上锁, 性能十分差, 还不如多线程抢夺FILE* 的使用权高效)

	所以基本上, 拷贝FILE* 本身就是愚蠢的做法!!
	FILE* 拷贝, 只是多创建一个FILE* 实体而已,
	而且在释放资源的时候, 只要其中一个FILE* 调用fclose(), 也会默认调用close(),
	那么目标fd 被释放, 其余的FILE* 就会崩溃, 全部变成无效FILE*文件流;

	一个进程, 同一个文件, 有两个FILE*, 是极其愚蠢的做法;
	比较优秀的是多线程抢夺FILE* 的使用权, 这样还可靠一点;


	所以, 使用FILE* 打开文件, 一般也推荐独占式打开!!
*/

#define FILE_PATH "./oldFILE文件流拷贝(愚蠢的做法).c"



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
		一旦close(), 其他FILE* 都会成为内存泄漏实体, 
		所以, 重新一次: 
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

