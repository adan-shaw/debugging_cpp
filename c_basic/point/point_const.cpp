//编译:
//		g++ -g3 ./point_const.cpp -o x



//const常量指针:
/*
	//常量字符串指针
	const char* pconst_char = "常量字符串指针,存放在.bss区,全局可访问,相当于静态变量;指针sizeof()=4/8字节";

	//不可改变类型, 可改变值(可后移), 适用于遍历访问值
	const char* p;

	//可改变类型, 不可改变值(不可后移), 很少用
	char* const p;

	//不可改变类型, 不可改变值(错误语法)
	const char* const p;


	ps:
		一般不用在'函数指针'上面, 只用在变量上面;
*/



#include<iostream>
#include"point_test_body.h"



//不可改变类型, 可改变值
void test1(void){
	//一维数组-指针
	const int* pi = arr_i;
	const char* pc = arr_c;

	//可后移
	pc++;

	//二维数组-行指针
	const double (*pd)[32] = arr2_d;
	const float (*pf)[32] = arr2_f;

	//结构体-指针
	const struct body* pbody = &m_body;

	//类-指针
	const class father* pfather = &m_father;



	//函数-指针(少用)
	int (*pFunc)(void) = &ret_int;
	const int (*pFunc2)(void) = &ret_const_int;

	return;
}


//不可改变值, 可改变类型
void test2(void){
	//一维数组-指针
	int* const pi = arr_i;
	char* const pc = arr_c;

	//不可后移
	//pc++;

	//二维数组-行指针
	double const (*pd)[32] = arr2_d;
	float const (*pf)[32] = arr2_f;

	//结构体-指针
	struct body* const pbody = &m_body;

	//类-指针
	class father* const pfather = &m_father;

	return;
}


//常量字符串指针
const char* pconst_char = "常量字符串指针,存放在.bss区,全局可访问,相当于静态变量;指针sizeof()=4/8字节";


int main(void){
	std::cout<<"不可改变类型, 可改变值: okay"<<std::endl;
	test1();

	std::cout<<"不可改变值, 可改变类型: okay"<<std::endl;
	test2();

	std::cout<<pconst_char<<std::endl;

	return 0;
}
