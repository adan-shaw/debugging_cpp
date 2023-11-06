#include <dlfcn.h>



//编译选项: -ldl
//	这样初始化, 不需要额外添加编译选项, 而是直接找到.so 共享库, 装载到系统中, count 计数+1;
//	(如果这个dll/so 已经被装载, 即count 计数+1)



//.so共享链接库的模式
/*
	RTLD_LAZY(最常用)
	RTLD_NOW
	RTLD_GLOBAL
	RTLD_LOCAL
	RTLD_NODELETE (since glibc 2.2)
	RTLD_NOLOAD (since glibc 2.2)
	RTLD_DEEPBIND (since glibc 2.3.4)
*/
#define open_soEx_mode RTLD_LAZY

//选择性-动态装载共享链接库, 成功返回:true+一根内存指针HMODULE hMod/void*, 出错返回:false+nullptr/NULL; (win 不能设置共享链接库打开模式)
#define open_soEx(pvoid, path_so) (((pvoid=dlopen(path_so, open_soEx_mode)) == NULL) ? 0 : 1)

//关闭已经打开的共享链接库(传入fd 句柄)
/*
	注意:
		调用dlclose()/FreeLibrary(), 只是会减少dll/so 共享链接库在系统中的引用计数;
		具体dll/so 共享链接库是否释放, 要看dll/so 共享链接库在系统中的引用计数是否为0, 为0 则自动释放, 不为0 则继续使用;
*/
#define close_soEx(pvoid) {if(dlclose(pvoid) != 0){perror("dlclose()");}}

