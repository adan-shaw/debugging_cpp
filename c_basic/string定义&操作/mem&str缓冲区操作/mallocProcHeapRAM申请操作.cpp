//编译:
//		g++ -g3 ./mallocProcHeapRAM申请操作.cpp -o x



//1.申请常规小粒'进程堆'空间(32bit=8字节一粒, 64bit=16字节一粒)
/*
	//申请'进程堆'空间, 返回void*指针(不会自动初始化申请到的内存空间)
	void *malloc(size_t size);

	//申请'进程堆'空间, 返回void*指针(自动初始化申请到的内存空间,全'0'初始化,但需指出元素粒度)
	//总分配量 = 元素个数 * 单个元素size
	void *calloc(size_t nmemb, size_t size);

	//对指针指向的'进程堆'空间, 进行重新分配.
	//(指针void *ptr必须是有效的指针,野指针,一使用就会内存溢出的)
	void *realloc(void *ptr, size_t size);

	//释放'malloc() / calloc() / realloc()'申请的void* 指针, 指向的内存空间.
	void free(void *ptr);
*/



//2.申请自定义粒度的'进程堆'空间(这些函数在头文件"stdlib.h"中声明)
/*
	#define _XOPEN_SOURCE 600
	#define _GNU_SOURCE

	//getpagesize() 返回一分页的大小, 单位为字节(byte)
	size_t getpagesize(void);

	//申请对齐内存
	int posix_memalign(void **memptr, size_t alignment, size_t size);
	//参数说明:
		void **memptr = 返回指针
		size_t alignment = 申请的'进程堆'空间的粒度(内存粒间隔的大小)
		size_t size = 总的内存申请量.

	//释放对齐内存, 同样用free()

	//弃用, freebsd 不能用
	void* memalign(size_t boundary, size_t size);

	//同样弃用, valloc()也是基于memalign()实现的
	void* valloc(size_t size);
	//它的实现如下所示:
		void* valloc (size_t size){
			return memalign(getpagesize(), size);
		}
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h> // for getpagesize()
#include <string.h> // for memset()



//测试'堆内存'分配类api
void malloc_RAM(void){
	unsigned int uiSize = 256;
	int iTmp;
	void *pTmp;


	//1.申请进程堆内存
	pTmp = malloc(uiSize);
	//pTmp = malloc(256);
	if(pTmp == NULL){
		printf("malloc() failed");
		return;
	}
	free(pTmp);



	//2.特殊用法: malloc(0);
	/*
		要么返回NULL,
		要么返回一小块'已被free()掉的内存'(ptmalloc重用的内存块)
		但不能用这块内存, 
		因为你根本不知道这块内存的真实大小, 乱用会造成'内存读写溢出'
		这种方法一般只是用来检查: 系统还有没有内存了.
	*/
	pTmp = malloc(0);
	if(pTmp == NULL){
		printf("malloc() failed(操作系统已经没有空余内存可供分配了)");
		return;
	}
	//这里不用释放,否则下一句realloc()就会引用一根野指针,会内存溢出的
	//free(pTmp);



	//3.重新对'指针指向的内存块'进行分配
	pTmp = realloc(pTmp, 1024);
	if(pTmp == NULL){
		printf("realloc() failed");
		return;
	}
	free(pTmp);



	//4.申请进程堆内存(自动全'0'初始化,但需指出元素粒度)
	pTmp = calloc(1024, sizeof(unsigned int));
	if(pTmp == NULL){
		printf("calloc() failed");
		return;
	}
	free(pTmp);



	//5.按照系统页大小的粒度, 分配大块'堆内存块'
	/*
		malloc() 粒度小, 8byte 一节, 容易被ptmalloc 拆分, 适合存放变量;
		这种大粒内存, 适合存放一些html,txt 标本数据, 也可以用来存放socket buf;
		posix_memalign() returns zero on success, or one of the error values;
		listed in the next section on failure;
	*/
	uiSize = getpagesize();
	pTmp = NULL;
	iTmp = posix_memalign(&pTmp, uiSize, uiSize*4);
	if(iTmp != 0){
		printf("posix_memalign() failed");
		return;
	}
	printf("getpagesize() = %d\n", uiSize);
	free(pTmp);



	return;
}



int main(void){
	malloc_RAM();
	printf("malloc_RAM() okay\n\n");

	return 0;
}








