//编译:
//		g++ -std=c++11 -g3 ./set.cpp -o x 



//set api list
/*
	成员函数
		(构造函数)				构造 set
		(析构函数)				析构 set
		operator=				赋值给容器
		get_allocator		返回相关的分配器

	迭代器
		begin						返回指向容器第一个元素的迭代器
		cbegin
		end							返回指向容器尾端的迭代器
		cend
		rbegin					返回指向容器最后元素的逆向迭代器
		crbegin
		rend						返回指向前端的逆向迭代器
		crend

	容量
		empty						检查容器是否为空
		size						返回容纳的元素数
		max_size				返回可容纳的最大元素数

	修改器
		clear						清除内容
		insert					插入元素或结点 (C++17 起)
		emplace					原位构造元素(C++11)
		emplace_hint		使用提示原位构造元素(C++11)
		erase						擦除元素
		swap						交换内容
		extract					从另一容器释出结点(C++17)
		merge						从另一容器接合结点(C++17)

	查找
		count						返回匹配特定键的元素数量
		find						寻找带有特定键的元素
		contains				检查容器是否含有带特定关键的元素(C++20)
		equal_range			返回匹配特定键的元素范围
		lower_bound			返回指向首个不小于给定键的元素的迭代器
		upper_bound			返回指向首个大于给定键的元素的迭代器

	观察器
		key_comp				返回用于比较键的函数
		value_comp			返回用于在value_type类型的对象中比较键的函数

	非成员函数
		operator==			按照字典顺序比较 set 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::set)		特化 std::swap 算法
		erase_if(std::set)		擦除所有满足特定判别标准的元素(C++20)
*/



#include <iostream>
#include <cstdio>
#include <cassert>
#include <set>

using namespace std;



