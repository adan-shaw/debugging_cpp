//编译:
//		gcc -g3 ./arr初始化.c -o x

#include<stdio.h>
#include<string.h>


//数组正确初始化的方式:
int arr_test[] = {1,2,3,4,5,6,7,8,9,0};
//数组创建64 个元素, 然后自动初始化前10 个
int arr_err[64][64] = {1,2,3,4,5,6,7,8,9,0};

void test(void){
	int tmp;
	int* pi;

	//赋值初始化(循环体)
	pi = arr_test;
	for(tmp=0;tmp<sizeof(arr_test)/sizeof(int);tmp++)
		pi[tmp] = tmp;

	//格式化初始化(连续内存格式化)
	for(tmp=0;tmp<sizeof(arr_err)/sizeof(int);tmp++)
		printf("%d ", arr_err[tmp]);
	printf("\n");

	memset(arr_err, 0, sizeof(arr_err));
	for(tmp=0;tmp<sizeof(arr_err)/sizeof(int);tmp++)
		printf("%d ", arr_err[tmp]);
	printf("\n");
	return;
}

int main(void){
	test();
	return 0;
}
