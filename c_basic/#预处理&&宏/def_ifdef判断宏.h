//
// 常见的ifdef 判断宏
//

//1.创建宏xxx
#define xxx "x"



//2.取消(销毁)宏xxx
#undef xxx



//3.预处理过程中的判断(通常只用来判断平台差异,编译器差异)
#if _WIN32
	//yes, this is windows system
#endif



//4.if 是否存在'某个宏'
#ifdef xxx
	//如果存在xxx宏, 则g++编译器, 编译这里面的代码
#endif

#ifdef __cplusplus
	//如果存在__cplusplus宏, 则g++编译器, 编译这里面的代码
#endif

//判断操作系统类型, 非目标平台, 终止编译
#ifdef _WIN32
	//#error: 报错&停止编译
	#error [ Not Support Windows 32-bit ]
#elif _WIN64
	#error [ Not Support Windows 64-bit ]
#elif __APPLE__ || __MACH__
	#error [ Not Support Mac OSX ]
#elif __linux__
	//#pragma message: 打印编译提示信息
	#pragma message("Support Linux")
#elif __FreeBSD__
	#pragma message("Support FreeBSD")
#elif __unix || __unix__
	#pragma message("Support Unix")
#else
	#error [ System Type Unknow!! ]
#endif



//5.if 如果不存在'某个宏'
#ifndef xxx
	//如果不存在xxx宏, 则定义xxx宏
	#define xxx "x"
#endif



//6.停止g++继续编译, 打印出错误信息.
#error [love you]
#error love you
#error "love you"
