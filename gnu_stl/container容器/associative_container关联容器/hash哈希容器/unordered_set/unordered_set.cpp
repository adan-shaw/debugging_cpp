//编译:
//		g++ -std=c++11 -g3 ./unordered_set.cpp -w -o x 



//unordered_set api list
/*
	成员函数
		(构造函数)					构造 unordered_set
		(析构函数)					析构 unordered_set
		operator=					赋值给容器
		get_allocator			返回相关的分配器

	迭代器
		begin							返回指向容器第一个元素的迭代器
		cbegin
		end								返回指向容器尾端的迭代器
		cend

	容量
		empty							检查容器是否为空
		size							返回容纳的元素数
		max_size					返回可容纳的最大元素数

	修改器
		clear							清除内容
		insert						插入元素或结点 (C++17 起)
		emplace						原位构造元素
		emplace_hint			使用提示原位构造元素
		erase							擦除元素
		swap							交换内容
		extract						从另一容器释出结点(C++17)
		merge							从另一容器接合结点(C++17)

	查找
		count							返回匹配特定键的元素数量
		find							寻找带有特定键的元素
		contains					检查容器是否含有带特定关键的元素(C++20)
		equal_range				返回匹配特定键的元素范围

	桶接口
		begin(size_type)	返回一个迭代器, 指向指定的桶的开始
		cbegin(size_type)
		end(size_type)		返回一个迭代器, 指向指定的桶的末尾
		cend(size_type)
		bucket_count			返回桶数
		max_bucket_count	返回桶的最大数量
		bucket_size				返回在特定的桶中的元素数量
		bucket						返回带有特定键的桶

	哈希策略
		load_factor				返回每个桶的平均元素数量
		max_load_factor		管理每个桶的平均元素数量的最大值
		rehash						为至少为指定数量的桶预留存储空间(这会重新生成哈希表)
		reserve						为至少为指定数量的元素预留存储空间(这会重新生成哈希表)

	观察器
		hash_function			返回用于对关键哈希的函数
		key_eq						返回用于比较键的相等性的函数

	非成员函数
		operator==				比较 unordered_set 中的值
		operator!=
		std::swap(std::unordered_set)		特化 std::swap 算法(C++11)
		erase_if(std::unordered_set)		擦除所有满足特定判别标准的元素(C++20)
*/



#include <cstdio>
#include <cassert>
#include <unordered_set>
//#include<functional>

using namespace std;



