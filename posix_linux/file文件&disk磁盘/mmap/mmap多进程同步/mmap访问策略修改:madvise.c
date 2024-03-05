//编译:
//		gcc -g3 ./mmap访问策略修改\:madvise.c -o x 



//madvise(): 通知内核对某块mmap 进行访问策略优化, 提高访问效率
/*
	int madvise(void *addr, size_t length, int advice);

	advice 参数:
		MADV_NORMAL:			这是默认行为, 分页是以簇的形式(较小的一个系统分页大小的整数倍)传输的.
											这个值会导致一些预先读和事后读.
		MADV_RANDOM:			这个区域中的分页会被随机访问, 
											这样预先读将不会带来任何好处, 因此内核在每次读取时所取出的数据量应该尽可能少. 
		MADV_SEQUENTIAL:	在这个范围中的分页只会被访问一次, 并且是顺序访问, 
											因此内核可以激进地预先读, 并且分页在被访问之后就可以将其释放了
		MADV_WILLNEED:		预先读取这个区域中的分页以备将来的访问之需. 
											MADV_WILLNEED 操作的效果与Linux特有的readahead()和posix_fadvise() 操作的效果类似
		MADV_DONTNEED:		调用进程不再要求这个区域中的分页驻留在内存中. 
											这个标记的精确效果在不同 UNIX 实现上是不同的. 


		大多数UNIX 实现都提供了一个madvise(), 它们通常至少支持上面描述的 advice 常量. 
		然而SUSv3 使用了一个不同的名称来标准化了这个 API, 即 posix_madvise(), 
		并且在相应的 advice 常量上加上了一个前缀字符串 POSIX_. 
		因此, 这些常量变成了:
			POSIX_MADV_NORMAL				提高预读
			POSIX_MADV_RANDOM				减少预读
			POSIX_MADV_SEQUENTIAL		访问一次,即被释放(疯狂的策略)
			POSIX_MADV_WILLNEED
			POSIX_MADV_DONTNEED
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>



#define PAGE_COUNT (4096*1)



void mmap_madvise(void){
	char *pmap;

	pmap = mmap(NULL, PAGE_COUNT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		return;
	}

	//修改mmap 内存块的访问策略
	posix_madvise(pmap, PAGE_COUNT, POSIX_MADV_NORMAL);

	strncpy(pmap, "hello fuckers", sizeof("hello fuckers"));
	printf("%s\n", pmap);

	//释放mmap
	munmap(pmap,PAGE_COUNT);

	return ;
}

int main(void){
	mmap_madvise();
	return 0;
}
