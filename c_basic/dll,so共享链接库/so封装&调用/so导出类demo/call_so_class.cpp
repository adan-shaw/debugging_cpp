//编译:
//		g++ -g3 -ldl ./call_so_class.cpp -o x 



#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "call_so_class.h"
#include "make_so_class.h"



//定义函数指针list
typedef struct {
	lib_struct_t* (*func1)(double, int);
	void (*func11)(lib_struct_t*, double, int);
	std::unique_ptr<lib_struct_t> (*func111)(double, int);
	test* (*get_test)(void);
	//test2* (*get_test2)(void);
} API_list;

//so 共享链接库的路径
const char* path_so = "./make_so_class.so";

void init_so_API(void* pso, API_list* papi){
	const char* perr_so = NULL;

	//dlerror();//每次执行dlsym()前, 都要清除一下错误记录器;
	papi->func1 = (lib_struct_t* (*)(double, int))dlsym(pso, "func1");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->func11 = (void (*)(lib_struct_t*, double, int))dlsym(pso, "func11");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->func111 = (std::unique_ptr<lib_struct_t> (*)(double, int))dlsym(pso, "func111");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->get_test = (test* (*)(void))dlsym(pso, "get_test");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	//papi->get_test2 = (test* (*)(void))dlsym(pso, "get_test2");
	//if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	return;
}



int main(void){
	API_list m_api;
	void* p = NULL;
	lib_struct_t m, *ps;
	std::unique_ptr<lib_struct_t> ptr;
	test* ptest;
	//test2* ptest2;

	if(!open_soEx(p, path_so)){
		perror("open_soEx()");
		return -1;
	}

	memset((void*)&m_api,0,sizeof(API_list));
	init_so_API(p, &m_api);

	//使用这种方法时, 需要注意: 使用之后, 别忘了释放内存(谨慎使用)
	ps = m_api.func1(0.0, 5);
	printf("x1=%lf,x2=%d\n",ps->x1,ps->x2);
	free(ps);

	m_api.func11(&m, 0.0, 5);
	printf("x1=%lf,x2=%d\n",m.x1,m.x2);

	ptr = std::move(m_api.func111(0.0, 5));
	printf("x1=%lf,x2=%d\n",ptr->x1,ptr->x2);

	ptest = m_api.get_test();
	if(ptest != NULL)
		ptest->padd();
	else
		printf("get_test() failed\n");

	delete ptest;

	/*
	ptest2 = m_api.get_test2();
	if(ptest2 != NULL)
		ptest2->padd2();
	else
		printf("get_test2() failed\n");

	delete ptest2;
	*/

	close_soEx(p);
	return 0;
}


