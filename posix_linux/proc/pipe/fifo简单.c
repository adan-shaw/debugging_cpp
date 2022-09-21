//编译:
//		gcc ./fifo简单.c -g3 -o x


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
//fifo 中间文件使用权限(必须为0644, 不能写成644)
#define FIFO_MODE (O_CREAT|O_EXCL|0600)



//'fifo有名管道'-write 测试
void fifo_write(void){
	int fifo_fd, tmp, len, bytes;
	char buf[FIFO_FILE_SIZE];

	fifo_fd = open(FIFO_FILE_PATH, O_RDWR);			//打开'fifo文件'(不能用O_WRONLY,要用O_RDWR读写模式)
	if(fifo_fd == -1){
		perror("open()");
		return;
	}
	printf("write-process %d opened FIFO'%s', fd=%d, mode=%d\n",getpid(),FIFO_FILE_PATH,fifo_fd,O_RDWR);

	memset(buf, 'A', sizeof(buf));							//填充需要写入的数据
	buf[FIFO_FILE_SIZE-1] = '\0';

	bytes = 0;
	len = strlen(buf)+1;
	while(bytes < len){													//向'fifo文件'写入数据(阻塞写)
		tmp = write(fifo_fd, &buf[bytes], len-bytes);
		if(tmp == -1){
			perror("write()");
			close(fifo_fd);
			return;
		}
		bytes += tmp;
		printf("written data size: %d\n", tmp);		//for test only
	}
	close(fifo_fd);
	return;
}





//'fifo有名管道'-read 测试
void fifo_read(void){
	int fifo_fd, tmp, len, bytes;
	char buf[FIFO_FILE_SIZE];

	fifo_fd = open(FIFO_FILE_PATH, O_RDONLY);		//打开'fifo文件'(O_RDONLY|O_NONBLOCK '只读+非阻塞'模式打开文件)
	//fifo_fd = open(FIFO_FILE_PATH, O_RDONLY|O_NONBLOCK);
	if(fifo_fd == -1){
		perror("open()");
		return;
	}
	printf("read process %d opened FIFO'%s', fd=%d, mode=%d\n",getpid(),FIFO_FILE_PATH,fifo_fd,O_RDONLY);

	bytes = 0;
	do{																					//从'fifo文件'中读取数据(阻塞读)
		tmp = read(fifo_fd, &buf[bytes], FIFO_FILE_SIZE - bytes);
		//tmp = pread(fifo_fd, &buf[bytes], FIFO_FILE_SIZE - bytes,SEEK_SET);
		if(tmp == -1){
			perror("read()");
			close(fifo_fd);
			return;
		}
		bytes += tmp;
		printf("read %d size data\n", tmp);				//for test only

	}while(bytes < FIFO_FILE_SIZE);
	close(fifo_fd);
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



//pipe无名管道, 'fifo有名管道'的区别:
/*
	相同点:
		都是流式ipc 通信工具, 没有消息边界, 一般只用作传输字符串;
		没有'大小端数字存储'的区分, 可以传输数字变量, 结构体变量;


	不同点:
		* pipe无名管道有'内核pipe缓冲区',数据不会丢失,先写入会预存在'内核pipe缓冲区'中;
					如果'内核pipe缓冲区'已满, 则阻止继续写入, write() 失败返回-1;
			fifo有名管道无'内核pipe缓冲区',数据会丢失!!
					必须先阻塞读, 然后再写入数据, 否则mkfifo() 文件并不会帮你缓存数据;
					(mkfifo()文件,是一个无限大的设备文件,不可能直接用text编辑器去打开,否则卡死)

		* fifo有名管道支持兄弟进程之间,任意进程之间进行读写操作;
			pipe无名管道只支持父子进程之间,进行读写操作[ps aux|grep aaa#实际上是父子进程的关系,fork()+execl()]

		* fifo有名管道相当于创建一个临时设备文件,进行数据共享,需要使用open()打开设备fd,再进行read()/write()读写;
			pipe无名管道直接调用pipe()就自动返回两个阻塞读写io的fd,可直接read()/write()读写;

		* fifo有名管道没有缓冲区已满, 造成阻塞的情况, 非常适合异步读写, 可比肩unix socket, 性能更好;
			pipe无名管道, 一般适合阻塞读写(就算设置了非阻塞, 内核也会因为缓冲区已满而阻塞);
*/



//'fifo有名管道'与unix socket的比较:
/*
	'fifo有名管道'适合'单向/双向一对一', 单向一对多(需要加信号灯做同步协助)的观察者模式的使用场景;
	unix socket适合双向一对多, '单向/双向多对多'的复杂应用场景, 不需要信号灯做同步协助, 跟普通socket一样;

	但'fifo有名管道'的性能, 实时性, 吞吐量比unix socket 强;
*/



//'fifo有名管道'的使用规则:
/*
	* FIFO严格遵循先进先出(first in first out);
		读, 总是从'头部'弹出数据; 写, 则把数据插入到'末尾';

	* fifo,pipe都不支持诸如'lseek()定位'等文件操作,只支持简单的read()/write()读写;

	* 'fifo有名管道'的缓冲区是有限的, 系统默认分配一页内存(一般是4KB), 作为'fifo有名管道'的缓冲区,
		但由于'fifo有名管道'不会帮你缓冲数据的关系, 一般不会造成数据挤压,
		所以一页4KB缓冲区也够用, 不会因为缓冲区已满而阻塞;

	* 不要使用text编辑器去打开mkfifo()创建的临时文件,
		这种特殊设备文件, 不是普通文件, 即便文件大小等于0 byte, 也不能用打开text编辑器去打开,
		否则会卡死机, 用性能最好的vim 也会卡死;

	* fifo 不允许离线读取, 读写必须on time(同时操作);
		所以最好的实现就是, 一端阻塞读, 一端阻塞写(先读后写, 写入可阻塞可异步),
		如果先写入再读出, 则先写入的数据会丢失, 再读时陷入阻塞(读的也是下一次写入的数据,已经丢了一帧数据了);

	应该先读后写, 一端读等待, 一端写入, 才能读出数据(阻塞读,不能异步循环读,否则也容易出错);
	否则读端容易读到0数据or 陷入读阻塞[读到0数据表示:数据已丢失];
	fifo 管道是一个设备文件, 写入数据, 不暂存, 会马上销毁, 如果没有阻塞读等待, 容易丢失数据;
*/



//函数原型:
/*
	int mkfifo(const char *pathname, mode_t mode);
	int mkfifoat(int dirfd, const char *pathname, mode_t mode);
	成功返回0, 失败返回-1;

	参数说明:
		pathname: 文件路径
		mode: 		一般为: O_CREAT|O_EXCL|0600
							如果加入O_NONBLOCK, 则开启异步模式
							(不推荐,这样读端也会成为异步模式,会导致读端数据丢失,陷入阻塞or读到0数据)

	读端/写端io:
		open():					读端/写端, 都需要重写使用open() 打开mkfifo() 文件, 来生成fd;[不推荐使用fopen()]
		read()/write():	读端/写端, 都需要read()/write()进行io
		close():				读端/写端, 都需要关闭fd

	不推荐搞成FILE 文件流模式, 这简直脱了裤子放屁!!
	'fifo有名管道'本身就有内存缓冲区, 而且写的也不是磁盘, 而是直接内存to 内存,
	这时候再使用FILE 文件流, 再多建一个FILE 文件流缓冲区, 简直脱了裤子放屁!!
	而且'fifo有名管道'不支持'lseek()定位'等文件操作, 只支持简单的read()/write()读写;
	你搞这么复杂是没用的
*/

