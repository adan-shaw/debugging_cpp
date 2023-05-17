//编译:
//		g++ -std=c++98 ./bitset容器\:位集合.cpp -g3 -o x 



//bitset 资料
/*
	头文件:
		#include <bitset>

	构造函数:
		bitset()                   : 每一位都是false
		bitset(unsigned long val)  : 设为 val 的二进制形式
		bitset(const string& str)  : 设为串str

	运算符:
		operator []                : 访问其特定的一位
		operator ==/!=             : 比较两个bitset内容是否完全一样
		operator &/&=/|/| =/^/^=/~ : 进行按位与/或/异或/取反操作; bitset只能与bitset 进行位运算, 若要和整型进行位运算, 要先将整型转换为bitset
		operator <</>>/<<=/>>=     : 进行二进制左移/右移
		operator <</>>             : 流运算符, 这意味着你可以通过cin/cout 进行输入输出

	成员函数:
		count()                    : 返回 true 的数量
		size()                     : 返回bitset的大小
		test(pos)                  : 它和vector中的at()的作用是一样的, 和[]运算符的区别是: 越界检查, 一旦越界, 弹出更准确的报错信息;
		any()                      : 若存在某一位是true, 则返回true, 否则返回false
		none()                     : 若所有位都是false, 则返回true, 否则返回false
		all()                      : c++11, 若所有位都是true, 则返回true, 否则返回false
		set()                      : 将整个bitset设置成true
		set(pos, val = true)       : 将某一位设置成true / false
		reset()                    : 将整个bitset设置成false
		reset(pos)                 : 将某一位设置成false; 相当于set(pos, false)
		flip()                     : 翻转每一位,相当于异或一个全是1的bitset
		flip(pos)                  : 翻转某一位
		to_string()                : 返回转换成的字符串(返回string 类)
		to_ulong()                 : 返回转换成的unsigned long
		to_ullong()                : c++11, 返回转换成的unsigned long long;
*/



#include <cstdio>
#include <string>
#include <bitset>



int main(void){
	//1.bitset位集合初始化
	std::bitset<32> x1,x4,xtmp;								//创建32bits的位集合, 初始化为默认值(默认值为全0)
	std::bitset<32> x2(120ul);								//创建32bits的位集合, 初始化为unsigned long = 120

	std::bitset<32> x3(std::string("01011"));	//创建32bits的位集合, 初始化initialize from string
	std::bitset<1000> bs;											//创建1000bits的位集合(一般不需要这么多bits, 慎用)



	//2.随机访问
	printf("x3[0]=%d\n", x3[0]);			//printf() 难以打印1bit 数据, 无解了0.0
	printf("x3[0]=%d\n", x3.test(0));	//更安全的随机访问方式(如果内存访问越界, 会有更多报错信息)

	if(x3[0] == x3.test(0))
		printf("x3: x3[0] == x3.test(0) == %d\n", x3.test(0));

	if(x3[0] == x3.test(2))
		printf("x3: x3[0] == x3.test(2) == %d\n", x3.test(2));



	//3.统计类
	//size_t count(); 统计bitset 被置1 的位数的count
	printf("x3: 被置1的位数count=%d\n", x3.count());

	//size_t size(); 返回bitset 的总容量有多少bit
	printf("x3: 总容量有%d bit\n", x3.size());



	//4.判断类
	//bool any(); 判断bitset 至少有一位被置1? 是就返回true
	if(x3.any())
		printf("x3: bitset 至少有一位被置1\n");

	//bool none(); 判断bitset 全为0? 是就返回true
	if(x1.none())
		printf("x1: bitset 全为0\n");

	//bool all(); 判断bitset 全为1? 是就返回true
	if(x1.all())
		printf("x1: bitset 全为1\n");

	//运算符==, !=, 比较两个bitset 是否相同
	if(x1 == x4)
		printf("x1 == x4\n");

	//bitset 的位数不同, 不能比较, 否则编译报错!!
	//if(x1 != bs)
		//printf("x1 != bs\n");



	//5.操作类
	//bitset<N>& reset(); - 重置bitset 的某一个位为0
	x3.reset(1);	//指定: 清0第1位
	printf("x3: %s\n", x3.to_string().c_str());
	x3.reset();		//默认: 全部清0
	printf("x3: %s\n", x3.to_string().c_str());

	//bitset<N>& set();   - 重置bitset 的某一个位为1
	x3.set(0);		//指定: 置1第0位
	printf("x3: %s\n", x3.to_string().c_str());
	x3.set();			//默认: 全部置1
	printf("x3: %s\n", x3.to_string().c_str());

	//bitset<N>& flip();  - 反转bitset 的某一个位
	x3.flip(0);		//指定: 反转第0位
	printf("x3: %s\n", x3.to_string().c_str());
	x3.flip();		//默认: 反转全部
	printf("x3: %s\n", x3.to_string().c_str());



	//6.转换
	//string to_string(); - bitset 转换为字符串(返回字符串string类)
	printf("x3: %s\n", x3.to_string().c_str());

	//to_ulong(); - bitset 转换为unsigned long
	printf("x3: %ld\n",x3.to_ulong());



	//7.运算符
	//'=' 赋值, 拷贝
	x4 = x3;
	printf("x4: %s\n", x4.to_string().c_str());

	//'&'与
	x1 = x1 & x3;
	printf("x1: %s\n", x1.to_string().c_str());

	//'|'或
	x1 = x1 | x3;
	printf("x1: %s\n", x1.to_string().c_str());

	//'~'非
	x1 = ~ x1;
	printf("x1: %s\n", x1.to_string().c_str());

	//'^'异或
	x1 = x1 ^ x3;
	printf("x1: %s\n", x1.to_string().c_str());

	//'<<'左移
	x1 = x1 << 8;
	printf("x1: %s\n", x1.to_string().c_str());

	//'>>'右移
	x1 = x1 >> 4;
	printf("x1: %s\n", x1.to_string().c_str());

	return 0;
}



