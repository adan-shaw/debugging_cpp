//编译:
//		g++ -I /home/tarball/jemalloc/include -DJEMALLOC_NO_DEMANGLE -g3 ./jemalloc_static.cpp -Bstatic -o x 

//运行(不需要使用LD_PROLOAD 进行hijacked 劫持):
//		./x
//		LD_PROLOAD=/home/tarball/jemalloc/lib/libjemalloc.a ./x



#include <stdio.h>
#include <map>
#include "jemalloc/jemalloc.h"

//没有je_new/je_free !! 需要自己强行调用构造函数!!
#define MALLOC je_malloc
#define FREE je_free
#define REALLOC je_realloc

int main(void){
	char* p = (char*)je_malloc(64);
	je_free(p);


	std::map<int,int>* pmap = (std::map<int,int>*)je_malloc(sizeof(std::map<int,int>));//强行申请类内存
	new (pmap) std::map<int,int>;		//调用构造函数placement-new 方法
	pmap->~map();										//调用析构函数
	je_free(pmap);									//释放类内存
	return 0;
}
