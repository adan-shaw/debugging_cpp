//编译:
//	gcc -g3 -nostartfiles 3目运算符.c -e test_3 -o x



//使用'3 目运算符'的意义:
/*
	使用'3 目运算符', 并不能减少预测分支的产生, 也就相当于使用了一个if,
	但使用'3 目运算符', 主要是可以节省代码, 日后更方便维护.

	有些少量多次, 但又没办法避免的if 判断, 可以使用'3 目运算符'来节省代码.
*/



//'3 目运算符'的格式:
//		(bool判断条件) ? Yes : No ;



#include<stdio.h>
#include<stdlib.h>



void test_3(void){
	1 ? printf("1: yes\n") : printf("1: no\n") ;

	0 ? printf("0: yes\n") : printf("0: no\n") ;

	1<0 ? printf("1<0: yes\n") : printf("1<0: no\n") ;

	(1>0) ? printf("(1>0): yes\n") : printf("(1>0): no\n") ;

	//不能使用{} 包装if 表达式
	//{1} ? printf("{1}: yes\n") : printf("{1}: no\n") ;

	//不能使用[] 包装if 表达式
	//[1] ? printf("[1]: yes\n") : printf("[1]: no\n") ;

	exit(0);
}
