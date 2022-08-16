/*
	-nostartfiles = 没有开始函数的.c 源文件file, 
	-e 表示自定义开始函数(需要用exit(0) 结束自定义开始函数, 否则必然会Segmentation fault溢出)
	局部编译(方便局部调试):
		gcc -g3 -nostartfiles arr_stack_mini.h arr_stack_mini.c -e test_as -o x
*/



#include "arr_stack_mini.h"
#include <stdio.h>
#include <stdlib.h>



void test_as(void){
	unsigned int tmp, count, err_count=0;
	as_t* pas = malloc(sizeof(as_t));


	//初始化
	as_init(pas);

	//填满栈
	for(tmp=0; tmp<as_buf_max; tmp++)
		as_push(pas, tmp);
	printf("填满栈: cur_size=%d\n", pas->cur_size);

	//弹空栈
	for(tmp=as_buf_max; tmp>0; tmp--)
		printf("%d, ", as_pop(pas));
	printf("\n弹空栈: cur_size=%d\n", pas->cur_size);


	printf("test_as() finished, err_count=%d\n",err_count);
	free(pas);
	exit(0);
}

