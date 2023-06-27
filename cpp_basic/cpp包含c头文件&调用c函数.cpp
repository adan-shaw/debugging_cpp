//编译:
//		gcc -g3 ./cpp包含c头文件\&调用c函数.cpp -o x




//cpp 中调用c 代码:
#ifdef __cplusplus
extern "C" {
#endif
	//包含c 头文件
	#include<stdio.h>

	//定义c 函数
	int add(int a, int b) { return a+b; };

#ifdef __cplusplus
}
#endif



int main(void){
	printf("1+1=%d\n", add(1,1));
	return 0;
}

