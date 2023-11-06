//编译:
//	g++ -fPIC -shared -fvisibility=hidden ./make_so_easy.h ./make_so_easy.cpp -o make_so_easy.so

//linux 查看.so 文件的API list:
//	nm -D ./make_so_easy.so
//	ar -t ./make_so_easy.a



#include <cstdio>
#include <cstdlib>
#include <memory>
#include "make_so_easy.h"



//dll/so 内部的静态变量
static int call_count = 0;



//返回结构体/类等非常规变量实体时, 只能使用指针返回; (严格执行线程安全函数标准, 这种写法要直接放弃)
//使用这种方法时, 需要注意: 使用之后, 别忘了释放内存(谨慎使用)
__attribute__ ((visibility("default"))) lib_struct_t* func1(double x1, int x2){
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
__attribute__ ((visibility("default"))) void func11(lib_struct_t* pStruct, double x1, int x2){
	call_count+=1;
	printf("func11(): static value call_count=%d\n", call_count);
	pStruct->x1 = x1 + 1;
	pStruct->x2 = x2 + 1;
}
//void (*func11)(lib_struct_t*, double, int);



//智能指针写法(c++11 起步) [既然使用智能指针, 就拒绝使用裸指针; unique_ptr不允许复制, 但可以用std::move()进行转移, 转移即相当于mv, 而不是cp]
__attribute__ ((visibility("default"))) std::unique_ptr<lib_struct_t> func111(double x1, int x2){
	std::unique_ptr<lib_struct_t> p(new lib_struct_t);
	p->x1 = x1 + 1;
	p->x2 = x2 + 1;
	call_count+=1;
	printf("func111(): static value call_count=%d\n", call_count);
	return p;
}
//std::unique_ptr<lib_struct_t> (*func111)(double, int);



