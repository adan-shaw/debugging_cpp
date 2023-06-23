//编译:
//		gcc -g3 ./union联合体.c -o x



//union 共用同一块内存区域, 取最大的来算; union 中的其中一个值改变, 其他的值也会跟着全部更改
union _union{
	int a;//4
	int b;//4
	char c[8];//8
};//8
//定义一个不等大的union
union _union union_tmp;



//union 联合体使用要义:
/*
	* 尽量保持union 里面的元素等大!!
		union 里面的元素不等大, 会出现有元素会出现数据错乱的;

	* 尽量使用2 个元素组成联合体, 多个元素组成联合体, 容易出现不等大的union, 尽量避免这种情况的出现;

	* 任何一个元素改变, 所有元素都跟着改变, 因此要注意更改后, 各个元素变动后的影响;
*/



#include <stdio.h>

int main(void){
	union_tmp.a = 999;
	printf("%d-%d-%s\n",union_tmp.a,union_tmp.b,union_tmp.c);

	union_tmp.b = 111;
	printf("%d-%d-%s\n",union_tmp.a,union_tmp.b,union_tmp.c);

	return 0;
}

