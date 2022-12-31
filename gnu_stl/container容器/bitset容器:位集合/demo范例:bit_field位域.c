//编译:
//	gcc -g3 ./demo范例\:bit_field位域.c -o x



#include<stdio.h>



//位域定义, 位操作的区别:
/*
位域定义:
	一般是将int,short,char,long(uint,ushort,uchar,ulong) 等整形数, 
	重新按位划分变量, 每n个 位一个变量, 组成新的结构体.

	好处:
		节约内存, 性能出色, 容易使用, 容易分辨, 不用记位(可以取代低级为操作)

	坏处:
		拓展性不强(一般用在嵌入式)


位操作:
	一般是用来对位域变量, 进行位运算, 运算符号有:
			&  按位与
			|  按位或
			^  按位异或
			～ 按位取反
			<< 左移n 位bit
			>> 右移n 位bit
*/



//位域定义的应用:
/*
	一般是用来做嵌入式, 也可以考虑用来做: 
		校验数算法(隐藏校验数及其算法)
		存储bool 开关, 全局变量数据, 节省内存, 集中管理代码开关变量
		用来记住io 引脚
*/



//位域定义的本质:
/*
	其实位域的声明, 只是表演给编译器看的而已, 
	编译器会自动将位域定义和操作, 翻译为位操作, 
	最终写到程序中执行.
*/



//1.位域结构A(一共32bit, 4字节刚好):
struct A{
	short A_x:16;
	//重定义为short 需要16bit 2字节, 如果不够16bit, 你应该定义为: unsigned, 免得产生歧义
	unsigned A_a:8;
	unsigned A_b:2;
	unsigned A_c:6;
};


//2.位域结构B(13bit,但由于内存对齐原则,必须是8bit的倍数; 所以最终会占用16bit(2字节)后面的2个bit被弃用!!
//	不知为何, 最终占用了4 字节, 32bit 的内存(奇怪了): sizeof(struct B)=4 字节
struct B{
	unsigned B_a:5;
	//无名位域, 相当于弃用
	unsigned :1;
	unsigned B_b:4;
	unsigned B_c:4;
};


//3.位域结构C(一共占64bit, 8 字节)
struct C{
	unsigned int C_a:32;
	unsigned :2;
	unsigned C_b:3;
	unsigned C_c:6;
	//内存对齐原则, 后面不足32bit, 补上32bit(最终sizeof(struct C)=8 字节=64 bit)
};



int main(void){
	struct A A_test;
	struct B B_test;
	struct C C_test;



	//1.
	printf("struct A=%d\n",sizeof(struct A));
	A_test.A_a = 127;
	A_test.A_b = 3;
	A_test.A_c = 15;


	//2.
	printf("struct B=%d\n",sizeof(struct B));
	B_test.B_a = 1;
	B_test.B_b = 7;
	B_test.B_c = 3;


	//3.
	printf("struct C=%d\n",sizeof(struct C));
	C_test.C_a = 1;
	C_test.C_b = 7;
	C_test.C_c = 3;

	C_test.C_c = 4;	//错误, 因为C_test.C_c 只有2 bit, 最大值 = 3
	C_test.C_c = 5;	//编译器对于这种错误,不强制报error,而是报警告错误
									//因此要小心'警告错误'才是.


	printf("位域变量测试 finish\n");
	return 0;
}
