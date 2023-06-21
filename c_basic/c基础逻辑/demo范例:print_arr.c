//编译:
//	gcc -g3 ./demo范例\:print_arr.c def_print_arr.h -o x



#include<stdio.h>
#include<stdlib.h>
#include"def_print_arr.h"



//测试打印整形number
void test_print_arr_num(void){
	unsigned long long a[] = {1,2,3,4,5,6};
	//unsigned int a[] = {1,2,3,4,5,6};
	//int a[] = {1,2,3,4,5,6};
	int len_tmp = 6;
	print_arr_num(a, len_tmp);
}

//测试打印浮点float
void test_print_arr_float(void){
	//long double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
	//double a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
	float a[] = {1.0,2.0,3.0,4.0,5.0,6.0};
	int len_tmp = 6;
	print_arr_float(a, len_tmp);
}



int main(void){
	test_print_arr_num();
	printf("\n");
	test_print_arr_float();
	exit(0);
}
