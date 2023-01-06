//编译:
//		g++ -std=c++11 ./copy通用深拷贝.cpp -o x



//0.std::copy 的函数原型
/*
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );

ps:
	3 个参数都是迭代器, 其中:
		参数1, 参数2, 都是InputIt 旧式输入迭代器;
		参数3是OutputIt 旧式输出迭代器;
		(big different)

ps2:
	array 数组不支持std::copy:
		由于std::copy 第3个参数是inserter插入迭代器, 而array 数组容器不支持inserter插入迭代器,
		所以array 数组容器不支持std::copy!!

	vector 向量不支持std::front_inserter 插入迭代器, 因为vector 向量没有push_front() 方法;
	vector 向量只支持std::inserter, std::back_inserter;
*/





// 1.copy只负责复制, 不负责申请空间, 所以复制前必须有足够的空间;
/*
	由于stl 大部分容器, 都支持扩容, 这时候会导致空间不足的情况, 只有内存不足时才会出现;
	而array 数组是不能使用std::copy() 函数的, 所以这个实现比较难实现;
	(要触发内存不足, 申请内存失败才能成功)


	拷贝者的容器容量, 小于被拷贝者的容器容量, 导致崩溃(crash):
		这种情况基本上不会发生, stl 大部分容器都会自动扩容;
*/





// 2.std::copy的拷贝效率
/*
	std::copy是C++标准库中的算法接口, 主要用于两个容器间的复制, 其效率要优于自己用for循环逐个复制;
	(但其实也是逻辑循环拷贝,只是不用手写而已)

	std::copy与memcpy的比较:
		期望std::copy不会比memcpy慢, 是痴心妄想, 只能靠编译器优化了;
		(c++ 编译器, 专业版很贵, c++ 编译器如果有优化的话, 可以一定程度上做适当的优化工作, 提高c++ 程序编译后的执行效率)
		但memcpy 只能拷贝连续内存的容器, 而且手动操作的成份比较大, 不太适合面向对象比较大的project;

	如果容器的元素数量众多, 使用std::copy 的时候, 应该注意效率问题;
*/





// 3.std::copy拷贝不同容器中的元素
/*
	std::copy 支持不同容器之间互相拷贝, 只要满足2个条件, 即可互相拷贝:
		* 这两个容器都可以用迭代器访问
		* '用于拷贝的容器', 能容纳'被拷贝容器'的所有元素, 不会造成容量溢出;
*/



#include <algorithm>
#include <iterator>
#include <vector>
#include <array>
#include <list>
#include <iostream>

int main(void){
	std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, v2, v3;
	std::vector<int>::iterator it;
	std::list<int> l{ 7, 5, 16, 8 };

	//1.拷贝'同类型容器'
	it = v2.begin();
	std::copy(v.begin(), v.end(), std::inserter(v2,it));//std::inserter()=定位向量头部插入
	for(it=v2.begin(); it!=v2.end(); ++it)//打印拷贝结果
		std::cout << *it << " ";
	std::cout << "\n";

	//2.拷贝'不同类型容器'
	std::copy(l.begin(), l.end(), std::back_inserter(v3));//std::back_inserter()=向量尾部插入(v2,v3 都是空vector, 头部尾部插入都一样)
	for(it=v3.begin(); it!=v3.end(); ++it)//打印拷贝结果
		std::cout << *it << " ";
	std::cout << "\n";

	return 0;
}
