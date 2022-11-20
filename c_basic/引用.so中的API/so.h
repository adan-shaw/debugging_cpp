#include <dlfcn.h>



//编译选项: -ldl
//	这样初始化, 不需要额外添加编译选项, 而是直接抓住.so 共享库, 装载进本进程, 留做自己使用;
//	所以, 这种情况下, 不需要添加编译选项(可能性能开销比较大, 但相对安全很多)



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
//打开共享链接库(装载so 库)(成功返回一根内存指针, 出错返回NULL)
#define open_soEx(path_so) ( dlopen(path_so, RTLD_LAZY) )



//关闭共享链接库(释放so 库)
#define close_soEx(pso) { if(dlclose(pso) != 0){ perror("dlclose()"); } }



//从已装载的so 库中, 提取API(init .so共享链接库):
/*
	由于dlsym() 函数返回的指针, 具体每个函数的类型都不一样, 具体需要每一个API都详细声明;
	dlsym() 函数只返回一根指针, 实际代表该函数在.so 共享链接库中的地址而已;

	因此, 详细的初始化工作, 每个.so 库都不一样, 这里不再详述;
*/



//获取.so 共享链接库的出错原因(少用, 可以用perror() 代替):
/*
	char *dlerror(void);
	dlerror() 的错误记录器, 存放在.so 共享链接库中, 是装载so 库之后, 才会使用的;
	一般用来报错dlsym() 函数执行的错误;
	因此每次操作.so 共享链接库前, 都要清除一下错误记录器;

	dlclose()/dlopen(), 是linux 系统层的API, 使用perror() 即可;
*/
