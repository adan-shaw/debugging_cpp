//编译:
//	gcc -g3 -nostartfiles ./demo范例\:switch.c -e test_switch -g3 -o x



//'switch 多分支转条'简介
/*
	switch 必然触发'cpu预取指令执行,中断'(不能预测执行结果), 自动连续取指令失败;
	(没办法避免的执行分支, 能少用就少用)
	但switch 的优势是:
		无论有多少个case, 直接中断一次(直接转跳), 不需要从上而下, 不断if, 才能找到匹配结果.

	所以, 只要if 语句超出了3句, 用switch()会更划算;

	//只有一次的时候, 性能与switch(){} 相当
	if(){}
	else{}

	但是如果出现:
	if(){}
	else if(){}
	else{}
	则可能导致double if, 浪费cpu 时间
	(如果匹配条件在第一个命中, 即不会浪费cpu时间, 否则就是在浪费cpu时间)
*/



#include<stdio.h>



int test_switch(void){
	int tmp=3;
	switch(tmp){
		case 0: printf("0\n"); break;
		case 1: printf("1\n"); break;
		case 2: printf("2\n"); break;
		case 3: printf("3\n"); break;
		default: printf("default\n"); break;
	}
	return 0;
}

