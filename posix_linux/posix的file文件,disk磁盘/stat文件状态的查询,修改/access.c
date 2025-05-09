//编译:
//		gcc -g3 ./access.c -o x

#include <stdio.h>
#include <unistd.h>

//access() 简介
/*
	//可是文件的访问权限(跟 ls -l 十分类似, 属主默认拥有 -rwxr-xr-x 权限)
	int access(const char *path, int amode);
			path:  文件路径
			amode: R_OK(可读), W_OK(可写), X_OK(可执行), F_OK(是否存在)
			成功返回0, 失败返回-1;
	int faccessat(int fd, const char *path, int amode, int flag);
*/

#define FILE_PATH "./x"

int main(void){
	if(access(FILE_PATH, F_OK) == -1){
		perror("access()");
		return -1;
	}
	if(access(FILE_PATH, R_OK) == -1){
		perror("access()");
		return -1;
	}
	if(access(FILE_PATH, X_OK) == -1){
		perror("access()");
		return -1;
	}
	if(access(FILE_PATH, W_OK) == -1){
		perror("access()");
		return -1;
	}
	if(access(FILE_PATH, R_OK|W_OK|X_OK|F_OK) == -1){
		perror("access()");
		return -1;
	}
	return 0;
}
