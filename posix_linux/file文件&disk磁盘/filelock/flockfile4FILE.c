//编译:
//		gcc -g3 -lpthread ./flockfile4FILE.c -o x 


//flockfile() 对'FILE文件流'加锁的使用:
/*
	flockfile()互斥属性的, 加锁之后就是对'FILE文件流'独占rw;

	flockfile() 系列的api 主要包括:
		void flockfile(FILE *filehandle);		//对'FILE文件流'加锁, 阻塞等待, 直接抢到锁为止;
		int ftrylockfile(FILE *filehandle);	//询问性加锁, 如果可以加速, 则返回0, 并执行加锁;
		void funlockfile(FILE *filehandle);	//解锁
	以上3 个函数都是线程安全的, 放心使用.


	多线程之间, 共享FILE* 不能直接传递FILE* 指针;
	多进程之间, 连fd 也没办法共享, 只能打开同一个文件, 会有多进程竞争性问题;

	多线程之间, 没办法共享'FILE*文件流', 只能共享'fd文件句柄';
	所以传递'FILE*文件流'的时候, 需要转换为'fd文件句柄', 用作多线程之间传递;
*/




#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>



#define LOCK_FILE_PATH "./flockfile4FILE.c"



//flockfile_test 线程函数
void* flockfile_test_pth_fun(void *arg){
	char buf[64];
	int fd = arg;
	FILE *fp = fdopen(fd,"r");

	printf("flockfile_test_pth_fun() start at %d, fd=%d\n",time(NULL),fd);
	//尝试加锁, 如果可以加锁, 则抢占加锁
	if(!ftrylockfile(fp)){
		rewind(fp);
		fgets(buf,64,fp);
		printf("first line: %s\n",buf);
		funlockfile(fp);
	}
	printf("flockfile_test_pth_fun() finish at %d, fd=%d\n",time(NULL),fd);

	pthread_exit(0);
}



//flockfile 主测试函数
void flockfile_test(void){
	FILE *fp;
	int tmp;
	pthread_t tpid;

	fp = fopen(LOCK_FILE_PATH,"r");
	if(fp == NULL){
		perror("fopen()");
		return;
	}

	//主线程直接阻塞加锁
	flockfile(fp);

	//常见子线程, 尝试是否可以抢占fp 的使用权
	tmp = pthread_create(&tpid,NULL,flockfile_test_pth_fun,fileno(fp));
	if(tmp != 0){
		perror("pthread_create()");
		fclose(fp);
		return;
	}
	pthread_detach(tpid);

	sleep(1);//等子线程结束, main线程才继续运行
	printf("main 线程再次'询问加锁'ftrylockfile() = %d\n",ftrylockfile(fp));

	funlockfile(fp);

	fclose(fp);
	return;
}



int main(void){
	flockfile_test();
	return 0;
}





