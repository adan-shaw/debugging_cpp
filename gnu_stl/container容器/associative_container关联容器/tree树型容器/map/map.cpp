//编译:
//		g++ -std=c++11 -g3 ./map.cpp -w -o x 



//map api list
/*
	成员类
		value_compare			比较类型为value_type的对象(类)

	成员函数
		(构造函数)					构造 map
		(析构函数)					析构 map
		operator=					赋值给容器
		get_allocator			返回相关的分配器

	元素访问
		at								访问指定的元素,同时进行越界检查(c++11)
		operator[]				访问或插入指定的元素

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
		insert						插入元素或结点 (c++17 起)
		insert_or_assign	插入元素,或若关键已存在则赋值给当前元素(c++17)
		emplace						原位构造元素(c++11)
		emplace_hint			使用提示原位构造元素(c++11)
		try_emplace				若键不存在则原位插入,若键存在则不做任何事(c++17)
		erase							擦除元素
		swap							交换内容
		extract						从另一容器释出结点(c++17)
		merge							从另一容器接合结点(c++17)

	查找
		count							返回匹配特定键的元素数量
		find							寻找带有特定键的元素
		contains					检查容器是否含有带特定关键的元素(c++20)
		equal_range				返回匹配特定键的元素范围
		lower_bound				返回指向首个不小于给定键的元素的迭代器
		upper_bound				返回指向首个大于给定键的元素的迭代器

	观察器
		key_comp					返回用于比较键的函数
		value_comp				返回用于在value_type类型的对象中比较键的函数;

	非成员函数
		operator==				按照字典顺序比较 map 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::map)		特化 std::swap 算法
		erase_if(std::map)		擦除所有满足特定判别标准的元素(c++20)
*/



#include <cstdio>
#include <cassert>
#include <map>

using namespace std;



