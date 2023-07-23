#include <stdio.h>
int static_lib_f1(void){
	printf("static_lib_f1()\n");
  return 1;
}
//g++ -c static_lib.cpp
//ar cr static_lib.a static_lib.o
