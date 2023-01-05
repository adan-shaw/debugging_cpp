//编译:
//		g++ -std=c++11 ./fill通用填充.cpp -o x



//std::fill()函数原型
/*
	template< class ForwardIt, class T >
	void fill(ForwardIt first, ForwardIt last, const T& value)
	{
	    for (; first != last; ++first) {
	        *first = value;
	    }
	}
*/



//std::fill()简介:
/*
	std::fill() 是一个通用容器填充函数, 填充方式跟memset() 有本质上的不同,
	std::fill() 依赖迭代器遍历, memset() 主要用来填充连续内存块;

	ps:
		std::fill() 的实现源码, 会有一个for() 循环, 有if, 有预测分支;
		连续内存填充, 追求性能, 可以使用memset();
*/



#include <algorithm>
#include <vector>
#include <iostream>



int main(void){
	std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::vector<int>::iterator it;

	//ps: v.begin(), v.end() 返回的是vector 迭代器, std::fill() 的第一, 第二参数, 需要传入迭代器;
	std::fill(v.begin(), v.end(), -1);

	for(it=v.begin(); it!=v.end(); ++it)
		  std::cout << *it << " ";

	std::cout << "\n";
	return 0;
}
