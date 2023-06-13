//编译:
//		gcc -g3 fcntl_lock4fd.h fcntl_lock4fd.c -o x 


//fcntl 基础:
/*
	fcntl() 功能简单目录:
		*1.复制一个现有的描述符			(cmd = F_DUPFD)
		*2.获得／设置文件状态标记		(cmd = F_GETFL / F_SETFL)
		*3.获得／设置异步I/O所有权	(cmd = F_GETOWN / F_SETOWN)
		*4.获得／设置记录锁				(cmd = F_GETLK / F_SETLK / F_SETLKW) [参考: fcntl_lock4fd.c]
*/

//fcntl使用flock结构体对fd进行加锁:
/*
	struct flcok{
		short int l_type;		// 锁定的状态

		short int l_whence;	//决定l_start位置(这三个参数用于分段对文件加锁, 若对整个文件加锁, 则: l_whence=SEEK_SET,l_start=0,l_len=0;)
		off_t l_start; 			//锁定区域的开头位置
		off_t l_len; 				//锁定区域的大小

		pid_t l_pid;				//执行锁定的进程pid
	};

	l_type 有三种状态:
		F_RDLCK 建立一个供读取用的锁定
		F_WRLCK 建立一个供写入用的锁定
		F_UNLCK 删除之前建立的锁定

	l_whence 也有三种方式:
		SEEK_SET 以文件开头为锁定的起始位置.
		SEEK_CUR 以目前文件读写位置为锁定的起始位置
		SEEK_END 以文件结尾为锁定的起始位置.


	fcntl()参数使用说明:
		start = lseek() 开始, 往后len bit 字节的文件内存, 全部上锁;
		whence = 起始相对位, 和lseek() 一样;
		若对整个文件加锁, 则:
			l_whence=SEEK_SET,l_start=0,l_len=0;

		加锁, 就是对文件的一部分内容进行加锁, 防止这部分内容被修改, 这种锁叫: 行锁(或者叫区域锁)
		至于锁整个文件的那种, 可以用flock() 或者flockfile().
*/



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "fcntl_lock4fd.h"



#define TEST_FILE_PATH "./lock_file_test_data"



void fcntl_lock_test(void);



int main(void){
	fcntl_lock_test();
	return 0;
}



//设置一把读锁
void readLock(int fd, short start, short whence, short len){
	struct flock lock;
	lock.l_type = F_RDLCK;	//short int
	lock.l_start = start;		//long int
	lock.l_whence = whence;	//short int
	lock.l_len = len;				//long int
	lock.l_pid = getpid();	//pid_t
	//阻塞方式加锁
	fcntl(fd, F_SETLKW, &lock);
}

//设置一把读锁(不阻塞等待加锁)
int readLocknb(int fd, short start, short whence, short len){
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();
	//非阻塞方式加锁
	return fcntl(fd, F_SETLK, &lock);
}

//设置一把写锁
void writeLock(int fd, short start, short whence, short len){
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();
	//阻塞方式加锁
	fcntl(fd, F_SETLKW, &lock);
}

//设置一把写锁(不阻塞等待加锁)
int writeLocknb(int fd, short start, short whence, short len){
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();
	//非阻塞方式加锁
	return fcntl(fd, F_SETLK, &lock);
}

//解锁(统一解锁的方式)
void unlock(int fd, short start, short whence, short len){
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();
	//执行解锁
	fcntl(fd, F_SETLKW, &lock);
}



void fcntl_lock_test(void){
	int fd;
	FILE* pfs;

	fd = open(TEST_FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0600);
	if(fd == -1){
		perror("open()");
		return;
	}
	printf("fd = %d\n", fd);

	pfs = fdopen(fd,"a+");
	fputs("sdjhfjkshfsdrngdufgndfugdnfghdfjghdfjghdfjgndjkfgn\n",pfs);
	fputs("sdjhfjkshfsdrngdufgndfugdnfghdfjghdfjghdfjgndjkfgn\n",pfs);
	fputs("sdjhfjkshfsdrngdufgndfugdnfghdfjghdfjghdfjgndjkfgn\n",pfs);
	fputs("sdjhfjkshfsdrngdufgndfugdnfghdfjghdfjghdfjgndjkfgn\n",pfs);
	fflush(pfs);
	rewind(pfs);

	readLock(fd,SEEK_SET,SEEK_END,32);							//加读锁-阻塞
	if(readLocknb(fd,SEEK_SET,SEEK_END,32) == -1){	//加读锁-非阻塞
		perror("readLocknb()");
		return;
	}

	unlock(fd,SEEK_SET,SEEK_END,32);								//解锁
	unlock(fd,SEEK_SET,SEEK_END,32);								//解锁多次解锁2
	unlock(fd,SEEK_SET,SEEK_END,32);								//解锁多次解锁3

	writeLock(fd,SEEK_SET,SEEK_END,32);							//加写锁-阻塞
	if(writeLocknb(fd,SEEK_SET,SEEK_END,32) == -1){	//加写锁-非阻塞
		perror("writeLocknb()");
		return;
	}

	fclose(pfs);
	return;
}