int main(void){
	int b_count,tmp;
	int arr[]= {5,10,20,30,40,50,60};
	//1.创建unordered_set
	unordered_set<int> x1;												//empty unordered_set of ints
	unordered_set<int> x2(arr,arr+7);							//指针拷贝[起始地址, (起始地址+7)=结束地址]
	unordered_set<int> x3(x2.begin(), x2.end());	//iterator 构造copy
	unordered_set<int>::iterator it,itlow,ithigh;	//顺向迭代器
	unordered_set<int>::hasher fn;								//hash 函数
	unordered_set<int>::key_equal eq;							//key 值比较函数
																								//pair一对变量
	pair<unordered_set<int>::iterator, bool> hash_set_pair_val;
	pair<unordered_set<int>::iterator, unordered_set<int>::iterator> hash_set_pair_range;

	printf("1 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("1 -- x3:");
	for(it = x3.begin(); it != x3.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//2.void clear();															//清空unordered_set
	x1.clear();
	x3.clear();



	//3.bool empty();															//检查unordered_set 是否为空
	if(x1.empty())
		printf("3 -- x1 unordered_set is now empty\n");
	if(x2.empty())
		printf("3 -- x2 unordered_set is now empty\n");
	if(x3.empty())
		printf("3 -- x3 unordered_set is now empty\n");



	//4.size_type size();													//返回unordered_set 的size()
	printf("4 -- x2 unordered_set size()=%d\n", x2.size());



	//5.顺向迭代器
	//iterator begin();														//返回unordered_set 的顺向front() 迭代器
	//iterator end();															//返回unordered_set 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//6.(没有)逆向迭代器



	//7.删除元素--三种删除方式
	//void erase(iterator position);							//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);					//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);	//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());													//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);																	//输入'常量值'删除(存在则删除, 不存在则忽略)

	assert(!x2.empty());
	it=x2.find(60);																//迭代器区域删除(区域内, 圈中则删除, 没有圈中则忽略)
	if(it != x2.end())
		x2.erase(it, x2.end());

	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//8.插入新元素(有2种方式)
	//std::pair<iterator,bool> insert(const value_type& x);					//直接插入常量, 自动排序, 插入位置不确定, 返回pair 对;
	//iterator insert(iterator position, const value_type& x);			//根据迭代器插入, 自动排序, 插入位置确定, 返回迭代器;
	//std::pair<iterator,bool> emplace( Args&&... args );						//尝试性插入, 返回pair<iterator,bool>; 
																																	//有插入, bool=true, 并返回迭代器; 无插入, bool=false, 表明元素已经存在, 返回迭代器;
	//hash_set_pair_val = x2.insert(pair<unordered_set<int>::iterator,bool>(123));	//失败, 不能像map 那样用, 不能直接插入pair 对;
	//hash_set_pair_val = x2.insert(int,bool>(123));
	hash_set_pair_val = x2.insert(123);															//插入方式1: 直接插入常量
	hash_set_pair_val = x2.insert(123);
	printf("%d, %d\n", *hash_set_pair_val.first, hash_set_pair_val.second);

	x2.insert(x2.begin(), 223);																			//插入方式2: 根据迭代器插入

	hash_set_pair_val = x2.emplace(123);														//插入方式3: 尝试性插入
	printf("%d, %d\n", *hash_set_pair_val.first, hash_set_pair_val.second);

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.void swap(unordered_set<Key,Compare,Allocator>& st);				//两个unordered_set之间交换元素
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
		printf("11 -- x1.find(123) failed !!\n");



	//13.pair<iterator,iterator> equal_range(const key_type& x)const;//'范围区域'搜索值, 找到则返回首个指向该值的迭代器;(多值容器常用, 可以知道有多少个相同的值, 唯一键容器少用)
	/*
	hash_set_pair_range = x1.equal_range(500);											//超出范围的值搜索, 会直接等于end(), 表示被搜索的值不存在
	assert(hash_set_pair_range.first == x1.end());

	hash_set_pair_range = x1.equal_range(223);											//刚好搜索到最后一个值, hash_set_pair_range.second == x1.end()
	assert(hash_set_pair_range.second == x1.end());

	assert(hash_set_pair_range.first != hash_set_pair_range.second);//hash_set_pair_range.first 永远不等于 hash_set_pair_range.second 
	*/
	hash_set_pair_range = x1.equal_range(20);												//在key 唯一的容器中, 找不到会返回'范围区域', 或者end()迭代器; 在ket 不是唯一的场合, 用得比较多;
	if(hash_set_pair_range.first != x1.end())
		for(; hash_set_pair_range.first != hash_set_pair_range.second; hash_set_pair_range.first++) 
			printf("13 -- x1.equal_range(20)=%d\n", *hash_set_pair_range.first);



	//14.删除范围区域中的元素(没有范围搜索函数, 无法遍历, 无法修改值, 因此也没办法做'范围区域的元素'删除)



	//15.桶接口
	//	 size_type bucket_count() const;														//返回桶数
	printf("15 -- x1.bucket_count=%d\n", x1.bucket_count());

	//	 size_type max_bucket_count() const;												//返回桶的最大数量
	printf("15 -- x1.max_bucket_count=%d\n", x1.max_bucket_count());

	//	 size_type bucket_size(size_type n) const;									//返回在特定的桶中的元素数量
	b_count = x1.bucket_count();																		//固定用法: 先求出桶的count数量, 然后遍历所有桶, 桶的下标从0 开始;
	for(tmp=0; tmp<b_count; tmp++)
		printf("15 -- x1.bucket_size(%d)首个桶的元素个数=%d\n", tmp, x1.bucket_size(tmp));

	//	 size_type bucket(const Key& key) const;										//输入指定key 值, 返回key 值所在的桶号(根据值, 索引桶号)
	printf("15 -- x1.bucket(20): 指定的key值=20, 指定key所在的桶号=%d\n", x1.bucket(20));



	//16.哈希策略
	//	 float load_factor() const;																	//返回每个桶的平均元素数量
	printf("16 -- x1.load_factor()目前每个桶存放的元素个数的平均值(结果为浮点数)=%f\n", x1.load_factor());

	//	 float max_load_factor() const;															//查询每个桶的平均元素数量的最大值
	printf("16 -- x1.max_load_factor()每个桶存放的元素个数的最大值(结果为浮点数)=%f\n", x1.max_load_factor());

	//	 void max_load_factor(float ml);														//管理(设置)每个桶的平均元素数量的最大值
	x1.max_load_factor(11);
	printf("16 -- x1.max_load_factor()每个桶存放的元素个数的最大值(结果为浮点数)=%f[强制修改后的结果]\n", x1.max_load_factor());

	//	 void rehash(size_type count);															//指定预留多少个桶的存储空间(这会重新生成哈希表)
	x1.rehash(1);

	//	 void reserve(size_type count);															//指定预留多少个元素空位(这会重新生成哈希表)
	x1.reserve(11);

	//由11 个hash 桶, 每个hash 桶存放一个元素, 改变为: 一个hash 桶存放11 个元素
	x1.rehash(1);
	x1.reserve(11);



	//17.观察器
	//	 hasher hash_function() const;															//获取哈希的函数(自定义hash函数略, 在另外一个文件上); ps: 获取hash 函数没什么用, 自定义hash函数较为复杂, 高级用法;
	fn = x1.hash_function();

	//	 key_equal key_eq() const;																	//获取比较函数(如果值类型比较复杂, 需要自定义key 值比较函数, 用做find 查找时, 匹配具体的key 值, 高级用法)
	eq = x1.key_eq();



	return 0;
}



