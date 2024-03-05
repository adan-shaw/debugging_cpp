//编译:
//	gcc -g3 ./fd_sync同步读写.c -o x 



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>



//测试函数: 普通file 直写型io(但实际也不是真的立刻直写,还要看内核调度,只是尽快直写)
/*
	内核对write()/read() 仍然有一定的缓冲调度机制, 但这个机制是不可控的.
	FILE* 是程序员可控的io流机制, 使用buf 内存缓冲区, 减少直接操作内核, 
	提高系统整体'多个进程'io读写能力(多个进程io操作,到了内核态也是需要排队的,由内核来调度)

	相比FILE* 的api, write()/read()实在太慢了, 
	应该避免频繁调用write()/read(), 
	write()/read() 会涉及到内核态的多个进程竞争问题, 涉及一些磁盘设备存储信息获取等操作, 十分慢;
*/
void io_basic(void){
	int fd,tmp;
	char buf[32];



	//1.open() 打开文件-直写无缓冲
	fd = open("./test_data", O_RDWR | O_CREAT | O_APPEND, 0600);
	if(fd == -1){
		perror("open()");
		return ;
	}


	//2.write() 直写文件
	//(socket 不支持游标pos lseek(), 只能原始的'块数据直写直读')
	//设备都需要这种块读写, pos 游标只是磁盘设备的专属
	strncpy(buf,"adan shaw",32);
	tmp = write(fd,buf,strlen(buf)+1);
	printf("write() len = %d size\n",tmp);


	memset(buf,'\0',32);
	//3.read() 直读文件
	lseek(fd,10,SEEK_SET);		//以追加文件打开,不重置到文件头,只能读取到EOF
	tmp = read(fd,buf,31);
	//tmp = read(fd,buf,32);	//记住要保留多一字节,用来存放'\0',否则乱码
	printf("read() len=%d, %s size\n",tmp,buf);

	//4.lseek() -- 单位是字节!!
	lseek(fd,10,SEEK_CUR);		//文件游标的当前pos, 向后偏移10 字节
	lseek(fd,10,SEEK_SET);		//文件游标重置到文件头, 向后偏移10 字节
	lseek(fd,10,SEEK_END);		//文件游标重置到文件尾, 向前偏移10 字节


	memset(buf,'\0',32);
	//5.lseek() + io 的原子操作(比较好用)
	//原子操作-写
	tmp = pwrite(fd,"1234567890",11,SEEK_SET);
	printf("pwrite() len=%d size\n",tmp);
	//原子操作-读
	tmp = pread(fd,buf,31,(SEEK_SET+10));//从SEEK_SET后移10字节,再读取31字节
	printf("pread()=%s, sum=%d size\n",buf);


	//6.立即截断文件(超出部分清除)
	//	注意, 不能对socket fd 进行截断, 有明文规定
	ftruncate(fd,32);					//截断已打开的fd , 肯定成功.

	//不一定成功, 权限, 存在不存在等问题, 需要加if 判断
	tmp = truncate("./test_data",32);
	if(fd == -1){
		perror("truncate()");
		return ;
	}


	//7.立即执行磁盘同步(这个才是真正的直写)
	fsync(fd);								//立刻同步fd 所有数据到磁盘
	fdatasync(fd);						//同步fd 差异数据到磁盘
	sync();										//立刻刷新内核所有的缓冲块


	//8.close()
	close(fd);

	return;
}



int main(void){
	io_basic();
	return 0;
}




