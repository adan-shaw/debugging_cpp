//编译:
//		g++ -std=c++11 -g3 ./multimap.cpp -o x 



//multimap api list
/*
	成员类
		value_compare			比较类型为value_type的对象(类)

	成员函数
		(构造函数)					构造 multimap
		(析构函数)					析构 multimap
		operator=					赋值给容器
		get_allocator			返回相关的分配器

	迭代器
		begin							返回指向容器第一个元素的迭代器
		cbegin
		end								返回指向容器尾端的迭代器
		cend
		rbegin						返回指向容器最后元素的逆向迭代器
		crbegin
		rend							返回指向前端的逆向迭代器
		crend

	容量
		empty							检查容器是否为空
		size							返回容纳的元素数
		max_size					返回可容纳的最大元素数

	修改器
		clear							清除内容
		insert						插入元素或结点 (C++17 起)
		emplace						原位构造元素(C++11)
		emplace_hint			使用提示原位构造元素(C++11)
		erase							擦除元素
		swap							交换内容
		extract						从另一容器释出结点(C++17)
		merge							从另一容器接合结点(C++17)

	查找
		count							返回匹配特定键的元素数量
		find							寻找带有特定键的元素
		contains					检查容器是否含有带特定关键的元素(C++20)
		equal_range				返回匹配特定键的元素范围
		lower_bound				返回指向首个不小于给定键的元素的迭代器
		upper_bound				返回指向首个大于给定键的元素的迭代器

	观察器
		key_comp					返回用于比较键的函数
		value_comp				返回用于在value_type类型的对象中比较键的函数

	非成员函数
		operator==				按照字典顺序比较 multimap 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::multimap)	特化 std::swap 算法
		erase_if(std::multimap)		擦除所有满足特定判别标准的元素(C++20)
*/



#include <cstdio>
#include <cassert>
#include <map>

using namespace std;



