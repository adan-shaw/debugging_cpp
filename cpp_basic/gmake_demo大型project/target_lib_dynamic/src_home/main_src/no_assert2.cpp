//#define NDEBUG
#include <assert.h>
#include <stdio.h>

//O2 优化的时候, no_assert2.cpp 必须包含自己的头文件
//否则找不到函数: void test(void);
#include "../src1/no_assert2.h"


void test(void){
  assert(0);
  fprintf(stderr, "XXXX2222\n");
}

