//编译:
//		g++ -std=c++11 ./inserter插入迭代器.cpp -o x



//inserter插入迭代器简介:
/*
	inserter插入迭代器, 是LegacyOutputIterator输出迭代器的一种;

	三种inserter插入迭代器api, 分别是:
		std::inserter()						根据容器自身的'读迭代器'进行定位, 返回该位置的inserter插入迭代器(写迭代器), 有两个参数. 需要容器支持insert() 操作;
		std::front_inserter()			直接返回容器的头部inserter插入迭代器. 需要容器支持push_front() 操作;
		std::back_inserter()			直接返回容器的尾部inserter插入迭代器. 需要容器支持push_back() 操作;

	现在知道为什么只有LegacyOutputIterator输出迭代器, 是写迭代器了吧?
	往容器中插入元素的时候, 很常用;

	ps:
		array 数组不支持inserter插入迭代器, 因为array 根本连insert(), push_front(), push_back()操作都没有;
		对array 数组使用inserter插入迭代器, 编译器会报错!!
*/



#include <cstdio>
#include <iterator>
#include <deque>
#include <algorithm>



int main(void){
	std::deque<int> v1 = { 1, 2, 3 };
	std::deque<int> v2 = { 4, 5, 6 };
	std::deque<int>::iterator iter;

	//1.std::front_inserter() 头部插入
	std::copy(v1.begin(), v1.end(), std::front_inserter(v2)); 
	//  v2 = { 3 2 1 4 5 6 }
	for(iter = v2.begin(); iter != v2.end(); ++iter)
		printf("%d ",*iter);
	printf("\n",*iter);

	//2.std::back_inserter() 尾部插入
	std::copy(v1.begin(), v1.end(), std::back_inserter(v2)); 
	//  v2 = { 3 2 1 4 5 6 1 2 3}
	for(iter = v2.begin(); iter != v2.end(); ++iter)
		printf("%d ",*iter);
	printf("\n",*iter);

	//3.std::inserter() 定位插入
	iter = v2.begin();
	iter += 3;
	std::copy(v1.begin(), v1.end(), std::inserter(v2,iter)); 
	//  v2 = { 3 2 1 1 2 3 4 5 6 1 2 3}
	for(iter = v2.begin(); iter != v2.end(); ++iter)
		printf("%d ",*iter);
	printf("\n",*iter);

	return 0; 
}
