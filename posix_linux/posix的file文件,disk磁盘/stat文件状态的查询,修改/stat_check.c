//编译:
//		gcc -g3 ./stat_check.c -o x 



#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>



//用stat()检查一下路径是否被占用, 顺便读取文件大小
bool stat_check(char* pfile_name, int* filesize){
	struct stat m_stat;
	int ret;
	char tmpbuf[64];

	ret = stat(pfile_name, &m_stat);
	if(ret == -1){
		perror("stat()");										//如果stat() 出错, 分析出错原因
		switch(errno){
			case ENOENT:
				printf("File %s not found.\n", pfile_name);
				break;
			case EINVAL:
				printf("Invalid parameter to stat().\n");
				break;
			default:
				printf("Unexpected error in stat().\n");
				break;
		}
		return false;
	}
	else{																	//如果stat() 没有出错, 打印stat() 结果
		printf("Output some of the statistics(输出一些统计数据):\n");
		printf("  File size(文件大小): %ld\n", m_stat.st_size);
		printf("  Drive(驱动标记): %c\n", m_stat.st_dev + 'A');
		printf("  st_dev: %lx\n", (u_long)m_stat.st_dev);
		printf("  st_ino: %lx\n", (u_long)m_stat.st_ino);

		//memset(tmpbuf,'\0',sizeof(tmpbuf));
		ctime_r(&m_stat.st_mtime, tmpbuf);	//打印-最近修改时间
		printf("  Time modified 最近修改时间: %s\n", tmpbuf);

		*filesize = m_stat.st_size;					//获取值文件大小
		return true;
	}

	return false;													//未知错误(不可能走到这一步)
}



int main(void){
	int filesize = -1;
	if(!stat_check("./stat_check.c",&filesize))
		return -1;
	else
		return 0;
}
