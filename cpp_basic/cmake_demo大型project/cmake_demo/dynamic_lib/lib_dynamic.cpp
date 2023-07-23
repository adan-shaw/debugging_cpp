//编译:
//		g++ -c ./headfile/lib_dynamic.h ./lib_dynamic.cpp -fPIC; g++ -shared -o lib_dynamic.so lib_dynamic.o

//快捷版本:
//		g++ -shared -fPIC -o lib_dynamic.so lib_dynamic.cpp

//编译时, 链接./lib_dynamic.a
//		g++ -o x ./main.cpp ./lib_dynamic.a # 静默规则写法
//		g++ -o x ./main.cpp -L. -l_dynamic  # 明文指定写法



#include "./headfile/lib_dynamic.h"
#include <stdio.h>


int lib_dynamic_f1(void){
	printf("lib_dynamic_f1()\n");
	return 11;
}

int lib_dynamic_f2(void){
	printf("lib_dynamic_f2()\n");
	return 22;
}