int main(void){
	int tmp;
	int arr[]= {10,20,30,40,50};
	//1.创建set
	set<int> x1;												//empty set of ints
	set<int> x2(arr,arr+5);							//指针拷贝[起始地址, (起始地址+5)=结束地址]
	set<int> x3(x2);										//a copy of x2
	set<int> x4(x2.begin(), x2.end());	//iterator ctor.
	set<int>::iterator it,itlow,ithigh;	//顺向迭代器
	set<int>::reverse_iterator rit;			//逆向迭代器
	set<int>::key_compare kcomp;				//key 比较对象的副本
	set<int>::value_compare vcomp;			//value 比较对象的副本
																			//pair一对变量
	pair<set<int>::iterator, bool> set_pair_val;
	pair<set<int>::iterator, set<int>::iterator> set_pair_equal_range;



	//2.void clear();										//清空set
	x1.clear();
	x3.clear();



	//3.bool empty();										//检查set 是否为空
	if(x1.empty())
		printf("3 -- x1 set is now empty\n");
	if(x2.empty())
		printf("3 -- x2 set is now empty\n");
	if(x3.empty())
		printf("3 -- x3 set is now empty\n");



	//4.size_type size();								//返回set 的size()
	printf("4 -- x2 set size()=%d\n", x2.size());



	//5.顺向迭代器遍历
	//iterator begin();									//返回set 的顺向front() 迭代器
	//iterator end();										//返回set 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//6.逆向迭代器遍历
	//iterator rbegin();								//返回set 的逆向front() 迭代器
	//iterator rend();									//返回set 的逆向end() 迭代器
	printf("6 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");



	//7.删除元素--三种删除方式
	//void erase(iterator position);																//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);														//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);										//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());								//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);												//输入'常量值'删除

	assert(!x2.empty());
	it=x2.find(40);											//迭代器区域删除
	if(it != x2.end())
		x2.erase(it, x2.end());

	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//8.插入新元素(有2种方式)
	//std::pair<iterator,bool> insert(const value_type& x);					//直接插入常量, 自动排序, 插入位置不确定, 返回pair 对
	//iterator insert(iterator position, const value_type& x);			//根据迭代器插入, 自动排序, 插入位置确定, 返回迭代器
	//std::pair<iterator,bool> emplace( Args&&... args );						//尝试性插入, 返回pair<iterator,bool>; 
																																	//有插入, bool=true, 并返回迭代器; 无插入, bool=false, 表明元素已经存在, 返回迭代器;
	set_pair_val = x2.insert(123);
	set_pair_val = x2.insert(123);
	printf("%d, %d\n", *set_pair_val.first, set_pair_val.second);

	x2.insert(x2.begin(), 223);

	set_pair_val = x2.emplace(123);
	printf("%d, %d\n", *set_pair_val.first, set_pair_val.second);		//无插入, 直接返回迭代器, bool=false(emplace 比insert 稍微安全一点)

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.void swap(set<Key,Compare,Allocator>& st);									//两个set之间交换元素
	x1.swap(x2);

	printf("9 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("9 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//10.size_type count(cont key_type& x);													//统计某个值元素的个数
	printf("10 -- x1, element '20' count()=%d\n", x1.count(20));
	printf("10 -- x1, element '50' count()=%d\n", x1.count(50));



	//11.iterator find(const key_type& x);													//查找指定元素
	it=x1.find(123);
	if(it != x1.end())
		printf("11 -- x1.find(123)=%d\n", *it);
	else
		printf("11 -- x1.find(123) failed !!\n", *it);



	//12.iterator lower_bound(const key_type& x);										//返回首个不小于'给定key值x'的元素的迭代器('给定key值x'不一定需要存在于set中, 等于也算是不小于)
	it = x1.lower_bound(25);
	if(it != x1.end())
		printf("12 -- lower_bound(25)=存在'不小于25 的值'\n", *it);
	else
		printf("12 -- lower_bound(25)=不存在'不小于25 的值'\n", *it);

	//	 iterator upper_bound(const key_type& x);										//返回首个大于'给定key值x'的元素的迭代器
	it = x1.upper_bound(25);
	if(it != x1.end())
		printf("12 -- upper_bound(25)=存在'大于25 的值'\n", *it);
	else
		printf("12 -- upper_bound(25)=不存在'大于25 的值'\n", *it);



	//13.pair<iterator,iterator> equal_range(const key_type& x)const;//返回'范围区域'组成的迭代器pair对, 等价于pair(lower_bound(), upper_bound()); ps: set 集合是有序的
	set_pair_equal_range = x1.equal_range(25);
	printf("13 -- set->equal_range(25): [%d, %d]\n", *set_pair_equal_range.first, *set_pair_equal_range.second);
	//删除区域内的元素



	//14.删除范围区域中的元素
	for(tmp=1; tmp<10; tmp++)
		x4.insert(tmp*10);								// 10 20 30 40 50 60 70 80 90
	itlow=x4.lower_bound(30);						//			 ^
	ithigh=x4.upper_bound(60);					//								^
	x4.erase(itlow,ithigh);							// 10 20 70 80 90

	printf("14 -- x4:");
	for(it = x4.begin(); it != x4.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//
	// set 观察器: 功能很鸡肋, 不知道有何作用, 还不如普通的迭代器遍历, 慎用(诡异的用法, 无聊得很!!)
	//
	//15.key_compare key_comp();				//取出set 中所有的key, 作为副本, 用于比较(返回值可以当成是比较函数用, 但这个比较函数, 只比较值, 不比较迭代器)
	kcomp = x1.key_comp();
	it=x1.begin();
	printf("15 -- x1-kcomp: ");
	while(kcomp(*it++, *x1.rbegin()))		//大于比较对象才打印, 小于&等于都不打印!!
		printf("%d  ", *it);
	printf("\n");



	//16.value_compare value_comp();		//取出set 中所有的value, 作为副本, 用于比较; 与key_compare key_comp(); 相同, (一样的功能)
	vcomp = x1.value_comp();
	it=x1.begin();
	printf("16 -- x1-vcomp: ");
	while(vcomp(*it++, *x1.rbegin()))		//大于比较对象才打印, 小于&等于都不打印!!
		printf("%d  ", *it);
	printf("\n");



	return 0;
}


