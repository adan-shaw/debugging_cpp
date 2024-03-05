//编译:
//		gcc -g3 ./mmap虚拟内存区域锁mlock.c -o x 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>



#define PAGE_COUNT (4096*1)



//创建一块匿名mmap 共享映射
void mmap_mlock(void){
	char *pmap;

	//创建一块mmap 映射内存, 进程共享, 可读写, 
	pmap = mmap(NULL, PAGE_COUNT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		return;
	}

	mlock(pmap, PAGE_COUNT/2);			//把前半页锁在物理内存中(禁止交换到disk 中)

	strncpy(pmap, "hello fuckers", sizeof("hello fuckers"));
	strncpy((pmap+(PAGE_COUNT/2)), "hello fuckers", sizeof("hello fuckers"));

	munlock(pmap, PAGE_COUNT/2);		//解锁

	//打印
	printf("%s\n", pmap);
	printf("%s\n", (pmap+(PAGE_COUNT/2)));

	//释放mmap
	munmap(pmap,PAGE_COUNT);

	return ;
}

//将进程中所有的'虚拟地址空间分页', 全部锁进内存中;(包括当前为程序文本段、数据段、内存映射以及栈分配的所有分页)
#define mlockall_current() (mlockall(MCL_CURRENT))

//将进程后续申请的mmap()/shmat()映射内存, 或是'向上增长的堆,向下增长的栈', 这些分页全部锁进内存中;
//(该操作会占用大量操作系统内存, 但性能提高不明显)
#define mlockall_future() (mlockall(MCL_FUTURE))

//解锁[专门撤销mlockall() 产生的执行结果]
#define mlockall_cancel() (munlockall())



int main(void){
	mmap_mlock();
	mlockall_current();
	mlockall_future();
	mlockall_cancel();
	return 0;
}
