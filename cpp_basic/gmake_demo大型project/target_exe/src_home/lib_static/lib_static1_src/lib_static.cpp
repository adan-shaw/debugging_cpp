//编译:
//g++ -c ./headfile/lib_static.h ./lib_static.cpp -fPIC
//ar -crv lib_static.a lib_static.o

//不能添加: ar -S !! 否则会报错:
//./lib_static.a: error adding symbols: Archive has no index; run ranlib to add one

//编译静态库.o 元素的时候, 最好附带.h 头文件, 这样比较好指认.


//编译时, 链接./lib_static.a
//g++ -o x ./main.cpp ./lib_static.a # 静默规则写法
//g++ -o x ./main.cpp -L. -l_static # 明文指定写法

//-L. 指定gcc 搜索路径, 搜索 '.' 当前文件夹.

//-l_static 指定加载./lib_static.a 库,
//	但是遇到lib 前缀, 省略; 遇到.a 后缀, 省略, 因此剩下字符_static



#include "./lib_static.h"

////告诉编译器, 这部分代码按C语言的格式进行编译, 而不是C++的
#ifdef __cplusplus
extern "C"{
#endif
//



int lib_static_f1(void){
  return 1;
}



//
#ifdef __cplusplus
}
#endif
//
