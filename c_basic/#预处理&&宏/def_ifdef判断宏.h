//停止g++继续编译, 并打印出错误信息.
#error [love you]
#error love you
#error "love you"





/*
#define           定义一个预处理宏
#undef            取消宏的定义

#if               编译预处理中的条件命令, 相当于C语法中的if语句(与c 语言的if 不同的是, 判断条件不能加() 括号)[效果不理想, 慎用, 可能会无效, 很无语的选项, 有些编译器不支持]
#ifdef            判断某个宏是否被定义, 不能或运算, 若已定义, 执行随后的语句
#ifndef           与#ifdef相反, 判断某个宏是否未被定义, 不能或运算
#elif             若#if, #ifdef, #ifndef或前面的#elif条件不满足, 则执行#elif之后的语句, 相当于C语法中的else-if
#else             与#if, #ifdef, #ifndef对应, 若这些条件不满足, 则执行#else之后的语句, 相当于C语法中的else
#endif            #if, #ifdef, #ifndef这些条件命令的结束标志.

defined         　与#if, #elif配合使用, 判断某个宏是否被定义
*/





//
// if 判断宏的常见格式
//
//创建宏xxx
#define xxx "x"

//取消(销毁)宏xxx
#undef xxx

//if 是否存在'某个宏'
#ifdef xxx
	//如果存在xxx宏, 则g++编译器, 编译这里面的代码
#endif

//if 如果不存在'某个宏'
#ifndef xxx
	//如果不存在xxx宏, 则创建xxx宏
	#define xxx "x"
#endif

//判断宏(可惜只能判断number 数字, 不能判断string, 而lua version 记录是"5","1","5" 字符串)
#define LUA_VERSION_MAJOR_EX 5
#define LUA_VERSION_MAJOR_EX 1

#if LUA_VERSION_MAJOR == 5 && LUA_VERSION_MINOR == 1
	#error [lua version 5.1]
#endif





//
// 常见的判断宏
//

//如果存在__cplusplus宏, 则支持c++ 语法
#ifdef __cplusplus
	#error [ Not Support c++ ]
#else
	#pragma message("Support c++")
#endif

//判断编译器是否支持c11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support C11 stdatomic.h ]
#else
	#pragma message("Support C11 stdatomic.h")
	#include <stdatomic.h>
#endif

//判断编译器是否支持c11 线程库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support C11 threads.h ]
#else
	#pragma message("Support C11 threads.h")
	#include <threads.h>
#endif



//判断操作系统类型, 非目标平台, 终止编译
#ifdef _WIN32
	#error [ Not Support Windows 32-bit ]
#elif _WIN64
	#error [ Not Support Windows 64-bit ]
#elif __APPLE__ || __MACH__
	#pragma message("Support Mac OSX")
#elif __linux__
	#pragma message("Support Linux")
#elif __FreeBSD__
	#pragma message("Support FreeBSD")
#elif __unix || __unix__
	#pragma message("Support Unix")
#else
	#error [ Unknow System Type !! ]
#endif



//判断操作系统类型, 非目标平台, 终止编译(判断宏第二种写法, 对付多个判断宏非常有效)
#if defined(_WIN32) || defined(_WIN64)
	#pragma message("Support Windows")
	#include <windows.h>
	//Sleep() 函数改名为: sleep(), 对齐unix
	#define sleep Sleep
#elif defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__) || defined(__unix) || defined(__unix__)
	#pragma message("Support Unix")
	#include <unistd.h>
#else
	#error [ Unknow System Type !! ]
#endif



