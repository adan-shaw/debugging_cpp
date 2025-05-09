//编译:
//		gcc -g3 ./utime.c -o x 



#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
#include <errno.h>



//1.先提取访问时间, 然后修改访问时间, 企图忽略本次访问时间记录?
void utime_test(void){
	const char *pathname = "utime.c";
	struct stat sb;
	struct utimbuf utb;

	//获取当前的file stat()
	if(stat(pathname, &sb) == -1){
		perror("stat()");
		return ;
	}

	//将当前的file stat()中的访问时间,当成是新的访问时间,写入到file 属性中
	utb.actime = sb.st_atime;				// Leave access time unchanged
	utb.modtime = sb.st_atime;			// Make modify time same as access time
	/*
	if(utime(pathname, &utb) == -1){// Update file times
		perror("utime()");
		return ;
	}
	*/
	//获取文件stat 成功的话, 修改utime 访问时间, 一般不会出错(权限不足也会出错)
	utime(pathname, &utb);

	return;
}



//2.精确到微秒
void utimes_test(void){
	struct stat sb;
	struct timeval tv[2];
	const char *pathname = "utime.c";

	//获取当前的file stat()
	if(stat(pathname, &sb) == -1){
		perror("stat()");
		return ;
	}

	tv[0].tv_sec = sb.st_atime;	// Leave atime seconds unchanged
	tv[0].tv_usec = 223344;			// Change microseconds for atime
	tv[1].tv_sec = sb.st_atime;	// mtime seconds == atime seconds
	tv[1].tv_usec = 667788;			// mtime microseconds

	if(utimes(pathname, tv) == -1){
		perror("utimes()");
		return ;
	}

	return;
}



int main(void){
	utime_test();
	utimes_test();
	return 0;
}




