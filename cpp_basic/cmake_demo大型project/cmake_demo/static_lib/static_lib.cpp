//编译:
//		g++ -c static_lib.cpp; ar cr lib_static.a static_lib.o

#include <stdio.h>
int static_lib_f1(void){
	printf("static_lib_f1()\n");
	return 1;
}
