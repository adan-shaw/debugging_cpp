//编译:
//		gcc ./fifo序列化.c -g3 -o x


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



//以管道max 作为标准(默认: 512 bytes * 8) ulimit -p
//#define FIFO_FILE_SIZE PIPE_BUF

//'fifo文件'的极限容量
#define FIFO_FILE_SIZE (1024 * 63)

//每次向'fifo文件'read读取n 字节数据
#define FIFO_FILE_READ_FRAME_SIZE (128)

//fifo 中间文件存放路径
#define FIFO_FILE_PATH "./tmp.fifo"
//fifo 中间文件使用权限(建议使用660, 兼容进程组之间通信; 666 兼容不同进程组之间通信)
#define FIFO_MODE (O_CREAT|O_EXCL|0600)

//结构体app_body{}
typedef struct app_body{
	int i1;
	double d2;
	char c3[64];//多余的字节位,继续写入; 虽然不能做到最小化拷贝,但胜在简单,数据对齐,拷贝速度快;
}body_t;



//'fifo有名管道'-write 测试
void fifo_write(void){
	int fifo_fd, tmp, len, bytes;
	body_t buf;

	fifo_fd = open(FIFO_FILE_PATH, O_RDWR);			//打开'fifo文件'(不能用O_WRONLY,要用O_RDWR读写模式)
	if(fifo_fd == -1){
		perror("open()");
		return;
	}
	printf("write-process %d opened FIFO'%s', fd=%d, mode=%d\n",getpid(),FIFO_FILE_PATH,fifo_fd,O_RDWR);

	buf.i1 = 999;
	buf.d2 = 88.88;
	strncpy(buf.c3,"hello fifo",64);						//填充需要写入的数据

	tmp = write(fifo_fd, &buf, sizeof(buf));		//向'fifo文件'写入数据(阻塞写)
	if(tmp == -1){
		perror("write()");
		close(fifo_fd);
		return;
	}
	close(fifo_fd);
	printf("written data size: %d\n", tmp);			//for test only
	return;
}





//'fifo有名管道'-read 测试
void fifo_read(void){
	int fifo_fd, tmp, len, bytes;
	body_t buf;

	fifo_fd = open(FIFO_FILE_PATH, O_RDONLY);		//打开'fifo文件'(O_RDONLY|O_NONBLOCK '只读+非阻塞'模式打开文件)
	//fifo_fd = open(FIFO_FILE_PATH, O_RDONLY|O_NONBLOCK);
	if(fifo_fd == -1){
		perror("open()");
		return;
	}
	printf("read process %d opened FIFO'%s', fd=%d, mode=%d\n",getpid(),FIFO_FILE_PATH,fifo_fd,O_RDONLY);

	tmp = read(fifo_fd, &buf, sizeof(buf));			//从'fifo文件'中读取数据(阻塞读)
	if(tmp == -1){
		perror("read()");
		close(fifo_fd);
		return;
	}
	close(fifo_fd);
	sleep(1);
	printf("read %d size data\n", tmp);					//for test only
	printf("struct body_t{} data: {%ld, %lf, %s}\n",buf.i1,buf.d2,buf.c3);
	return;
}

int main(void){
	int tmp;

	unlink(FIFO_FILE_PATH);											//强制unlink(), 防止已存在的fifo 产生冲突

	if((mkfifo(FIFO_FILE_PATH, FIFO_MODE) == -1) && (errno != EEXIST)){
		perror("mkfifo()");												//创建'fifo文件', 供后续打开使用
		return -1;
	}

	//注意:
	/*
		应该先读后写, 一端读等待, 一端写入, 才能读出数据(阻塞读,不能异步循环读,否则也容易出错);
		否则读端容易读到0数据or 陷入读阻塞[读到0数据表示:数据已丢失];
		fifo 管道是一个设备文件, 写入数据, 不暂存, 会马上销毁, 如果没有阻塞读等待, 容易丢失数据;
	*/
	tmp = fork();
	if(tmp == -1){
		perror("fork()");
		return -1;
	}
	if(tmp == 0){																//子进程
		fifo_read();															//'fifo有名管道'-read 等待
		exit(0);
	}
	else{																				//父进程
		sleep(1);																	//等子进程先陷入read读等待,父进程才写入数据
		fifo_write();															//'fifo有名管道'-write
		wait(NULL);																//回收子进程尸体
	}

	unlink(FIFO_FILE_PATH);											//unlink()释放'fifo有名管道'
	return 0;
}



