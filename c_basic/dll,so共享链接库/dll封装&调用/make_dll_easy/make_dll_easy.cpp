//cl编译出dll:
//	cl .\make_dll_easy.h .\make_dll_easy.cpp -link -dll -out:make_dll_easy.dll
//cl编译出exe[-Os 是编译优化选项]:
//	cl .\make_dll_easy.cpp -Os -link -out:make_dll_easy.exe

//windows 重建命令行c++ 编译工具链十分麻烦, 还是直接用qt IDE 最省事, 直接跨平台开发;



//windows 查看.so 文件的API list:
//	dumpbin.exe -exports ./make_dll_easy.dll
//	dumpbin.exe -LINENUMBERS ./make_dll_easy.lib



#include <cstdio>
#include <cstdlib>
#include "make_dll_easy.h"



//dll/so 内部的静态变量
static int call_count = 0;



//返回结构体/类等非常规变量实体时, 只能使用指针返回; (严格执行线程安全函数标准, 这种写法要直接放弃)
//使用这种方法时, 需要注意: 使用之后, 别忘了释放内存(谨慎使用)
__declspec(dllexport) lib_struct_t* func1(double x1, int x2){
	call_count+=1;
	printf("func1(): static value call_count=%d\n", call_count);
	//lib_struct_t m_struct;
	lib_struct_t* pStruct = (lib_struct_t*)malloc(sizeof(lib_struct_t));
	if(!pStruct){
		printf("func1(): malloc() failed!!");
		return NULL;
	}
	else{
		pStruct->x1 = x1 + 1;
		pStruct->x2 = x2 + 1;
		return pStruct;//这样返回, 调用者还要再次释放(除非你使用智能指针)
	}
}
//lib_struct_t* (*func1)(double, int);



//改进写法for old c++: 这样定义和释放都在调用者端, 逻辑非常明确
__declspec(dllexport) void func11(lib_struct_t* pStruct, double x1, int x2){
	call_count+=1;
	printf("func11(): static value call_count=%d\n", call_count);
	pStruct->x1 = x1 + 1;
	pStruct->x2 = x2 + 1;
}
//void (*func11)(lib_struct_t*, double, int);



//智能指针写法(c++11 起步)
__declspec(dllexport) lib_struct_t* func111(std::string s, double x1, int x2){
	lib_struct_t* pStruct = (lib_struct_t*)malloc(sizeof(lib_struct_t));
	if(!pStruct){
		printf("func1(): malloc() failed!!");
		return NULL;
	}
	else{
		pStruct->x1 = x1 + 1;
		pStruct->x2 = x2 + 1;
		printf("func111(): %s, x1=%lf, x2=%d\n", s.c_str(), x1, x2);
		return pStruct;
	}
}
//std::unique_ptr<lib_struct_t> (*func111)(double, int);



