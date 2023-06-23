//编译:
//		g++ -g3 ./extern_head.h ./extern_func.cpp ./extern_main.cpp -o x 



//extern 关键字的作用:
/*
	引用&声明extern 外部的:
		* 外部全局变量(包括: 普通变量, 结构体实体, 类实体... 等等)
		* 外部函数API(ps: c语言所有函数都是全局的, 相当于引用一根全局函数指针变量)

	extern 不能引用:
		* const 常量
		* static 变量
		* 宏

	extern 引用声明只能作用于'已创建后的实体变量', 不能自己创建实体;
	因此, extern 可以引用全局函数, 
	但extern 不能引用class 定义, struct 定义;
	(引用class 实体, struct 实体, 可以)
*/



#include <stdio.h>
#include "extern_head.h"



//定义测试用的全局变量实体(不能引用static 定义的变量)
//static enum DAY e1;
enum DAY e1;
union uu u1;
struct x ex_1;
class c ex_2;



//extern 引用外部函数
extern void ttt(void);



int main() {
	ttt();
	printf("%d-%d\n",ex_1.x,ex_2.d);
	printf("!!!Hello World!!!");
	return 0;
}
