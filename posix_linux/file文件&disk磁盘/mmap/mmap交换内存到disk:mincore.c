//编译:
//		gcc -g3 ./mmap交换内存到disk\:mincore.c -o x 



//mincore(): 将指向的mmap 中, 没有被mlock()/mlockall()锁住的分页, 全部交换到disk 中
/*
	mincore() 只能交换一个整页, 不能交换半页;(每次交换一页)
	mlock()   可以锁住半页, 任意长度的页内存;

	实际上, 只要被mincore() '第三个参数指针'指向的mmap 内存块, 肯定会被交换到disk 中;
	无论是否已经调用mlock() 锁住, 都会被交换;

	mincore() 并不会智能跳位, '第三个参数指针'指向的mmap 内存块中, mlock() 和普通mmap 内存区分开;
	把被mlock() 锁住的, 留下来, 再自动拼接起来?
	(不拼接, 会像数组一样, 出现空位数组, 所以没有那么智能, '第三个参数指针'指向的mmap 内存块, 全部交换到disk 中)
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>



#define PAGE_COUNT (4096*16)



//创建一块匿名mmap 共享映射
void mmap_mincore(void){
	char *pmap;

	//创建一块mmap 映射内存, 进程共享, 可读写, 
	pmap = mmap(NULL, PAGE_COUNT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		return;
	}

	mlock(pmap, PAGE_COUNT/16);											//把第一页锁在物理内存中(禁止交换到disk 中)

	strncpy(pmap, "hello lovers", sizeof("hello lovers"));
	strncpy((pmap+(PAGE_COUNT/16/2)), "hello lovers", sizeof("hello lovers"));

	mincore(pmap, PAGE_COUNT, pmap+(PAGE_COUNT/16));//除了第一页, 后面的15 页全部被交换到disk 中了

	munlock(pmap, PAGE_COUNT/16);										//解锁

	//打印
	printf("%s\n", pmap);
	printf("%s\n", (pmap+(PAGE_COUNT/16/2)));

	//释放mmap
	munmap(pmap,PAGE_COUNT);

	return ;
}



int main(void){
	mmap_mincore();
	return 0;
}
