//编译:
//		gcc -g3 ./stat.c -o x 

/*
	文件状态结构体:
		struct stat {
			dev_t			st_dev;			//ID of device containing file	文件所在设备的ID
			ino_t			st_ino;			//inode number									inode节点号
			mode_t		st_mode;		//Mode of file									文件模式
			nlink_t		st_nlink;		//number of hard links					链向此文件的连接数(硬连接)
			uid_t			st_uid;			//user ID of owner							user id
			gid_t			st_gid;			//group ID of owner							group id
			dev_t			st_rdev;		//device ID 										设备号, 针对设备文件(if file is character or block special)
			off_t			st_size;		//total size, in bytes					文件大小, 字节为单位
			blksize_t	st_blksize;	//blocksize for filesystem I/O	系统块的大小
			blkcnt_t	st_blocks;	//number of blocks allocated		文件所占块数
			time_t		st_atime;		//time of last access						最近访问时间
			time_t		st_mtime;		//time of last modification			最近修改时间
			time_t		st_ctime;		//time of last status change		最近状态变化时间
		};



	1.提供文件名字, 获取文件对应属性. 感觉一般是文件没有打开的时候这样操作
	int stat(const char *restrict pathname, struct stat *restrict buf);

	2.通过文件描述符获取文件对应的属性. 文件打开后这样操作
	int fstat(int filedes, struct stat *buf);

	3.连接文件
	int lstat(const char *restrict pathname, struct stat *restrict buf);
 */



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysmacros.h>



#define FP_SPECIAL (1)


//1.stat()检查文件的stat状态(相当与: stat <file_path>查看文件信息一样)
void stat_check(const char* pfile_name);

//2.file stat info 打印函数(书本上的)
void displayStatInfo(const struct stat *sb);

//3.打印文件权限
char* filePermStr(mode_t perm, int flags, char str[32]);

//4.权限测试(书本上的)
void umask_test(void);



int main(void){
	//const char file_name[260] = "./stat.c";
	const char *file_name = "./stat.c";
	key_t kfd;

	printf("0.ftok() 测试\n");
	kfd = ftok(file_name, 0x57);
	if(kfd == -1){
		perror("ftok()");
		return -1;
	}

	printf("1.stat()检查文件的stat状态(相当与: stat <file_path>查看文件信息一样)\n");
	stat_check(file_name);

	printf("2.file stat info 打印函数(书本上的)\n");
	printf("忽略, displayStatInfo()函数在stat_check()中被引用\n");

	printf("3.打印文件权限[Return ls(1)-style string for file permissions mask]\n");
	printf("忽略, filePermStr()函数在stat_check()中被引用\n");

	printf("4.权限测试(书本上的)\n");
	umask_test();

	return 0;
}



//1.stat()检查文件的stat状态(相当与: stat <file_path>查看文件信息一样)
void stat_check(const char* pfile_name){
	struct stat stat_buf;
	int ret;
	char ptimebuf[64];				//打印修改时间的buf

	struct timespec st_atim;	//Time of last access.文件的最后访问时间
	struct timespec st_mtim;	//Time of last modification.文件的最后内容修改时间
	struct timespec st_ctim;	//Time of last status change.文件的最后属性修改时间

	long int st_size;					//Size of file, in bytes. 文件大小
	long int st_blksize;			//Optimal block size for I/O. 文件系统的块大小
	long int st_nlink;				//Link count. 文件有多少个软/硬链接
	unsigned int st_mode;			//File mode. 文件模式
	unsigned int st_uid;			//User ID of the file's owner.属主id
	unsigned int st_gid;			//Group ID of the file's group.属主组id

	unsigned long int st_rdev;//Device number, if device. 一般很少用
	unsigned long int st_dev;	//Device.
	long int st_ino;					//File serial number.



	ret = stat(pfile_name, &stat_buf);
	if(ret == -1){						//如果stat() 出错, 分析出错原因
		perror("stat()");
		switch(errno){
			case ENOENT:
				printf("File %s not found.\n", pfile_name);
				break;
			case EINVAL:
				printf("Invalid parameter to stat().\n");
				break;
			default:
				printf("Unexpected error in stat().\n");
		}
		return;									//出错总会结束本函数
	}
	else{											//提取file stat() 中的数据
		memcpy(&st_atim,&stat_buf.st_atim,sizeof(st_atim));
		memcpy(&st_mtim,&stat_buf.st_mtim,sizeof(st_mtim));
		memcpy(&st_ctim,&stat_buf.st_ctim,sizeof(st_ctim));

		st_size = stat_buf.st_size;
		st_blksize = stat_buf.st_blksize;
		st_nlink = stat_buf.st_nlink;
		st_mode = stat_buf.st_mode;
		st_uid = stat_buf.st_uid;
		st_gid = stat_buf.st_gid;

		st_rdev = stat_buf.st_rdev;
		st_dev = stat_buf.st_dev;
		st_ino = stat_buf.st_ino;

		//如果stat()没有出错,则打印stat()结果(输出一些统计数据):
		printf("File size 文件大小: %ld\n", stat_buf.st_size);
		printf("文件系统的块大小: %ld\n", stat_buf.st_blksize);
		printf("文件模式: %ld\n", stat_buf.st_mode);
		printf("文件有多少个软/硬链接: %ld\n", stat_buf.st_nlink);
		printf("Drive 驱动标记: %c:\n", stat_buf.st_dev + 'A');
		printf("st_dev: %lx, st_ino: %lx\n",(u_long)stat_buf.st_dev,(u_long)stat_buf.st_ino);
		printf("属主id: %ld\n", stat_buf.st_uid);
		printf("属主组id: %ld\n", stat_buf.st_gid);

		//文件时间-快速转换提取
		/*
		# define st_atime st_atim.tv_sec
		# define st_mtime st_mtim.tv_sec
		# define st_ctime st_ctim.tv_sec
		*/

		//打印-最后内容修改时间
		//ctime_s() 是c11 特有, 线程安全 + 缓冲区安全
		//ctime_s(timebuf, 64, &stat_buf.st_mtime);
		ctime_r(&stat_buf.st_mtime, ptimebuf);//自己保障缓冲区不会溢出
		printf("Time of last modified 最后内容修改时间: %s\n", ptimebuf);

		//打印-最后访问时间
		ctime_r(&stat_buf.st_atime, ptimebuf);
		printf("Time of last access 最后访问时间: %s\n", ptimebuf);

		//打印-最后属性修改时间
		ctime_r(&stat_buf.st_ctime, ptimebuf);
		printf("Time of last status change 最后属性修改时间: %s\n", ptimebuf);


		printf("\n\n");
		//书本上的file stat info 打印函数
		printf("3.书本上的file stat info 打印函数\n");
		displayStatInfo(&stat_buf);
		printf("\n\n");
	}

	return;
}



