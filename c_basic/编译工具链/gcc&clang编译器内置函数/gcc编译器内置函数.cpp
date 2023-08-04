//编译:
//		g++ -g3 ./gcc编译器内置函数.cpp -o x



//gcc 内置函数
/*
	gcc 是一个比较老的编译器, 内置函数其实没有什么亮点, 如下:
		__builtin_parity(unsigned int x)   统计一个数二进制表示中1的个数是偶数还是奇数
		__builtin_popcount(unsigned int x) 统计一个数的二进制表示中1的个数
		__builtin_ffs(unsigned int x)      找出一个数的二进制表示中从末尾开始到遇见第一个1的的位置
		__builtin_clz(unsigned int x)      返回一个数二进制表示中前导零的数目
		__builtin_ctz(unsigned int x)      返回一个数二进制表示中尾零的数目
*/



#include <iostream>
#include <map>



#define max_n (200005)



std::map<int,int> mp;
long long a[max_n];
int n;



int main(void){
	std::cout << __builtin_popcount(3) << std::endl; //3:11 output:2
	std::cout << __builtin_popcount(7) << std::endl; //7:111 output:3

	std::cout << __builtin_parity(3) << std::endl;   //3:11 output:0
	std::cout << __builtin_parity(7) << std::endl;   //7:111 output:1

	std::cout << __builtin_ffs(3) << std::endl;      //3:11 output:1
	std::cout << __builtin_ffs(10) << std::endl;     //10:1010 output:2

	std::cout << __builtin_ctz(3) << std::endl;      //3:11 output:0
	std::cout << __builtin_ctz(10) << std::endl;     //10:1010 output:1

	std::cout << __builtin_clz(3) << std::endl;      //3:11 output:30
	std::cout << __builtin_clz(10) << std::endl;     //10:1010 output:28
	return 0;
}
