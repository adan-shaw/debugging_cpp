//编译(编译时, 需要开启最高版本, 才能检测c++ 编译器支持的最高版本, 否则都是检测默认c++ 版本):

//		g++ -std=c++2a ./cpp_version_checking.cpp -o x

//		clang-14 -std=c++2a ./cpp_version_checking.cpp -o x



//ps: 
//	gcc 8.3.0, clang-14 的默认c++ 编译器版本, 都是c++14;
//	gcc 8.3.0 最高支持c++17          -- #pragma message("C++17")
//	clang-14 最高支持c++20甚至更高!!  -- #pragma message("C++20 or higher")



//为了减少报错的可能性, 一个头文件都不添加, 直接在c++ 编译器中运行, 主打就是检测c++ 编译器的内部属性(c++ 编译器内置宏检测)
//#include <iostream>



#define XSTR(x) STR(x)
#define STR(x) #x

//打印c++ 编译器支持哪个c++ 版本(支持gcc/clang/msvc), 检测c++ 版本包括: c++03, c++11, c++14, c++17, c++20
void print_cpp_std_version(void)
{
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
	#pragma message("C++20 or higher")
#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	#pragma message("C++17")
#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
	#pragma message("C++14")
#elif (__cplusplus >= 201103)
	#pragma message("C++11")
#elif (__cplusplus >= 199711L)
	#pragma message("C++03")
#else
	#pragma message "unknown C++ standard"
#endif
}

int main(void)
{
	//检测宏, 编译时编译器打印检测结果
	print_cpp_std_version();
	return 0;
}