//2.file stat info 打印函数(书本上的)
void displayStatInfo(const struct stat *sb){
	char str_auth[32];

	printf("File type:                ");
	switch (sb->st_mode & S_IFMT) {
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFLNK:  printf("symbolic (soft) link\n");    break;
		case S_IFIFO:  printf("FIFO or pipe\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown file type?\n");      break;
	}

	printf("Device containing i-node: major=%ld   minor=%ld\n",(long)major(sb->st_dev),(long)minor(sb->st_dev));
	printf("I-node number:            %ld\n", (long) sb->st_ino);
	printf("Mode:                     %lo (%s)\n",(unsigned long)sb->st_mode,filePermStr(sb->st_mode,0,str_auth));

	if(sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
		printf("    special bits set:     %s%s%s\n",
				(sb->st_mode & S_ISUID) ? "set-UID " : "",
				(sb->st_mode & S_ISGID) ? "set-GID " : "",
				(sb->st_mode & S_ISVTX) ? "sticky " : "");

	printf("Number of (hard) links:   %ld\n", (long) sb->st_nlink);
	printf("Ownership:                UID=%ld   GID=%ld\n",(long)sb->st_uid,(long)sb->st_gid);
	if(S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
		printf("Device number (st_rdev):  major=%ld; minor=%ld\n",(long)major(sb->st_rdev),(long)minor(sb->st_rdev));

	printf("File size:                %lld bytes\n", (long long) sb->st_size);
	printf("Optimal I/O block size:   %ld bytes\n", (long) sb->st_blksize);
	printf("512B blocks allocated:    %lld\n", (long long) sb->st_blocks);

	printf("Last file access:         %s", ctime(&sb->st_atime));
	printf("Last file modification:   %s", ctime(&sb->st_mtime));
	printf("Last status change:       %s", ctime(&sb->st_ctime));

	return;
}



//3.打印文件权限[Return ls(1)-style string for file permissions mask]
char* filePermStr(mode_t perm, int flags, char str[32]){
	memset(str,'\0',32);

	snprintf(str, 32, "%c%c%c%c%c%c%c%c%c",
		(perm & S_IRUSR) ? 'r' : '-', 
		(perm & S_IWUSR) ? 'w' : '-',
		(perm & S_IXUSR) ?
			(((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
			(((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
		(perm & S_IRGRP) ? 'r' : '-', 
		(perm & S_IWGRP) ? 'w' : '-',
		(perm & S_IXGRP) ?
			(((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
			(((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
		(perm & S_IROTH) ? 'r' : '-', 
		(perm & S_IWOTH) ? 'w' : '-',
		(perm & S_IXOTH) ?
			(((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
			(((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));

	return str;
}



#define MYFILE "myfile"
#define MYDIR  "mydir"
#define FILE_PERMS    (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS     (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

//4.权限测试(书本上的)
void umask_test(void){
	int fd;
	struct stat sb;
	mode_t u;
	char str_auth[32];

	umask(UMASK_SETTING);

	fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
	if(fd == -1){
		perror("open()");
		return ;
	}

	if(mkdir(MYDIR, DIR_PERMS) == -1){
		perror("mkdir()");
		return ;
	}

	u = umask(0);									//Retrieves (and clears) umask value

	if(stat(MYFILE, &sb) == -1){	//查看文件属性
		perror("stat()");
		return ;
	}

	printf("Requested file perms: %s\n",filePermStr(FILE_PERMS, 0, str_auth));
	printf("Process umask:        %s\n",filePermStr(u, 0, str_auth));
	printf("Actual file perms:    %s\n\n",filePermStr(sb.st_mode, 0, str_auth));

	if(stat(MYDIR, &sb) == -1){		//查看文件夹属性
		perror("stat()");
		return ;
	}

	printf("Requested dir. perms: %s\n",filePermStr(DIR_PERMS, 0, str_auth));
	printf("Process umask:        %s\n",filePermStr(u, 0, str_auth));
	printf("Actual dir. perms:    %s\n",filePermStr(sb.st_mode, 0, str_auth));

	if(unlink(MYFILE) == -1){
		perror("stat()");
		return ;
	}

	if(rmdir(MYDIR) == -1){
		perror("stat()");
		return ;
	}
	return;
}

