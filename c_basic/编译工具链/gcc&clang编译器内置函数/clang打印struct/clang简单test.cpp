//编译:
//		clang-16 -g3 ./clang简单test.cpp -o x


#include <cstdio>



struct testInfo{
	float testA[6];
	int   testB;
	int   testC;
	float testD;
	float testE;
	float testF;
	int   testG;
};



#ifndef __has_builtin       // Optional of course.
#define __has_builtin(x) 0  // Compatibility with non-clang compilers.
#endif


int main(void){
	struct testInfo x;

	#if __has_builtin(__builtin_dump_struct)
	__builtin_dump_struct(&x, &printf);
	#endif

	return 0;
}
