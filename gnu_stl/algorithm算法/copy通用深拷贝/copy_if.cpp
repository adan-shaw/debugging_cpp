//编译:
//		g++ -std=c++11 ./copy_if.cpp -o x



//std::copy_if(): 有选择性深拷贝

//std::copy_if()函数原型
/*
template< class InputIt, class OutputIt, class UnaryPredicate >
OutputIt copy_if( InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred );

ps:
	最后一个参数, 是一个函数指针, 用作std::copy_if()过程中, 进行元素筛选;
*/


#include <algorithm>
#include <iterator>
#include <vector>
#include <array>
#include <list>
#include <iostream>

//std::copy_if() 元素筛选函数
/*
	函数返回类型, 必须跟容器元素保持一致; 
	函数的形参, 应只有一个, 且形参类型, 也必须跟容器元素保持一致;
*/
int f(int x){ return x % 3 == 0; }

int main(void){
	std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, v2, v3;
	std::vector<int>::iterator it;
	std::list<int> l{ 9, 5, 16, 8 };

	//1.拷贝'同类型容器'
	it = v2.begin();
	std::copy_if(v.begin(), v.end(), std::inserter(v2,it), [](int x) { return x % 2 != 0; });//std::inserter()=定位向量头部插入
	for(it=v2.begin(); it!=v2.end(); ++it)//打印拷贝结果
		std::cout << *it << " ";
	std::cout << "\n";

	//2.拷贝'不同类型容器'
	std::copy_if(l.begin(), l.end(), std::back_inserter(v3), f);//std::back_inserter()=向量尾部插入(v2,v3 都是空vector, 头部尾部插入都一样)
	for(it=v3.begin(); it!=v3.end(); ++it)//打印拷贝结果
		std::cout << *it << " ";
	std::cout << "\n";

	return 0;
}
