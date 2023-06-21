//编译:
//		gcc -g3 ./point指针偏移量4结构体.c -o x



//
// 1.<stddef.h>
//
/*
	类型:
		ptrdiff_t				 通常用来保存两个指针减法操作的结果, 有符号整数类型(typedef)
		NULL						 空指针常量(值=0)



	函数宏, 最终会被直接替换为数值常量(单位:字节):
		size_t sizeof(): 求'变量声明'中, 变量所需的内容容量
										 (编译器会自动计算并替换为常量:无符号整数类型)

		size_t offsetof(structName, memberName):
										 求'结构体声明'中, '元素成员首地址'与'结构体首地址'的地址偏移
										 (编译器会自动计算并替换为常量:无符号整数类型)
*/



#include <stddef.h>
#include <stdio.h>

struct addr{
	char name[50];
	char street[50];
	int phone;
};

int main(void){
	//指针偏移量查询
	printf("元素name首地址, 与addr{}结构的首地址偏移=%d字节\n", offsetof(struct addr, name));
	printf("元素street首地址, 与addr{}结构的首地址偏移=%d字节\n", offsetof(struct addr, street));
	printf("元素phone首地址, 与addr{}结构的首地址偏移=%d字节\n", offsetof(struct addr, phone));

	//变量实体的容量
	printf("\n\naddr{}结构占用了=%d字节\n", sizeof(struct addr));

	return(0);
}



//
// C11 新增
//
/*
	类型:
		max_align_t (C11) 对齐要求不小于任何其他标量类型的类型(typedef)


	宏常量:
		alignas (C11)便利宏, 展开成关键词 _Alignas(宏常量)
		alignof (C11)便利宏, 展开成关键词 _Alignof(宏常量)

		__alignas_is_defined (C11)展开成整数常量 1(宏常量)
		__alignof_is_defined (C11)展开成整数常量 1(宏常量)<stdnoreturn.h>

		noreturn (C11)便利宏, 展开成 _Noreturn(宏常量)
*/


