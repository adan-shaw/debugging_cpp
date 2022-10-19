//编译:
//		g++ -I /home/tarball/jemalloc/include -L /home/tarball/jemalloc/lib ./jemalloc_dynamic.cpp -ljemalloc -Bdynamic -g3 -o x

//运行(需要使用LD_PROLOAD 进行hijacked 劫持, 最简单):
//		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/jemalloc/lib" LD_PROLOAD=/home/tarball/jemalloc/lib/libjemalloc.so ./x

//最复杂的hijacked 进程运行环境重置:
/*
	LD_PROLOAD="$LD_PROLOAD:/home/tarball/jemalloc/lib/libjemalloc.so" \
	PATH="$PATH:/home/tarball/jemalloc/bin" \
	LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/jemalloc/lib" \
	C_INCLUDE_PATH="$C_INCLUDE_PATH:/home/tarball/jemalloc/include" \
	CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/home/tarball/jemalloc/include" ./x
*/

#include <stdio.h>
#include <map>
#include "jemalloc/jemalloc.h"

int main(void){
	char* p = (char*)malloc(64);
	std::map<int,int>* pmap = new(std::map<int,int>);
	free(p);
	delete(pmap);
	return 0;
}

