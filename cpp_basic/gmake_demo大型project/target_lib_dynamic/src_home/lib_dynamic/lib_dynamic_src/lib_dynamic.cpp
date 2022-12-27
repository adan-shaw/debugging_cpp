//编译: (gcc 也可以-v 显示详细的编译信息)
//g++ -c ./headfile/lib_dynamic.h ./lib_dynamic.cpp -fPIC
//g++ -shared -o lib_dynamic.so lib_dynamic.o
//
//快捷版本:
//g++ -shared -fPIC -o lib_dynamic.so lib_dynamic.cpp

//编译时, 链接./lib_dynamic.a
//g++ -o x ./main.cpp ./lib_dynamic.a # 静默规则写法
//g++ -o x ./main.cpp -L. -l_dynamic # 明文指定写法

//-L. 指定gcc 搜索路径, 搜索 '.' 当前文件夹.

//-l_dynamic 指定加载./lib_dynamic.a 库,
//	但是遇到lib 前缀, 省略; 遇到.a 后缀, 省略, 因此剩下字符_dynamic



#include "./headfile/lib_dynamic.h"

//
#ifdef __cplusplus
extern "C"{
#endif
//



int lib_dynamic_f1(void){
  return 11;
}

int lib_dynamic_f2(void){
  return 22;
}



//
#ifdef __cplusplus
}
#endif
//
