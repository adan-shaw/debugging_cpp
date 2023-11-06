#include <windows.h>

//选择性-动态装载共享链接库, 成功返回一根内存指针HMODULE hMod, 出错返回nullptr; (win 不能设置共享链接库打开模式)
//宽字符版本
#define openw_soEx(hMod, wchar_t_path) {if((hMod = LoadLibraryW(wchar_t_path)) == nullptr){printf("LoadLibrary(): load %ls failed!!\n",wchar_t_path);}}
//窄字符版本
#define opena_soEx(hMod, path) {if((hMod = LoadLibraryA(path)) == nullptr){printf("LoadLibrary(): load %s failed!!\n",path);}}

//关闭已经打开的共享链接库(传入fd 句柄)
/*
	注意:
		调用dlclose()/FreeLibrary(), 只是会减少dll/so 共享链接库在系统中的引用计数;
		具体dll/so 共享链接库是否释放, 要看dll/so 共享链接库在系统中的引用计数是否为0, 为0 则自动释放, 不为0 则继续使用;
*/
#define close_soEx(hMod) {if(FreeLibrary(hMod)==0){printf("FreeLibrary() failed!!");}}



