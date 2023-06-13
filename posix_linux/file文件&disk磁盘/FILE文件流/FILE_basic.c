//编译:
//		gcc ./FILE_basic.c -g3 -o x 



//1.FILE* 流对象的基本属性(每个流对象保有下列信息):
/*
	1) (C95)字符宽度: 未设置、窄或宽
	2) 缓冲状态: 无缓冲、行缓冲、全缓冲
	3) 缓冲区, 可为外部的用户提供缓冲区所替换
	4) I/O 模式: 输入、输出或更新(兼具输入与输出)
	5) 二进制/文本模式指示器
	6) 文件尾指示器
	7) 错误状态指示器
	8) 文件位置指示器(fpos_t 类型对象)
		 对于宽字符流包含剖析状态(mbstate_t 类型对象(C95))
	9) (C11)在多个线程读、写、寻位或查询流时避免数据竞争的再入锁
*/


//2.重新设置一个缓冲区
/*
	void setbuf(FILE *stream, char *buf);
	void setbuffer(FILE *stream, char *buf, size_t size);
	void setlinebuf(FILE *stream);

	上面三个API, 都是基于setvbuf 进行二次封装而已, 真正的修改FILE 缓冲区的函数是setvbuf;
	*I.等价于setbuf()/setbuffer()
	setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
	*II.等价于setlinebuf()
	setvbuf(stream, (char *) NULL, _IOLBF, 0);

	int setvbuf(FILE *stream, char *buf, int mode, size_t size);
	int mode:
		IONBF == 不缓存
		IOLBF == 行缓存
		IOFBF == 全缓存
*/


//3.mkstemp 创建临时文件
/*
	mkstemp函数在系统中以唯一的文件名创建一个文件并打开, 而且只有当前用户才能访问这个临时文件, 并进行读、写操作.
	mkstemp函数只有一个参数, 这个参数是个以"XXXXXX"结尾的非空字符串. 
	mkstemp函数会用随机产生的字符串替换"XXXXXX", 保证了文件名的唯一性. 
	函数返回一个文件描述符, 如果执行失败返回-1. 
	在glibc 2.0.6 以及更早的glibc库中这个文件的访问权限是0666, glibc 2.0.7以后的库这个文件的访问权限是0600. 
	临时文件使用完成后应及时删除, 否则临时文件目录会塞满垃圾. 
	由于mkstemp函数创建的临时文件不能自动删除, 所以执行完mkstemp函数后要调用unlink函数, 
	unlink函数删除文件的目录入口, 
	但临时文件还可以通过文件描述符进行访问, 直到最后一个打开的进程关闭文件操作符, 或程序退出后临时文件被自动彻底地删除. 
*/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



#define FS_BUF_LEN (4096)
#define FILE_PATH "./test_data"



//1.FILE* 文件流的基础操作
void fio_basic(void){
	FILE* pfs;
	char fs_buf[FS_BUF_LEN];
	char buf[64];

	pfs = fopen(FILE_PATH,"a+");
	if(!pfs){
		perror("fopen()");
		return;
	}

	setvbuf(pfs, fs_buf, _IOFBF, FS_BUF_LEN);

	//如果FILE流出错,则ferror(pfs)返回非0
	printf("file stream error is: %d\n",ferror(pfs));

	//fputs()向FILE*流中写入一串字符串(长度不限,遇到'\0'结束)
	fputs("welcome to new york\n",pfs);

	//判断pos游标是否到了文件尾部(end-of-file)
	if(feof(pfs))
		printf("pos游标已经移动到文件尾部'end-of-file'\n");
	else
		printf("pos游标还未移动到文件尾部'end-of-file'\n");


	//快速将pos 游标移动到文件头
	rewind(pfs);
	//截断式读取所有字符串, 遇到'\0' 结束(或者缓冲区'溢出截断'结束)
	while(!feof(pfs) && !ferror(pfs)){
		fgets(buf,64,pfs);
		printf("%s\n",buf);
	}

	//清除流错误
	clearerr(pfs);

	fclose(pfs);
	return;
}



//2.FILE* 'tmpfile文件流'的基础操作
void fio_basic_tmpfile_FILE(void){
	FILE* pfs;
	int fd;
	int tmp;
	char path_tmpfile[255] = {'\0'};				//tmpfile文件的路径
	char path_tmpfile_info[4096] = {'\0'};	//tmpfile文件路径的详细信息

	//创建一个tmpfile FILE*文件流
	pfs = tmpfile();
	if(pfs == NULL){
		perror("tmpfile()");
		return ;
	}

	//反向tmpfile 文件的fd 句柄
	fd = fileno(pfs);

	//根据fd 句柄, 打印tmpfile 的详细信息
	snprintf(path_tmpfile,sizeof(path_tmpfile), "/proc/self/fd/%d", fd);
	readlink(path_tmpfile,path_tmpfile_info,sizeof(path_tmpfile_info)-1);
	printf("path_tmpfile:%s\n",path_tmpfile);
	printf("path_tmpfile_info:%s\n",path_tmpfile_info);

	fclose(pfs);
	return ;
}



//3.fd 模式创建tmpfile
void fio_basic_tmpfile(void){
	int fd;
	char str_path[] = "tmp_XXXXXX";
	//创建临时文件
	fd = mkstemp(str_path);
	if(fd == -1){
		perror("mkstemp()");
		return;
	}
	//解除tmpfile所有link, 只要进程所有关联的fd 被关闭, str_path就会被自动删除
	unlink(str_path);

	//关闭tmpfile文件句柄
	close(fd);
}



int main(void){
	fprintf(stderr,"%s","love ing hell");
	fio_basic();
	fio_basic_tmpfile_FILE();
	fio_basic_tmpfile();
	return 0;
}









