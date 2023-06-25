//编译:
//		g++ -g3 ./c++内存对齐test.cpp -o x



//强行修改内存对齐的粒度大小(默认值是8 bit)
#pragma pack(8)



#include <stdio.h>

//最大的变量放前面, 最优策略
class cpp{
public:
	int i;    //4
	char c;   //1
	short s;  //2
};					//8

class cpp2{
public:
	char c;   //1
	int i;    //4
	short s1; //2
};					//12

//类中的函数成员, 不参与内存对齐;
class cpp3{
public:
	int i;    //4
	char c;   //1
	short s;  //2
	void print(void){ printf("idiot\n"); };
};					//8

//类中的常量, 参与内存对齐(常量尽量使用指针, 这样内存最小, 反正const指针指向的内存是共用的);
class cpp4{
public:
	const char *p="hhhhhhhhhhhhhhhhhhhh";      //常量指针(4bit)
	int i;    //4
	char c;   //1
	short s;  //2
	const char pbuf[32]="hhhhhhhhhhhhhhhhhhhh";//常量缓冲区(32bit)
};					//8

int main(void){
	cpp c;
	cpp2 c2;
	cpp3 c3;
	cpp4 c4;
	printf("c=%d-c2=%d-c3=%d-c4=%d\n", sizeof(c),sizeof(c2),sizeof(c3),sizeof(c4));
	c3.print();
	printf("%s-%s\n", c4.p,c4.pbuf);
	return 0;
}




