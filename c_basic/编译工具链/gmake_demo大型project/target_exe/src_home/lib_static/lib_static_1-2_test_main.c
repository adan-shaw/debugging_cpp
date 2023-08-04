//makefile 只会搜索编译*.cpp 文件,
//那么.c 文件后缀, 其实可以用来做local 测试文件.

#include <stdio.h>
#include "../lib_static1_src/lib_static.h"
#include "../lib_static2_src/lib_static2.h"


//编译时, 链接../lib_static.a ../lib_static2.a
// 静默规则写法:
//	g++ -o x ./test_main.c ../lib_static.a ../lib_static2.a
// 明文指定写法:
//	g++ -o x ./test_main.c -L.. -l_static -l_static2

//-L.. 指定gcc 搜索路径, 搜索 '..' 当前文件夹.

//-l_static 指定加载./lib_static.a 库,
//	但是遇到lib 前缀, 省略; 遇到.a 后缀, 省略, 因此剩下字符_static

//-l_static2 指定加载./lib_static2.a 库,
//	但是遇到lib 前缀, 省略; 遇到.a 后缀, 省略, 因此剩下字符_static2

//g++ -c ./test_main.c
//g++ -o x ./test_main.o -L.. -l_static -l_static2
int main(void){
  fprintf(stderr, "lib_static_f1(): %d\n", lib_static_f1());
  fprintf(stderr, "lib_static_f2(): %d\n", lib_static_f2());
  return 0;
}
