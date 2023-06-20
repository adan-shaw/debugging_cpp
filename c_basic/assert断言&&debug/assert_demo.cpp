//
// 1.断言简介
//
/*
	1.断言的实用意义:
		if语句太多, 其实也是会影响性能的.
		所以使用断言assert(), 就是用来替代if语句的, 减少if 语句的使用.


	2.如何禁用断言:
		在编译的时候, 添加'-DNDEBUG 编译选项'来禁用assert()


	3.断言会发送两个信号来终结程序, 其中一个是9 必杀信号.
		断言失败, 程序所在的进程, 肯定会被终结的.


	4.断言的<实现本质>就是一个三目运算符:
		assert(a == b)? passed : printf(error msg)
*/





//
// 2.本assert_demo.cpp 的编译测试:
//
/*
	test 1: 
		# 启动assert() 进行编译, 测试消耗时间
		gcc ./assert_demo.cpp -g3 -o x.exe

	test 2: 
		# 禁用assert() 进行编译, 测试消耗时间
		gcc ./assert_demo.cpp -g3 -DNDEBUG -o x.exe
*/



#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <error.h>



//测试次数总计: N=1亿
unsigned int test_sum=100000000;





//func_1-测试1: 带断言assert() && if, 跑N 次test函数, 计算测试前后消耗的时间.
//func_1-测试2: 禁用断言assert() && if, 跑N 次test函数, 计算测试前后消耗的时间.
void func_1(void){
	unsigned int tmp=0;
	int i;



	printf("func_1 started at %d\n", time(NULL));
	for(i=0; i<test_sum; i++){
		tmp = time(NULL);
		assert(tmp != -1);
	}
	printf("func_1 end at %d\n", time(NULL));
	return;
}





//func_2: 单纯 if, 跑N 次test函数, 计算测试前后消耗的时间.(只具备参考意义)
void func_2(void){
	unsigned int tmp=0;
	int i;



	printf("func_2 started at %d\n", time(NULL));
	for(i=0; i<test_sum; i++){
		tmp = time(NULL);
		if(tmp == -1)
			printf("time() failed, error=%d\n", error);
	}
	printf("func_2 end at %d\n", time(NULL));
	return;
}





//func_3: 单纯 time(NULL), 跑N 次test函数, 计算测试前后消耗的时间.(只具备参考意义)
void func_3(void){
	unsigned int tmp=0;
	int i;



	printf("func_3 started at %d\n", time(NULL));
	for(i=0; i<test_sum; i++)
		tmp = time(NULL);
	printf("func_3 end at %d\n", time(NULL));
	return;
}





int main(void){
	func_1();

	func_2();

	func_3();

	return 0;
}
