//编译:
//		gcc -g3 ./fd文件读写read,write.c -o x 


/*
	1.read()是一个底层api,其他read系列函数,最终也会调用该函数(最底层的读操作)
	ssize_t pread(int fildes, void *buf, size_t nbytes, off_t offset);
	ssize_t read(int fildes, void *buf, size_t nbytes);
	ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);

	以下几种情况会导致读取到的字节数小于nbytes:
		A.读取普通文件时, 读到文件末尾还不够nbytes字节;
		  那么实际读到的只有30字节, read 函数返回30(read读的时候,文件光标也在移动)
		B.从终端设备(terminal device)读取时, 一般情况下每次只能读取一行;
		C.从网络读取时, 网络缓存可能导致读取的字节数小于nbytes字节;
		D.读取pipe或者FIFO时,pipe或FIFO里的字节数可能小于nbytes;
		E.从面向记录(record-oriented)的设备读取时,某些面向记录的设备(如磁带)每次最多只能返回一个记录;
		F.在读取了部分数据时被信号中断;
	注意:
		read不会自动加上'\0'; 对于'\n', 它也是原封不动的读入, 当做普通字符.


	2.write()是一个底层api,其他write系列函数,最终也会调用该函数(最底层的写操作)
	ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset);
	ssize_t write(int fildes, const void *buf, size_t nbyte);
	ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);


	3.pos游标移动与读写结合的原子操作:
		//写
		tmp = pwrite(fd,"1234567890",11,SEEK_SET);
		printf("pwrite() 已写入data长度=%d size\n",tmp);
		//读
		tmp = pread(fd,buf,31,(SEEK_SET+10));					//(从SEEK_SET后移10字节,再读取31字节)
		printf("pread()=%s, 已读出data长度=%d size\n",buf,tmp);



	4.io读写阻塞问题: 
			read()/write() 系列函数, 操作会触发阻塞, 直到io完成才返回;
			pread()/pwrite() 原子操作函数, 同样会阻塞;
		不过可以调成nonblocking 模式, 
		这时候, 读写数据会由内核代为操作, 应用程序马上返回API执行结果;



	5.设备io读写, 与磁盘io读写的区别:
		socket 一般都是网络设备的fd, 一种特殊fd;
		socket 不支持定位游标pos[lseek()], 只能用最原始的'块数据直写直读';

		socket 跨平台, 跨网络, 还要考虑网络字节序的问题,
		普通设备是可以直接跟主机PC 传递数字的, socket 在本地局域网也可以传递数字的;

		定位pos游标是'磁盘设备'的专属, 其他设备没有这个功能;
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdlib.h>



#define STR_BUF_MAX (256)
const char* test_file_path = "./fd_read_write.c";



//多重缓冲区-写入或读取(简单read()/write()省略,不做实验)
int main(void){
	int fd;
	ssize_t nRead;
	struct iovec iov[3];	//多缓冲区容器(读写3个缓冲区,n=3)
	struct stat myStruct;	//第一个读入iov[0]的用户数据
	int x;								//第二个读入iov[1]的int数据
	char str[STR_BUF_MAX];//第三个读入iov[2]的字符串数据



	//打开文件(只读模式)
	fd = open(test_file_path, O_RDONLY);
	if(fd == -1){
		perror("open() failed");
		return -1;
	}

	//iov 读取缓冲区不可能发生溢出, 以为会根据iov_len 进行截断, 超出则截断;
	//所以, 不用考虑太多其它问题;
	iov[0].iov_base = &myStruct;	//填入第一个读取缓冲区
	iov[0].iov_len = sizeof(struct stat);

	iov[1].iov_base = &x;					//填入第二个读取缓冲区
	iov[1].iov_len = sizeof(x);

	iov[2].iov_base = str;				//填入第三个读取缓冲区
	iov[2].iov_len = STR_BUF_MAX;

	//执行多缓冲区-读取操作
	nRead = readv(fd, iov, 3);
	if(nRead == -1){
		perror("readv");
		return -1;
	}
	printf("readed total bytes: %ld\n",(long)nRead);

	//打印读取到的数据(只打印读取到的数据长度):
	printf("iov[0].iov_len=%d\n",iov[0].iov_len);
	printf("iov[1].iov_len=%d\n",iov[1].iov_len);
	printf("iov[2].iov_len=%d\n",iov[2].iov_len);

	close(fd);
	return 0;
}



