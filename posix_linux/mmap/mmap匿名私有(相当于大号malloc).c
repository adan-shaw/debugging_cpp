//编译:
//		gcc -g3 ./mmap匿名私有\(相当于大号malloc\).c -o x 


//说明: malloc() 在申请大块内存的时候, 超过ptmalloc 内存管理器的勘值, 就会用mmap 分配内存;


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>



#define PAGE_COUNT (4096*1)



//创建一块匿名mmap 私有映射(MAP_PRIVATE + MAP_ANONYMOUS, 相当于大号malloc, 很少会用到)
void mmap_noname_private(void){
	char *pmap;



	//根据指定大小, 创建一块mmap 映射内存, 进程私有, 不可读写, 
	pmap = mmap(NULL, PAGE_COUNT, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		return;
	}

	//改变mmap 映射属性, 允许读写
	mprotect(pmap, PAGE_COUNT, PROT_READ | PROT_WRITE);

	strncpy(pmap, "hello fuckers", sizeof("hello fuckers"));
	printf("%s\n", pmap);

	//释放mmap
	munmap(pmap,PAGE_COUNT);

	return ;
}




int main(void){
	mmap_noname_private();
	return 0;
}
