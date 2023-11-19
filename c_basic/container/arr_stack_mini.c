/*
	-nostartfiles = 没有开始函数的.c 源文件file, 
	-e 表示自定义开始函数(需要用exit(0) 结束自定义开始函数, 否则必然会Segmentation fault溢出)
	局部编译(方便局部调试):
		gcc -g3 -DNODEBUG -nostartfiles arr_stack_mini.h arr_stack_mini.c -e test_as_basic -o x
*/



#include "arr_stack_mini.h"
#include <stdio.h>
#include <stdlib.h>

#define as_size_max4test (64)

void test_as_basic(void){
	unsigned int tmp, val;
	as_type* p;
	as_t* pas = malloc(sizeof(as_t));
	as_type* buf4test;
	buf4test = malloc(as_size_max4test * sizeof(as_type));

	//初始化
	as_init(pas,as_size_max4test);
	printf("init栈: cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));
	printf("init栈: cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));



	//填满栈
	for(tmp=0; tmp<as_size_max(pas); tmp++){
		as_push(pas, tmp);
		//printf("%d, ", tmp);
	}
	printf("填满栈: cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));

	//为批量操作保留测试数据
	memcpy(buf4test, pas->buf, as_size_max4test*sizeof(as_type));

	//弹空栈
	for(tmp=0; tmp<as_size_max(pas); tmp++){
		as_pop(pas, val);
		printf("%d, ", val);
	}
	printf("\n弹空栈: cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));




	//批量插入数据
	printf("\n批量插入数据: cur_size%d+N%d, as_size_max=%d\n", as_cur_size(pas),as_size_max4test, as_size_max(pas));
	as_pushN(pas, buf4test, as_size_max4test);

	//批量弹空栈
	memset(buf4test,0,as_size_max4test);
	as_popALL(pas, buf4test);
	//as_pop(pas, val);
	printf("\n批量弹空栈: cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));



	//优先插入数据as_push_prio1
	as_push_prio1(pas, 1);
	printf("\n优先插入数据as_push_prio1:cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));
	as_pop(pas, val);
	printf("%d, ", val);

	printf("\n优先插入数据as_push_prioN:cur_size=%d, as_size_max=%d\n", as_cur_size(pas), as_size_max(pas));
	as_push_prioN(pas, buf4test, as_size_max4test);
	for(tmp=0; tmp<as_size_max(pas); tmp++){
		as_pop(pas, val);
		printf("%d, ", val);
	}
	//as_pop(pas, val);

	//摧毁栈
	as_destroy(pas);
	free(pas);

	free(buf4test);
	exit(0);
}


