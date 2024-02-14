//编译:
//		gcc -g3 ./c语言使用汇编.c -o x



#include <stdio.h>

// 定义一个简单的汇编函数, 该函数返回两个整数的和
int add_asm(int a, int b) {
	int sum;
	__asm__ (
		"addl %%ebx, %%eax;"	// 将ebx寄存器的值加到eax寄存器中
		: "=a" (sum)					// 输出: 将sum的值放入eax寄存器
		: "a" (a), "b" (b)		// 输入: 将a和b的值分别放入eax和ebx寄存器
	);
	return sum;
}

int main(void) {
	int n = add_asm(10, 20);
	printf("The sum is: %d\n", n);
	return 0;
}
