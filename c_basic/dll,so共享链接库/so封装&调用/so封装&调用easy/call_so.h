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



//从已装载的dll/so共享链接库中, 提取API 函数指针(该API 函数指针在定义时, 必须有dll/so 的public 公开前缀, 这是封装dll/so API时必要的关键字前缀):
/*
	由于dlsym() 返回一根函数指针, 所以大多数情况下, 每次dlsym() 调用, 都会得到一根不一样的函数指针,
	因此装载选择性-动态装载共享链接库时, 在初始化dll/so共享链接库的环节, 最好定义一个结构体, 来装载所有需要用到的函数指针;
	这样一遍过, 每个函数指针执行一次dlsym(), 完成API 函数指针struct 结构体的填充;
	ps:
		dlsym() 返回的API 函数指针, 实际表示: 该函数在dll/so 共享链接库中的地址而已;
*/



//获取dll/so 共享链接库的出错原因:
/*
	dlerror() 是错误记录器, 存放在dll/so 共享链接库中;

	dlclose()/dlopen() 可以使用perror(), errno 可以报错;
	dlsym() 报错则一般从dlerror() 中获取具体原因;

	char *dlerror(void);
	而且使用dlerror() 时要注意:
		每次调用dlsym() 时, 都要先清除一下错误记录器;(这个用法待定)
*/
