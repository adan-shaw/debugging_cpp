//cl编译出dll:
//	cl .\make_dll_easy.h .\make_dll_easy.cpp -link -dll -out:make_dll_easy.dll
//cl编译出exe[-Os 是编译优化选项]:
//	cl .\make_dll_easy.cpp -Os -link -out:make_dll_easy.exe

//windows 重建命令行c++ 编译工具链十分麻烦, 还是直接用qt IDE 最省事, 直接跨平台开发;



//windows 查看.so 文件的API list:
//	dumpbin.exe -exports ./make_dll_easy.dll
//	dumpbin.exe -LINENUMBERS ./make_dll_easy.a



#include "call_dll.h"
#include "../make_dll_easy/make_dll_easy.h"

//windows 平台必须把函数指针typedef 重定义, 才能正确调用, 很滑稽!! (应该是编译器没办法翻译的问题, 需要重定义告诉编译器, 这是一个函数指针)
//windows 平台定义函数指针的方式, 不一样!! 必须使用CALLBACK* 指针做前缀, 而且函数名不能出现任何小写, 必须全部大写;
//windows 平台下的函数指针, 统一都叫: CALLBACK 函数
typedef lib_struct_t* (CALLBACK* FUNC1)(double, int);
typedef void (CALLBACK* FUNC11)(lib_struct_t*, double, int);
typedef lib_struct_t* (CALLBACK* FUNC111)(std::string s, double, int);



//定义函数指针list
typedef struct {
	FUNC1 func1;
	FUNC11 func11;
	FUNC111 func111;
} API_list;

//dll 共享链接库的路径
const wchar_t* pathw_dll = L"make_dll_easy.dll";
const char* patha_dll = "make_dll_easy.dll";

bool init_dll_API(HMODULE* hMod, API_list* pdll){
	if((pdll->func1 = (FUNC1)::GetProcAddress(*hMod, "func1")) == nullptr){printf("GetProcAddress(): %ld\n",GetLastError()); return false;}
	if((pdll->func11 = (FUNC11)::GetProcAddress(*hMod, "func11")) == nullptr){printf("GetProcAddress(): %ld\n",GetLastError()); return false;}
	if((pdll->func111 = (FUNC111)::GetProcAddress(*hMod, "func111")) == nullptr){printf("GetProcAddress(): %ld\n",GetLastError()); return false;}
	return true;
}



int main(void){
	API_list m_api;
	HMODULE hMod;
	lib_struct_t m, *ps;
	std::string s = "hello world";

	//openw_soEx(hMod, pathw_dll);//宽字符版本
	opena_soEx(hMod, patha_dll);//窄字符版本
	if(!init_dll_API(&hMod, &m_api)){
		printf("init_dll_API() failed");
		return -1;
	}
	else{
		//使用这种方法时, 需要注意: 使用之后, 别忘了释放内存(谨慎使用)
		ps = m_api.func1(0.0, 5);
		printf("x1=%lf,x2=%d\n",ps->x1,ps->x2);
		free(ps);

		m_api.func11(&m, 0.0, 5);
		printf("x1=%lf,x2=%d\n",m.x1,m.x2);

		ps = m_api.func111(s, 0.0, 5);
		printf("x1=%lf,x2=%d\n",ps->x1,ps->x2);
		free(ps);

		close_soEx(hMod);
		return 0;
	}
}

