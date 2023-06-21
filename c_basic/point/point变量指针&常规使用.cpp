//编译:
//		g++ -g3 ./point变量指针\&常规使用.cpp -o x



#include<iostream>
#include<cstring>
#include"point_test_body.h"



//
//常规变量指针的定义
//
//一维数组-指针
int* pi = arr_i;
char* pc = arr_c;

//二维数组-行指针
double (*pd)[32] = arr2_d;
float (*pf)[32] = arr2_f;

//结构体-指针
struct body* pbody = &m_body;

//类-指针
class father* pfather = &m_father;

//函数-指针(不可以位移)
int (*pFunc)(void) = &ret_int;
const int (*pFunc2)(void) = &ret_const_int;





//
//常规变量指针的使用
//
//一维数组-指针
void test1(void){
	int tmp;
	int* pi = arr_i;
	char* pc = arr_c;

	//遍历-指针赋值
	for(tmp=0;tmp<64;tmp++)
		pi[tmp] = tmp;

	//遍历-指针访问值
	for(tmp=0;tmp<64;tmp++)
		std::cout << pi[tmp] << std::endl;

	//遍历-指针赋值2
	for(tmp=0;tmp<64;tmp++){
		*(pi+tmp) = tmp;
		//*(pi++) = tmp;//错误指针引用, 不能*(pi++)这样用; 只能p++后移, 引用时不能用p++
	}

	//遍历-指针访问值2
	for(tmp=0;tmp<64;tmp++){
		std::cout << *(pi) << std::endl;
		pi++;
	}

	//字符串批量处理
	strncpy(pc, "fuck you bitch", 64);
	std::cout << pc << std::endl;

	return;
}



//二维数组-行指针
void test2(void){
	int tmp,row;
	double (*pd)[32] = arr2_d;
	float (*pf)[32] = arr2_f;

	//遍历-指针赋值
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			pd[row][tmp] = tmp;

	//遍历-指针访问值
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			std::cout << pd[row][tmp] << std::endl;

	//遍历-指针赋值2
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			(*pd+row)[tmp] = tmp;//错误指针引用, 不能(*pd++)[tmp]这样用; 只能p++后移, 引用时不能用p++

	//遍历-指针访问值2
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			std::cout << (*pd+row)[tmp] << std::endl;

	//float 行指针同理, 忽略
	return;
}



//结构体-指针
void test3(void){
	struct body* pbody = &m_body;
	//赋值
	pbody->head = 111;
	pbody->hand = 111;
	pbody->penis = 111;
	//访问
	std::cout << pbody->head << std::endl;
	std::cout << pbody->hand << std::endl;
	std::cout << pbody->penis << std::endl;
	return;
}



//类-指针
void test4(void){
	class father* pfather = &m_father;
	//赋值
	pfather->money = 444444;
	//访问
	std::cout << pfather->money << std::endl;
	return;
}

//函数-指针(不可以位移)
void test5(void){
	int (*pFunc)(void) = &ret_int;
	const int (*pFunc2)(void) = &ret_const_int;
	Func pFuncEx = ret_int;
	Func2 pFunc2Ex = ret_const_int;
	int tmp;

	//调用
	tmp = pFunc();
	std::cout << tmp << std::endl;
	tmp = pFunc2();
	std::cout << tmp << std::endl;
	tmp = pFuncEx();
	std::cout << tmp << std::endl;
	tmp = pFunc2Ex();
	std::cout << tmp << std::endl;
	return;
}



int main(void){
	//一维数组-指针
	//test1();

	//二维数组-行指针
	test2();

	//结构体-指针
	//test3();

	//类-指针
	//test4();

	//函数-指针
	//test5();

	return 0;
}
