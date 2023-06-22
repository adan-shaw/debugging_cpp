//编译:
//		gcc -g3 func函数参数入栈demo.c -o x



//c/c++ 的function 参数&变量入栈基本规则:
/*
	全局变量/static变量: 指针引用(无性能影响, 无内存消耗, 但容易有数据访问冲突)
	局部变量:            全部推入栈, 因此不要在function 内创建大的buf, 否则会影响c/c++ 函数的执行效率;
	posix API:         指针引用(无性能影响, 共享链接库式内存消耗(相当于函数内嵌), 无数据访问冲突)
	指针:               直接拷贝指针的值, 然后入栈指针地址;
	buf缓冲区作为形参:    这是一种特殊情况, 默认是整个buf形参入堆, 但现代编译器, 可能把它优化为&引用(可能不会优化, 慎用);

	总结:
		遇到buf缓冲区作为形参, 直接用指针/引用代替即可, 减少语法陷阱;
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void func(char* pbuf){
	printf("%s",pbuf);
}

void func2(char* pbuf){
	strncpy(pbuf,"hello world",64);
	printf("%s\n",pbuf);
}

void test_char(void){
	char buf[64]="love you";
	char* pbuf = (char*)malloc(64);
	func(buf);
	func2(buf);
	func(pbuf);
	func2(pbuf);
	free(pbuf);
}

void ifunc(int* pbuf){
	int tmp;
	for(tmp=0;tmp<16;tmp++)
		pbuf[tmp] = 999;
}

void ifunc2(int buf[64]){//可能会被优化为&引用(可能不会优化, 慎用);
	int tmp;
	for(tmp=0;tmp<16;tmp++)
		buf[tmp] = 999;
}

void test_int(void){
	int buf[64],tmp;
	ifunc(buf);
	for(tmp=0;tmp<16;tmp++)
		printf("%d\n",buf[tmp]);
	memset(buf,0,sizeof(buf));
	ifunc2(buf);
	for(tmp=0;tmp<16;tmp++)
		printf("%d\n",buf[tmp]);
}

int main(void){
	test_char();
	test_int();
	return 0;
}
