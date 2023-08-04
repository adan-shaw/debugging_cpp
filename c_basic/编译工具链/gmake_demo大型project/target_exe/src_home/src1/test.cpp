//测试1: (测试跨越性调用)
#include "../main_src/test.h"

void printfxx(void){
  fprintf(stderr, "SXXWSSXSS\n");
}





//测试2: (测试静态库函数调用)
extern "C"{
	#include "../lib_static/lib_static1_headfile/lib_static.h"
	#include "../lib_static/lib_static2_headfile/lib_static2.h"
}

extern "C" int lib_static_f1(void);
extern "C" int lib_static_f2(void);

void test_lib_static1(void){
  fprintf(stderr, "lib_static_f1(): %d\n", lib_static_f1());
}
void test_lib_static2(void){
  fprintf(stderr, "lib_static_f2(): %d\n", lib_static_f2());
}





//测试3: (测试动态库函数调用)
#include "../lib_dynamic/lib_dynamic_headfile/lib_dynamic.h"

void test_lib_dynamic(void){
  fprintf(stderr, "lib_dynamic_f1(): %d\n", lib_dynamic_f1());
  fprintf(stderr, "lib_dynamic_f2(): %d\n", lib_dynamic_f2());
}
