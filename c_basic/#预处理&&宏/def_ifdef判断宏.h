//停止g++继续编译, 并打印出错误信息.
#error [love you]
#error love you
#error "love you"





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