int main(void){
	int tmp;
	//1.创建map
	map<int,int> x1;
	map<int,int> x2 { {1,1},{2,10},{3,100},{4,1000}, };
	map<int,int,less<int>> x3(x2);					//a copy of x2, 重写排序比较函数为less<int>升序, 从小到大; greater<int>降序不能用, 不知道为什么!!
	map<int,int> x4(x2.begin(), x2.end());	//iterator 构造copy
	map<int,int>::iterator it,itlow,ithigh;	//顺向迭代器
	map<int,int>::reverse_iterator rit;			//逆向迭代器
	map<int,int>::key_compare kcomp;				//key 比较对象的副本
	//map<int,int>::value_compare vcomp;		//value 比较对象的副本(gcc 没有这个对象, 十分诡异)
																					//pair一对变量
	pair<map<int,int>::iterator, bool> map_pair_val;
	pair<map<int,int>::iterator, map<int,int>::iterator> map_pair_range;


	//2.void clear();												//清空map
	x1.clear();
	x3.clear();



	//3.bool empty();												//检查map 是否为空
	if(x1.empty())
		printf("3 -- x1 map is now empty\n");
	if(x2.empty())
		printf("3 -- x2 map is now empty\n");
	if(x3.empty())
		printf("3 -- x3 map is now empty\n");



	//4.size_type size();										//返回map 的size()
	printf("4 -- x2 map size()=%d\n", x2.size());



	//5.顺向迭代器
	//iterator begin();											//返回map 的顺向front() 迭代器
	//iterator end();												//返回map 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");

	//6.逆向迭代器
	//iterator rbegin();										//返回map 的逆向front() 迭代器
	//iterator rend();											//返回map 的逆向end() 迭代器
	printf("6 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("[%d,%d]  ", (*rit).first, (*rit).second);
	printf("\n");



	//7.删除元素--三种删除方式
	//void erase(iterator position);																				//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);																		//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);														//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());										//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);														//输入'常量值'删除(存在则删除, 不存在则忽略)

	assert(!x2.empty());
	it=x2.find(3);													//迭代器区域删除(区域内, 圈中则删除, 没有圈中则忽略)
	if(it != x2.end())
		x2.erase(it, x2.end());

	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//8.插入新元素
	//std::pair<iterator,bool> insert( const value_type& value );							//pair一对值的'插入常量'
	//iterator insert( iterator hint, const value_type& value );							//迭代器'插入常量'
	//std::pair<iterator,bool> emplace( Args&&... args );											//尝试性插入, 返回pair<iterator,bool>; 
																																						//有插入, bool=true, 并返回迭代器; 无插入, bool=false, 表明元素已经存在, 返回迭代器;
	//pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);						//pair一对值的'插入key+value'(c++17)
	//iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);	//迭代器'插入key+value'(c++17)
	//pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);					//pair一对值的'尝试性插入key+value', 元素存在则不做任何操作(c++17)
	//iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);//迭代器'尝试性插入key+value', 元素存在则不做任何操作(c++17)

	it = x2.insert(x2.begin(), pair<int,int>(9,123));													//插入方式1: 迭代器 + pair() + insert()
	it = x2.insert(x2.begin(), pair<int,int>(9,65));

	map_pair_val = x2.insert(pair<int,int>(0,10));														//插入方式2: pair() + insert()
	map_pair_val = x2.insert(pair<int,int>(1,20));
	map_pair_val = x2.insert(pair<int,int>(2,30));
	map_pair_val = x2.insert(pair<int,int>(3,40));
	map_pair_val = x2.insert(pair<int,int>(4,50));

	x2[5] = 60;																																//插入方式3: '模仿'数组插入语法方式
	x2[6] = 70;
	//x2["name"] = "adan";									//假如是字符串, 可以这样操作

	map_pair_val = x2.insert(map<int, int>::value_type(7, 80));								//插入方式4: value_type() + insert()
	map_pair_val = x2.insert(map<int, int>::value_type(8, 90));

	map_pair_val = x2.emplace(pair<int,int>(99,1230));												//插入方式5: emplace 返回判断结果, 查看是否插入, 还是map 中已存在该值
	printf("%d, %d\n", *map_pair_val.first, map_pair_val.second);

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//9.void swap(map<Key,Compare,Allocator>& st);														//map 之间交换元素
	x1.swap(x2);

	printf("9 -- x2:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");

	printf("9 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//10.size_type count(cont key_type& x);																		//统计某个值元素的个数
	printf("10 -- x1, element '2' count=%d\n",x1.count(2));
	printf("10 -- x1, element '11' count=%d\n",x1.count(11));



	//11.iterator find(const key_type& x);																		//查找指定元素
	it=x1.find(4);
	if(it != x1.end())
		printf("11 -- x1.find(4) = [%d,%d]\n", (*it).first,(*it).second);
	else
		printf("11 -- x1.find(4) failed!!\n");



	//12.iterator lower_bound(const key_type& x);															//返回首个不小于'给定key值x'的元素的迭代器('给定key值x'不一定需要存在于set中, 等于也算是不小于)
	it = x1.lower_bound(25);
	if(it != x1.end())
		printf("12 -- lower_bound(25)=存在'不小于25 的key值', 这个key&value是: [%d,%d]\n", it->first, it->second);
	else
		printf("12 -- lower_bound(25)=不存在'不小于25 的key值'\n");

	//	 iterator upper_bound(const key_type& x);															//返回首个大于'给定key值x'的元素的迭代器
	it = x1.upper_bound(25);
	if(it != x1.end())
		printf("12 -- upper_bound(25)=存在'大于25 的key值', 这个key&value是: [%d,%d]\n", it->first, it->second);
	else
		printf("12 -- upper_bound(25)=不存在'大于25 的key值'\n");



	//13.pair<iterator,iterator> equal_range(const key_type& x)const;					//'范围区域'搜索值, 找到则返回首个指向该值的迭代器;(多值容器常用, 可以知道有多少个相同的值, 唯一键容器少用)
	/*
	map_pair_range = x1.equal_range(500);																			//超出范围的值搜索, 会直接等于end(), 表示被搜索的值不存在
	assert(map_pair_range.first == x1.end());

	map_pair_range = x1.equal_range(223);																			//刚好搜索到最后一个值, map_pair_range.second == x1.end()
	assert(map_pair_range.second == x1.end());

	assert(map_pair_range.first != map_pair_range.second);										//map_pair_range.first 永远不等于 map_pair_range.second 
	*/
	map_pair_range = x1.equal_range(4);
	if(map_pair_range.first != x1.end())
		for(; map_pair_range.first != map_pair_range.second; map_pair_range.first++)
			printf("13 -- x1.equal_range(4)=%d [pair.second=%d]\n", *map_pair_range.first, *map_pair_range.second);



	//14.删除范围区域中的元素
	x4.clear();
	for(tmp=0; tmp<10; tmp++)								// 1  2  3  4  5  6  7  8  9    key
		x4[tmp]=tmp*10;												// 10 20 30 40 50 60 70 80 90   value
	itlow=x4.lower_bound(3);								//			 ^
	ithigh=x4.upper_bound(6);								//									 ^
	x4.erase(itlow,ithigh);									// 10 20 70 80 90

	printf("14 -- x4:");
	for(it = x4.begin(); it != x4.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//
	// map 观察器: 功能很鸡肋, 不知道有何作用, 还不如普通的迭代器遍历, 慎用(诡异的用法, 无聊得很!!)
	//
	//15.key_compare key_comp();																							//取出map 中所有的key, 作为副本, 用于比较(返回值可以当成是比较函数用, 但这个比较函数, 只比较值, 不比较迭代器)
	kcomp = x1.key_comp();																										//(其实key_compare 取出后, 也包含map 中所有的value)

	//打印
	it=x1.begin();
	tmp=x1.rbegin()->first;
	printf("15 -- x1:");
	while(kcomp((*it++).first, tmp))				//大于比较对象才打印, 小于&等于都不打印!!
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	return 0;
}


