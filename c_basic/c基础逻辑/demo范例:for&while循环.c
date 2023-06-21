//编译:
//	gcc -g3 -nostartfiles ./demo范例\:for\&while循环.c -e test_for_while -g3 -o x



//循环体
/*
	for(){}				先执行()中的条件判断, 后执行{}中的代码逻辑
	while(){}			先执行()中的条件判断, 后执行{}中的代码逻辑
	do{}while()		先执行{}中的代码逻辑, 后执行()中的条件判断*特别*

	全部都会触发'cpu预取指令执行,中断'(没办法避免的执行分支, 能少用就少用)
*/



#include<stdio.h>
#include<stdlib.h>



int test_for_while(void){
	int tmp=3;



	for(tmp=3; tmp>0; tmp--){
		printf("%d\n",tmp);
	}


	//需要额外写代码, 初始化'循环计数tmp', 并控制'循环计数tmp'的加减
	tmp=3;
	while(tmp > 0){
		printf("%d\n",tmp);
		tmp--;
	}


	//需要额外写代码, 初始化'循环计数tmp', 并控制'循环计数tmp'的加减
	tmp=3;
	do{
		printf("%d\n",tmp);
		tmp--;
	}while(tmp > 0);


	exit(0);
}
