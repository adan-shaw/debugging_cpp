//编译:
//	g++ -o -g3 x ./wchar_test.cpp


//基础字符操作
/*
	宽字符, 窄字符, sizeof() strlen() wcslen() 等问题:
	sizeof()和strlen()的区别:
		sizeof() 直接给出缓冲区'字节数', 包括n 个'\0' 字符结束符号;
		sizeof() 不受'\0' 结束符号的影响;

		strlen() 直接返回窄字符的'字符数', 不包括'\0' 结束符号;
		strlen() 只能接受一个'\0' 结束符号;
		strlen() 只对窄字符英文有效!!
		1个英文 = 1个char; 1个中文 = 3个窄字符char;

		wcslen() 是宽字符专用的. 并且可以忽略中英文的差异, 统一对待.
		wcslen() 求字符长度是准确的.


	宽字符:
		宽字符有自己的一套'定义方式'和'api 函数'
		宽字符定义: wchar_t c3[] = L"宽字符-中文占多少个字节";
		宽字符'api 函数': 略.
*/





#include <stdio.h>
#include <string.h>
//宽字符必须头文件
#include <wchar.h>



//宽字符测试
void wchar_test(){
	//11*3 + 2 = 35 = sizeof(); '-' 和'\0' = 2 个字节
	//strlen() = 34; 就是这么来的.
	const char c1[] = "窄字符-中文占多少个字节";
	const char c2[] = "abcdefg";		//窄字符-英文占多少个字节

	//宽字符中, '-' 也算一个字符;
	//12*8 + '\0' = 52 = sizeof() 宽字符;
	const wchar_t c3[] = L"宽字符-中文占多少个字节";
	const wchar_t c4[] = L"abcdefg";//宽字符-英文占多少个字节



	printf("窄字符-中文占多少个字节: sizeof() = %d\n", sizeof(c1));
	printf("窄字符-中文占多少个字节: strlen() = %d\n", strlen(c1));

	printf("窄字符-英文占多少个字节: sizeof() = %d\n", sizeof(c2));
	printf("窄字符-英文占多少个字节: strlen() = %d\n", strlen(c2));

	printf("宽字符-中文占多少个字节: sizeof() = %d\n", sizeof(c3));
	printf("宽字符-中文占多少个字节: strlen() = %d\n", wcslen(c3));

	printf("宽字符-英文占多少个字节: sizeof() = %d\n", sizeof(c4));
	printf("宽字符-英文占多少个字节: strlen() = %d\n", wcslen(c4));

	return;
}



int main(void){
	wchar_test();
	printf("宽字符测试 finish\n");
}



