#include <stdio.h>
#include "extern_head.h"



//extern引用外部全局变量:
extern struct x ex_1;
extern class c ex_2;
extern enum DAY e1;
extern union uu u1;



void ttt(void){
	ex_1.x = 100;

	ex_2.d = 999;

	u1.x = 888;

	printf("static enum:%d\n", MON);
	printf("static enum:%d\n", TUE);
	printf("static enum:%d\n", WED);
	printf("static enum:%d\n", e1);

	//枚举是每次递增+1 的, 所以直接+3 效果也一样
	printf("static enum:%d\n", (enum DAY*)e1 + 3);

	printf("static union:%d\n", u1.x1);

	return;
}
