//可以打备注, 跟c/c++ 文件一样, 可以打备注
/*
	实际上, swig 就是一个c/c++ 预处理优化project, 
	最终还是编译出c/c++ 二进制程序的, .exe/.a/.so
	只不过, swig 主要的功能, 是编译出script 脚本机兼容的模块而已.
*/





/*--- File: example.i ---*/
%module example
%{
#include "word.h"
%}



/*
	为什么是std_string.i, 而不是string.h? 
	因为std_string.i 是gcc 编译器预处理之后的名字, 
	被内嵌的.h 头文件, 需要直接包含*.i 而不是*.h ?
*/
%include "std_string.i"

//包含word.h
%include "word.h"
