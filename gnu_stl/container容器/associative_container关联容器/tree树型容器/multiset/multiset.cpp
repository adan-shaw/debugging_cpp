//编译:
//		g++ -std=c++11 -g3 ./multiset.cpp -o x 



//multiset apt list
/*
	成员函数
		(构造函数)					构造 multiset
		(析构函数)					析构 multiset
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
		operator==				按照字典顺序比较 multiset 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::multiset)	特化 std::swap 算法
		erase_if(std::multiset)		擦除所有满足特定判别标准的元素(C++20)
*/



#include <cstdio>
#include <cassert>
#include <set>//multiset 同样声明在<set>
//#include<functional>

using namespace std;



int main(void){
	int tmp;
	int arr[]= {10,20,30,40,50};
	//1.创建multiset
	multiset<int> x1;												//empty multiset of ints
	multiset<int> x2(arr,arr+5);						//指针拷贝[起始地址, (起始地址+5)=结束地址]
	multiset<int,less<int>> x3(x2);					//a copy of x2, 重写排序比较函数为less<int>升序, 从小到大; greater<int>降序不能用, 不知道为什么!!
	multiset<int> x4(x2.begin(), x2.end());	//iterator 构造copy
	multiset<int>::iterator it,itlow,ithigh;//顺向迭代器
	multiset<int>::reverse_iterator rit;		//逆向迭代器
	multiset<int>::key_compare kcomp;				//key 比较对象的副本
	multiset<int>::value_compare vcomp;			//value 比较对象的副本
																					//pair一对变量
	//pair<multiset<int>::iterator, bool> multiset_pair_val;
	pair<multiset<int>::iterator, multiset<int>::iterator> multiset_pair_equal_range;

	printf("1 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("1 -- x3:");
	for(it = x3.begin(); it != x3.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//2.void clear();												//清空multiset
	x1.clear();
	x3.clear();



	//3.bool empty();												//检查multiset 是否为空
	if(x1.empty())
		printf("3 -- x1 multiset is now empty\n");
	if(x2.empty())
		printf("3 -- x2 multiset is now empty\n");
	if(x3.empty())
		printf("3 -- x3 multiset is now empty\n");



	//4.size_type size();										//返回multiset 的size()
	printf("4 -- x2 multiset size()=%d\n", x2.size());



	//5.顺向迭代器遍历
	//iterator begin();											//返回multiset 的顺向front() 迭代器
	//iterator end();												//返回multiset 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//6.逆向迭代器遍历
	//iterator rbegin();										//返回multiset 的逆向front() 迭代器
	//iterator rend();											//返回multiset 的逆向end() 迭代器
	printf("6 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");



	//7.删除元素--三种删除方式
	//void erase(iterator position);																//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);														//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);										//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());										//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);														//输入'常量值'删除(存在则删除, 不存在则忽略)

	assert(!x2.empty());
	it=x2.find(40);													//迭代器区域删除(区域内, 圈中则删除, 没有圈中则忽略)
	if(it != x2.end())
		x2.erase(it, x2.end());

	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//8.插入新元素(有3种方式, 与set不同的是, set insert/emplace 返回std::pair<iterator,bool> 对变量; multiset 则返回multiset<int>::iterator 迭代器)
	//iterator insert(const value_type& x);													//直接插入常量, 自动排序, 插入位置不确定, 返回pair 对
	//iterator insert(iterator position, const value_type& x);			//根据迭代器插入, 自动排序, 插入位置确定, 返回迭代器
	//iterator emplace( Args&&... args );														//尝试性插入, 返回pair<iterator,bool>; 
																																	//有插入, bool=true, 并返回迭代器; 无插入, bool=false, 表明元素已经存在, 返回迭代器;
	//it = x2.insert(pair<set<int>::iterator,bool>(123));						//失败, 不能像map 那样用, 不能直接插入pair 对;
	//it = x2.insert(int,bool>(123));
	it = x2.insert(123);																						//插入方式1: 直接插入常量
	it = x2.insert(123);
	printf("%d\n", *it);

	x2.insert(x2.begin(), 223);																			//插入方式2: 根据迭代器插入

	it = x2.emplace(123);																						//插入方式3: 尝试性插入
	printf("%d\n", *it);

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.void swap(multiset<Key,Compare,Allocator>& st);							//两个multiset之间交换元素
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



	//12.iterator lower_bound(const key_type& x);										//返回首个不小于'给定key值x'的元素的迭代器('给定key值x'不一定需要存在于multiset中, 等于也算是不小于)
	it = x1.lower_bound(25);
	if(it != x1.end())
		printf("12 -- lower_bound(25)=存在'不小于25 的key值', 这个key值是: %d\n", *it);
	else
		printf("12 -- lower_bound(25)=不存在'不小于25 的key值'\n");

	//	 iterator upper_bound(const key_type& x);										//返回首个大于'给定key值x'的元素的迭代器
	it = x1.upper_bound(25);
	if(it != x1.end())
		printf("12 -- upper_bound(25)=存在'大于25 的key值', 这个key值是: %d\n", *it);
	else
		printf("12 -- upper_bound(25)=不存在'大于25 的key值'\n");



	//13.pair<iterator,iterator> equal_range(const key_type& x)const;//返回'范围区域'组成的迭代器pair对, 等价于pair(lower_bound(), upper_bound()); ps: multiset 集合是有序的
	multiset_pair_equal_range = x1.equal_range(25);
	printf("13 -- multiset->equal_range(25): [%d, %d]\n", *multiset_pair_equal_range.first, *multiset_pair_equal_range.second);



	//14.删除范围区域中的元素
	for(tmp=1; tmp<10; tmp++)
		x4.insert(tmp*10);										// 10 20 30 40 50 60 70 80 90
	itlow=x4.lower_bound(30);								//			 ^
	ithigh=x4.upper_bound(60);							//								^
	x4.erase(itlow,ithigh);									// 10 20 70 80 90

	printf("14 -- x4:");
	for(it = x4.begin(); it != x4.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//
	// multiset 观察器: 功能很鸡肋, 不知道有何作用, 还不如普通的迭代器遍历, 慎用(诡异的用法, 无聊得很!!)
	//
	//15.key_compare key_comp();						//取出multiset 中所有的key, 作为副本, 用于比较(返回值可以当成是比较函数用, 但这个比较函数, 只比较值, 不比较迭代器)
	kcomp = x1.key_comp();
	it=x1.begin();
	tmp=*x1.rbegin();
	printf("15 -- x1-kcomp: ");
	while(kcomp(*it++, tmp))								//大于比较对象才打印, 小于&等于都不打印!!
		printf("%d  ", *it);
	printf("\n");

	//16.value_compare value_comp();				//取出multiset 中所有的value, 作为副本, 用于比较; 与key_compare key_comp(); 相同, (一样的功能)
	vcomp = x1.value_comp();
	it=x1.begin();
	tmp=*x1.rbegin();
	printf("16 -- x1-vcomp: ");
	while(vcomp(*it++, tmp))								//大于比较对象才打印, 小于&等于都不打印!!
		printf("%d  ", *it);
	printf("\n");



	return 0;
}

