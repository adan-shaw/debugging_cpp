//编译:
//		gcc -g3 ./dir遍历操作old.c -o x



//dir 目录操作api.list(均以getdents()为基础, 但已纳入SUSv3)
/*
	int							alphasort(const struct dirent**, const struct dirent**);
	int							closedir(DIR *);
	int							dirfd(DIR *);
	DIR							*fdopendir(int);
	DIR							*opendir(const char *);
	struct dirent		*readdir(DIR *);
	int							readdir_r(DIR *restrict, struct dirent *restrict, struct dirent **restrict);
	void						rewinddir(DIR *);
	int							scandir(const char *, struct dirent ***, int (*)(const struct dirent *), \
									int (*)(const struct dirent**, const struct dirent**));
	void						seekdir(DIR *, long);
	long						telldir(DIR *);
*/

//struct dirent 结构体
/*
	struct dirent{
		long d_ino;									// inode number 索引节点号 
		off_t d_off;								// offset to this dirent 在目录文件中的偏移 
		unsigned short d_reclen;		// length of this d_name 文件名长 
		unsigned char d_type;				// the type of d_name 文件类型 
		char d_name [NAME_MAX+1];		// file name (null-terminated) 文件名, 最长255字符 
	}

	d_type 文件类型:
		enum{
			DT_UNKNOWN = 0,		未知类型
			DT_FIFO = 1,			命名管道
			DT_CHR = 2,				字符设备
			DT_DIR = 4,				文件夹
			DT_BLK = 6,				块设备
			DT_REG = 8,				常规文件
			DT_LNK = 10,			符号链接
			DT_SOCK = 12,			unix socket套接字
			DT_WHT = 14				链接
		};
*/

//DIR 结构体
/*
	struct __dirstream{
		void *__fd;											// `struct hurd_fd' pointer for descriptor.
		char *__data;										// Directory block.
		int __entry_data;								// Entry number `__data' corresponds to.
		char *__ptr;										// Current pointer into the block.
		int __entry_ptr;								// Entry number `__ptr' corresponds to.
		size_t __allocation;						// Space allocated for the block.
		size_t __size;									// Total valid data in the block.
		__libc_lock_define (, __lock)		// Mutex lock for this structure.
	};
	typedef struct __dirstream DIR;
*/



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#include <limits.h>
#include <dirent.h>
#include <stddef.h>

#include <libgen.h>
#include <time.h>



//文件目录的最大字符串
#define FILEPATH_MAX (256)



//遍历某个目录下的所有文件-线程安全版本
void listFiles_r(const char *dirpath);
//void listFiles_r(const char *dirpath, const char *file_prefix);

//遍历某个目录下的所有文件(readdir()不是线程安全函数)
void listFiles(const char *dirpath);



//测试主函数
int main(void){
	printf("遍历某个目录下的所有文件:\n");
	listFiles(".");
	printf("\n\n");

	//解析目录流DIR*(只保留demo function)
	printf("遍历某个目录下的所有文件-线程安全版本:\n");
	listFiles_r(".");
	//listFiles_r(".","x-");
	printf("\n\n");

	return 0;
}



//遍历某个目录下的所有文件-线程安全版本
void listFiles_r(const char *dirpath){
//void listFiles_r(const char *dirpath, const char *file_prefix){
	DIR *dirp;
	struct dirent *result, entryp;
	char buf[FILEPATH_MAX];
	char buf_old[FILEPATH_MAX];
	char time_buf[64];



	//获取目录流DIR*
	dirp = opendir(dirpath);
	if(dirp == NULL) {
		perror("opendir() fail");
		return;
	}

	//解析目录流DIR*
	for(;;) {
		//不需要反复询问readdir_r() 是否出错
		/*
		errno = readdir_r(dirp, &entryp, &result);
		if(errno != 0){
			perror("readdir_r() fail");
			break;
		}
		*/
		readdir_r(dirp, &entryp, &result);

		//这个询问不能省, 否则程序会陷入死循环, 卡死在第一个索引的文件上, 不知为何
		if(result == NULL)
			break;
		//free(result);//不用释放result

		//跳过目录自身和父目录
		if(strcmp(entryp.d_name, ".") == 0 || strcmp(entryp.d_name, "..") == 0)
			continue;

		//打印目录内的所有信息
		//printf("name: %s, type: %d\n", entryp.d_name, entryp.d_type);

		//判断文件类型: 如果是文件夹, 递归调用
		if(entryp.d_type == DT_DIR){
			memset(buf,'\0',FILEPATH_MAX);
			strncpy(buf, dirpath, FILEPATH_MAX);
			strncat(buf, "/", FILEPATH_MAX);
			strncat(buf, entryp.d_name, FILEPATH_MAX);
			printf("***buf: %s\n", buf);
			listFiles_r(buf);
			//listFiles_r(buf,file_prefix);
		}

		//判断文件类型: 如果是'普通文件/链接文件'
		if(entryp.d_type == DT_REG || entryp.d_type == DT_LNK){
			printf("name: %s, type: %d\n", entryp.d_name, entryp.d_type);
			/*
			//逐个文件'加上固定的前缀'
			memset(buf,'\0',FILEPATH_MAX);
			strncat(buf,dirpath,FILEPATH_MAX);
			strncat(buf, "/", FILEPATH_MAX);

			//加前缀
			strncpy(buf,file_prefix,FILEPATH_MAX);
			strncat(buf,entryp.d_name,FILEPATH_MAX);
			//加日期后缀
			snprintf(time_buf,64,"-%d",time(NULL));
			strncat(buf,time_buf,FILEPATH_MAX);

			//old name
			memset(buf_old,'\0',FILEPATH_MAX);
			strncat(buf_old,dirpath,FILEPATH_MAX);
			strncat(buf_old, "/", FILEPATH_MAX);

			strncat(buf_old,entryp.d_name,FILEPATH_MAX);

			//执行改名
			printf("+++buf_old=%s, buf=%s\n", buf_old, buf);
			rename(buf_old,buf);
			*/
		}
	}

	//关闭目录流DIR*
	closedir(dirp);

	return;
}



//遍历某个目录下的所有文件(readdir()不是线程安全函数)
void listFiles(const char *dirpath){
	DIR *dirp;
	struct dirent *dp;



	//1.获取目录流DIR*
	dirp = opendir(dirpath);
	if(dirp == NULL){
		perror("opendir() fail");
		return;
	}


	//2.解析目录流DIR*(遍历所有文件)
	//	(请谨慎操作!! 万一失败,所有文件可能被重叠成一个文件,等于变相删除,操作前最好先备份数据)
	for(;;){
		dp = readdir(dirp);
		if(dp == NULL)
			break;

		//跳过'父目录'和'文件目录本身'
		if(strncmp(dp->d_name, ".", FILEPATH_MAX) == 0 || strncmp(dp->d_name, "..", FILEPATH_MAX) == 0)
			continue;

		//打印'文件/目录'名 + 类型
		printf("name: %s, type: %d\n", dp->d_name, dp->d_type);
	}


	//3.关闭目录流DIR*
	closedir(dirp);

	return;
}
