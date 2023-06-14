//编译:
//		gcc ./disk_size_check检查进程所在磁盘的容量.c -g3 -o x 



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/statfs.h>
#include <limits.h>



//这种系统文件路径, 一般不会超过256 个字符串.
#define buf_max (256)



// disk_size_check检查进程所在的磁盘容量
int main(void){
	//string buffer for .exe 所在的绝对路径, 还有dirname() 路径(basename() = .exe自己的名)
	char path_buf_whole[buf_max], path_buf[buf_max];

	struct statfs diskInfo;
	unsigned long long blocksize, totalsize, freeDisk, availableDisk;



	if(readlink("/proc/self/exe", path_buf_whole, buf_max) == -1){
		perror("fuck");
		return -1;
	}
	// 得到.exe 所在的dirname() 路径
	strncpy(path_buf, dirname(path_buf_whole), buf_max);


	// 根据'得到.exe 所在的dirname() 路径', 获取磁盘info
	if(statfs(path_buf, &diskInfo) == -1){
		perror("fuck");
		return -1;
	}

	// 计算容量
	blocksize = diskInfo.f_bsize;									//每个block里包含的字节数
	totalsize = blocksize * diskInfo.f_blocks; 		//总的字节数
	freeDisk = diskInfo.f_bfree * blocksize;			//剩余空间的大小
	availableDisk = diskInfo.f_bavail * blocksize;//可用空间大小


	printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n", \
		totalsize, totalsize>>10, totalsize>>20, totalsize>>30);

	printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n", \
		freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30);

	return 0;

}
