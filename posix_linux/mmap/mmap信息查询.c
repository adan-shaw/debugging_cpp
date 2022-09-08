//编译:
//		gcc -g3 mmap.h ./mmap信息查询.c -o x 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mmap.h"



#define MMAP_SHARED_FILE "mmap_shared.file"	//mmap 映射绑定的文件名
#define MMAP_SHARED_FILE_SIZE (4096)				//mmap 映射总长



//以'mmap匿名共享'映射为例, 'mmap文件共享'同理;
void mmap_shared_check(void){
	char cmd[32];
	char *pmap;

	//根据指定大小, 创建一块mmap 映射内存, 进程私有, 不可读写, 
	pmap = mmap(NULL, MMAP_SHARED_FILE_SIZE, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		return;
	}

	printf("查询mmap属性(Before)\n");
	CHECK_MMAP_SHARED(cmd);

	//改变mmap 映射属性, 允许读写
	mprotect(pmap, MMAP_SHARED_FILE_SIZE, PROT_READ | PROT_WRITE);

	printf("查询mmap属性(After)\n");
	CHECK_MMAP_SHARED(cmd);

	//释放mmap
	munmap(pmap,MMAP_SHARED_FILE_SIZE);

	return ;
}



int main(void){
	mmap_shared_check();
	return 0;
}
