#include <stdio.h>
int static_lib_f2(void){
	printf("static_lib_f2()\n");
  return 2;
}
//g++ -c static_lib2.cpp
//ar cr static_lib2.a static_lib2.o
