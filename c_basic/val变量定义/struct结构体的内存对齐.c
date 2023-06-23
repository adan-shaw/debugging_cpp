//编译:
//		gcc -g3 ./struct结构体的内存对齐.c -o x



//struct 定义的要点
/*
1.碰到大内存缓冲区需求(很少用), 请看: ../val超大buf的定义.c


2.结构体之间的内嵌:
	很多场合, 会碰到结构体内嵌的使用, 因此, 用到结构体内嵌时, 应当:
	* 保证内嵌时, 用指针进行内嵌:
		因为性能更好, 无论是编译, 调试, 还是执行;
		如果直接结构体内嵌, 碰到内嵌深度较大, 结构体成员的size 过大, 编译器会很卡, 不方便编译, 调试;
		保留一个指针, 直接malloc/new, 性能更好, 只是编码时复杂一点而已, 不影响;

	* 尽量避免内嵌深度过大:
		极限内嵌深度, 最好保证在3 级内嵌以内, 除非是物理设备固定的内嵌方式, 业务逻辑没办法避免, 那只能硬着头皮上了;
		普通是自己定义的数据格式, 最好就是: 拒绝内嵌, 不内嵌;


3.结构体的内存对齐原则:
	需要做到内存对齐, 以节省内存, 请看下面的demo;
*/



//内存对齐原则: 大的元素, 放在顶部, 从上往下排列, 从大到小;
struct SA{
	short int a;	// 2字节
								// 6个空洞
	double b;			// 8
	int d;				// 4
								// 4个空洞
};							// 24 bit(总消耗)

struct SB{
	double b;			// 8
	int d;				// 4
	short int a;	// 2
								// 2个空洞
								// 16 bit(总消耗)
};



#include <stdio.h>
#include <stdbool.h>



//内存未对齐的结构体
struct S{
	int deskA;
	double deskB;
	bool deskC;
	int chairA;
	double chairB;
	bool chairC;
};

//内存对齐的结构体
struct S2{
	int deskA;
	int chairA;
	double deskB;
	double chairB;
	bool deskC;
	bool chairC;
};



int main(void){
	struct S s;
	struct S2 s2;
	struct SA sa;
	struct SB sb;
	printf("s=%d, s2=%d\n", sizeof(s), sizeof(s2));
	printf("sa=%d, sb=%d\n", sizeof(sa), sizeof(sb));
	return 0;
}


