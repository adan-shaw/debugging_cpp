//编译:
//		gcc -g3 ./flock4fd.c -o x 


//flock() 对文件加锁的使用:
/*
	int flock(int fd,int operation);
	返回0表示成功, 若有错误则返回-1, 错误代码存于errno;

	operation 参数有下列四种情况:
		LOCK_SH 建立共享锁定, 多个进程可同时对同一个文件作共享锁定;
		LOCK_EX 建立互斥锁定, 一个文件同时只有一个互斥锁定;
		LOCK_UN 解除文件锁定状态;
		LOCK_NB 无法建立锁定时, 此操作可不阻塞, 马上返回;
						通常用法: LOCK_SH|LOCK_NB, LOCK_EX|LOCK_NB ...


	一个文件无法同时建立: 共享锁定 & 互斥锁定,
	而且当使用dup()或fork()时, 'fd文件描述符'不会继承flock()锁属;

	不同的'fd文件描述符'指向同一个文件, 虽然可以加锁, 但实际还是要等待, 阻塞, 直到获取到使用权为止;
	一个'fd文件描述符', 可以加一次锁, 排队由内核决定;(不能二次加锁)

	哪个进程的哪个描述符加锁,必须由这个进程,这个描述符自己解锁,别人没办法解锁.


	(阻塞用法)
		进程调用flock()尝试锁文件时, 如果文件已被其他进程锁住, 进程会被阻塞直到锁被释放掉;
	(非阻塞用法)
		在调用flock()的时候, 如果添加LOCK_NB参数, 则在尝试锁住该文件的时候, 发现已经被其他服务锁住, flock()会返回错误.
	(释放锁)
		调用flock(fd,LOCK_UN)可释放文件锁,
		调用close(fd)也可释放文件锁,
		这意味着进程的关闭会导致flock()会被自动释放

*/

//文件锁的用途:
/*
	对运行的.exe 文件进行加锁, 可以防止.exe 运行过程中, 被人篡改数据;
	这个办法可以防止:
		dll 注入, 恶意代码注入, 修改.exe 程序破解, 等手段;
		运行前对.exe 进行md5 完整性校验;
		运行过程中, 对.exe 文件进行上锁, 独占, 防止被篡改;

	该方法, 也可以用在dll/.so 等运行时库中(动静链接库);

	该方法, 也可以用来防止.exe 打开了两个程序, 也可以用来保证.exe 的唯一性;

	一般使用flock() 强制锁, 比较有效;
*/



#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>



#define TEST_FILE_PATH "./lock_file_test_data"



void father_lock(void){
	int fd;

	printf("father_lock()\n");

	fd = open(TEST_FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0600);
	if(fd == -1){
		perror("open()");
		return ;
	}

	flock(fd, LOCK_SH);			//阻塞加共享锁
	flock(fd, LOCK_UN);			//解锁

	flock(fd, LOCK_EX);			//阻塞加互斥锁
	sleep(2);								//for test [父进程互斥独占, 卡死子进程, 子进程报错: Resource temporarily unavailable]
	flock(fd, LOCK_UN);			//解锁

	close(fd);							//关闭fd会自动释放flock()

	printf("father_lock() end\n");
	return ;
}

void son_lock(void){
	int fd;

	printf("son_lock()\n");

	//等父进程上锁了再nonblocking 尝试加锁
	sleep(1);

	fd = open(TEST_FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0600);
	if(fd == -1){
		perror("open()");
		return ;
	}

	if(flock(fd, LOCK_SH | LOCK_NB) == -1)			//阻塞加共享锁
		perror("flock() LOCK_SH");
	if(flock(fd, LOCK_UN | LOCK_NB) == -1)			//解锁
		perror("flock()");
	if(flock(fd, LOCK_EX | LOCK_NB) == -1)			//阻塞加互斥锁
		perror("flock() LOCK_EX");
	if(flock(fd, LOCK_UN | LOCK_NB) == -1)			//解锁
		perror("flock()");
	close(fd);																	//关闭fd会自动释放flock()

	printf("son_lock() end\n");
	return ;
}



int main(void){
	int tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return -1;
	}
	if(tmp == 0){
		son_lock();					//子进程
		exit(EXIT_SUCCESS);	//子进程结束
	}
	father_lock();				//父进程
	exit(EXIT_SUCCESS);		//父进程结束
}