int main(void){
	int tmp;
	//1.创建multimap
	multimap<int,int> x1;
	multimap<int,int> x2 { {1,1},{2,10},{3,100},{4,1000}, };
	multimap<int,int,less<int>> x3(x2);						//a copy of x2, 重写排序比较函数为less<int>升序, 从小到大; greater<int>降序不能用, 不知道为什么!!
	multimap<int,int> x4(x2.begin(), x2.end());		//iterator 构造copy
	multimap<int,int>::iterator it,itlow,ithigh;	//顺向迭代器
	multimap<int,int>::reverse_iterator rit;			//逆向迭代器
	multimap<int,int>::key_compare kcomp;					//key 比较对象的副本
	//multimap<int,int>::value_compare vcomp;			//value 比较对象的副本(gcc 没有这个对象, 十分诡异)



	//2.void clear();															//清空multimap
	x1.clear();
	x3.clear();



	//3.bool empty();															//检查multimap 是否为空
	if(x1.empty())
		printf("3 -- x1 multimap is now empty\n");
	if(x2.empty())
		printf("3 -- x2 multimap is now empty\n");
	if(x3.empty())
		printf("3 -- x3 multimap is now empty\n");



	//4.size_type size();													//返回multimap 的size()
	printf("4 -- x2 multimap size()=%d\n", x2.size());



	//5.顺向迭代器
	//iterator begin();														//返回multimap 的顺向front() 迭代器
	//iterator end();															//返回multimap 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");

	//6.逆向迭代器
	//iterator rbegin();													//返回multimap 的逆向front() 迭代器
	//iterator rend();														//返回multimap 的逆向end() 迭代器
	printf("6 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("[%d,%d]  ", (*rit).first, (*rit).second);
	printf("\n");



	//7.删除元素--三种删除方式
	//void erase(iterator position);																				//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);																		//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);														//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());													//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);																	//输入'常量值'删除(存在则删除, 不存在则忽略)

	assert(!x2.empty());
	it=x2.find(3);																//迭代器区域删除(区域内, 圈中则删除, 没有圈中则忽略)
	if(it != x2.end())
		x2.erase(it, x2.end());

	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//8.插入新元素
	//iterator insert( const value_type& value );														//pair一对值的'插入常量'
	//iterator insert( iterator hint, const value_type& value );						//迭代器'插入常量'
	//iterator emplace( Args&&... args );																		//尝试性插入, 返回pair<iterator,bool>; 
																																					//有插入, bool=true, 并返回迭代器; 无插入, bool=false, 表明元素已经存在, 返回迭代器;

	it = x2.insert(x2.begin(), pair<int,int>(9,123));												//插入方式1: 迭代器 + pair() + insert()
	it = x2.insert(x2.begin(), pair<int,int>(9,65));

	it = x2.insert(pair<int,int>(0,10));																		//插入方式2: pair() + insert()
	it = x2.insert(pair<int,int>(1,20));
	it = x2.insert(pair<int,int>(2,30));
	it = x2.insert(pair<int,int>(3,40));
	it = x2.insert(pair<int,int>(4,50));

	//x2[5] = 60;																														//插入方式3: '模仿'数组插入语法方式(key 值不是唯一的, 没办法支持这种插入方式)
	//x2[6] = 70;
	//x2["name"] = "adan";												//假如是字符串, 可以这样操作

	it = x2.insert(multimap<int, int>::value_type(7, 80));									//插入方式4: value_type() + insert()
	it = x2.insert(multimap<int, int>::value_type(8, 90));

	it = x2.emplace(pair<int,int>(99,1230));																//插入方式5: emplace 返回判断结果, 查看是否插入, 还是multimap 中已存在该值
	printf("%d, %d\n", (*it).first,(*it).second);

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//9.void swap(multimap<Key,Compare,Allocator>& st);											//multimap 之间交换元素
	x1.swap(x2);

	printf("9 -- x2:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");

	printf("9 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//10.size_type count(cont key_type& x);																	//统计某个值元素的个数
	printf("10 -- x1, element '2' count=%d\n",x1.count(2));
	printf("10 -- x1, element '11' count=%d\n",x1.count(11));



	//11.iterator find(const key_type& x);																	//查找指定元素
	it=x1.find(4);
	if(it != x1.end())
		printf("11 -- x1.find(4) = %d\n", *it);
	else
		printf("11 -- x1.find(4) failed!!\n", *it);



	//12.iterator lower_bound(const key_type& x);														//返回首个不小于'给定key值x'的元素的迭代器('给定key值x'不一定需要存在于set中, 等于也算是不小于)
	it = x1.lower_bound(25);
	if(it != x1.end())
		printf("12 -- lower_bound(25)=存在'不小于25 的key值', 这个key&value是: [%d,%d]\n", it->first, it->second);
	else
		printf("12 -- lower_bound(25)=不存在'不小于25 的key值'\n");

	//	 iterator upper_bound(const key_type& x);														//返回首个大于'给定key值x'的元素的迭代器
	it = x1.upper_bound(25);
	if(it != x1.end())
		printf("12 -- upper_bound(25)=存在'大于25 的key值', 这个key&value是: [%d,%d]\n", it->first, it->second);
	else
		printf("12 -- upper_bound(25)=不存在'大于25 的key值'\n");



	//13.删除范围区域中的元素
	x4.clear();
	for(tmp=0; tmp<10; tmp++)											// 1  2  3  4  5  6  7  8  9    key
		x4.insert(pair<int,int>(tmp,tmp*10));				// 10 20 30 40 50 60 70 80 90   value
	itlow=x4.lower_bound(3);											//			 ^
	ithigh=x4.upper_bound(6);											//									 ^
	x4.erase(itlow,ithigh);												// 10 20 70 80 90

	printf("13 -- x4:");
	for(it = x4.begin(); it != x4.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//
	// multimap 观察器: 功能很鸡肋, 不知道有何作用, 还不如普通的迭代器遍历, 慎用(诡异的用法, 无聊得很!!)
	//
	//14.key_compare key_comp();																						//取出multimap 中所有的key, 作为副本, 用于比较(返回值可以当成是比较函数用, 但这个比较函数, 只比较值, 不比较迭代器)
	kcomp = x1.key_comp();																									//(其实key_compare 取出后, 也包含multimap 中所有的value)

	//打印
	it=x1.begin();
	tmp=x1.rbegin()->first;
	printf("14 -- x1:");
	while(kcomp((*it++).first, tmp))							//大于比较对象才打印, 小于&等于都不打印!!
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	return 0;
}


