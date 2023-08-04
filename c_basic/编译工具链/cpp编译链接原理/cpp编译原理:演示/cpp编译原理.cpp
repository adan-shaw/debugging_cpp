//cpp 编译流程demo:
/*
	预处理:
		# 宏字符串, 各类特殊字符串(编译器替换字符串级别)
		g++ -E cpp编译原理.cpp -o cpp编译原理.i


	编译:
		# 得出汇编码
		g++ -S cpp编译原理.i -o cpp编译原理.s


	汇编to机械码s:
		# 将汇编码, 翻译出机械码
		g++ -c cpp编译原理.s -o cpp编译原理.o

		chmod 755 ./cpp编译原理.o
		./cpp编译原理.o
		file ./cpp编译原理.o


	链接:
		# 二进制共享链接库, 链接, 联合, 得出完整的.exe可执行程序
		g++ cpp编译原理.o -lm -lpthread -o cpp编译原理.exe

		chmod 755 ./cpp编译原理.exe
		./cpp编译原理.exe
		file ./cpp编译原理.exe
*/




//我是备注
/*
	我是备注, gcc 编译器会忽略这里面的内容;
*/

#include <stdio.h>
#include <stddef.h>
//不要math.h pthread.h, 本示例仅做简单测试, 不考虑ld 链接器, 链接共享链接库的情况;
//#include <pthread.h>
//#include <math.h>


//
// 错误测试: 预编译的职责
//

// 预编译过程, 只有宏定义出错, 才会报错.
//#define _H2(tmp

// 变量声明严重错误, 编译器也不会报错.
//int love
//suck a cock



//
// 宏定义
//
#define _H1(tmp) #tmp



//
// 结构体, 类声明
//
struct G_info{
	int a;
	char b;
	void* c;
};
G_info m_G_info;

class G_body{
	int a;
	char b;
	void* c;
};
G_body m_G_body;



//
//全局变量
//
int G_val;

char G_str[64]="hello world";

unsigned int G_arr[5]={1,2,3,4,5};




int main(int argc, char* argv[]){
	printf("%s\n\n", _H1("suck a cock adan"));

	printf("%s: %d%d%d%d%d\n\n",\
			G_str, G_arr[0], G_arr[1], G_arr[2], G_arr[3], G_arr[4]);

	printf("struct G_info=%d字节  class G_body=%d字节\n",\
			sizeof(struct G_info), sizeof(class G_body));
	return 0;
}




