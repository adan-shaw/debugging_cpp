//编译:
//		g++ -std=c++11 -g3 ./unordered_multimap.cpp -o x 



//unordered_multimap api list
/*
	成员函数
		(构造函数)					构造 unordered_multimap
		(析构函数)					析构 unordered_multimap
		operator=					赋值给容器
		get_allocator			返回相关的分配器

	迭代器
		begin							返回指向容器第一个元素的迭代器
		cbegin
		end								返回指向容器尾端的迭代器
		cend

	容器
		empty							检查容器是否为空
		size							返回容纳的元素数
		max_size					返回可容纳的最大元素数

	修改器
		clear							清除内容
		insert						插入元素或结点 (c++17 起)
		emplace						原位构造元素
		emplace_hint			使用提示原位构造元素
		erase							擦除元素
		swap							交换内容
		extract						从另一容器释出结点(c++17)
		merge							从另一容器接合结点(c++17)

	查找
		count							返回匹配特定键的元素数量
		find							寻找带有特定键的元素
		contains					检查容器是否含有带特定关键的元素(c++20)
		equal_range				返回匹配特定键的元素范围

	桶接口
		begin(size_type)	返回一个迭代器，指向指定的桶的开始
		cbegin(size_type)
		end(size_type)		返回一个迭代器，指向指定的桶的末尾
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
		operator==				比较 unordered_multimap 中的值
		operator!=
		std::swap(std::unordered_multimap)	特化 std::swap 算法(c++11)
		erase_if(std::unordered_multimap)		擦除所有满足特定判别标准的元素(c++20)
*/



#include <cstdio>
#include <cassert>
#include <unordered_map>//unordered_multimap 同样定义在<unordered_map> 中

using namespace std;



int main(void){
	int b_count,tmp;
	//1.创建unordered_multimap
	unordered_multimap<int,int> x1;
	unordered_multimap<int,int> x2 { {1,1},{2,10},{3,100},{4,1000}, };
	unordered_multimap<int,int> x3(x2.begin(), x2.end());		//iterator 构造copy
	unordered_multimap<int,int>::iterator it,itlow,ithigh;	//顺向迭代器
	unordered_multimap<int,int>::hasher fn;									//hash 函数
	unordered_multimap<int,int>::key_equal eq;							//key 值比较函数
																													//pair一对变量
	pair<unordered_multimap<int,int>::iterator, unordered_multimap<int,int>::iterator> hash_map_pair_range;


	//2.void clear();																				//清空unordered_multimap
	x1.clear();
	x3.clear();



	//3.bool empty();																				//检查unordered_multimap 是否为空
	if(x1.empty())
		printf("3 -- x1 unordered_multimap is now empty\n");
	if(x2.empty())
		printf("3 -- x2 unordered_multimap is now empty\n");
	if(x3.empty())
		printf("3 -- x3 unordered_multimap is now empty\n");



	//4.size_type size();																		//返回unordered_multimap 的size()
	printf("4 -- x2 unordered_multimap size()=%d\n", x2.size());



	//5.顺向迭代器
	//iterator begin();																			//返回unordered_multimap 的顺向front() 迭代器
	//iterator end();																				//返回unordered_multimap 的顺向end() 迭代器
	printf("5 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");

	//6.(没有)逆向迭代器



	//7.删除元素--三种删除方式
	//void erase(iterator position);																					//利用迭代器进行删除(单个)
	//size_type erase(const key_type& x);																			//输入'常量值'进行删除(值不存在就不删除,也不会失败)
	//void erase(iterator first, iterator last);															//利用迭代器进行区域删除(多个)
	assert(!x2.empty());
	x2.erase(x2.begin());																		//迭代器单个删除

	assert(!x2.empty());
	x2.erase(30);																						//输入'常量值'删除(存在则删除, 不存在则忽略)

	assert(!x2.empty());
	it=x2.find(3);																					//迭代器区域删除(区域内, 圈中则删除, 没有圈中则忽略)
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

	it = x2.insert(pair<int,int>(0,10));																			//插入方式2: pair() + insert()
	it = x2.insert(pair<int,int>(1,20));
	it = x2.insert(pair<int,int>(2,30));
	it = x2.insert(pair<int,int>(3,40));
	it = x2.insert(pair<int,int>(4,50));

	//x2[5] = 60;																															//插入方式3: '模仿'数组插入语法方式(key 值不是唯一的, 没办法支持这种插入方式)
	//x2[6] = 70;
	//x2["name"] = "adan";																										//假如是字符串, 可以这样操作

	it = x2.insert(unordered_multimap<int, int>::value_type(7, 80));					//插入方式4: value_type() + insert()
	it = x2.insert(unordered_multimap<int, int>::value_type(8, 90));

	it = x2.emplace(pair<int,int>(99,1230));																	//插入方式5: emplace 返回判断结果, 查看是否插入, 还是unordered_multimap 中已存在该值
	printf("%d, %d\n", (*it).first, (*it).second);

	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("[%d,%d]  ", (*it).first,(*it).second);
	printf("\n");



	//9.void swap(unordered_multimap<Key,Compare,Allocator>& st);							//unordered_multimap 之间交换元素
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



	//13.pair<iterator,iterator> equal_range(const key_type& x)const;					//'范围区域'搜索值, 找到则返回首个指向该值的迭代器;(多值容器常用, 可以知道有多少个相同的值, 唯一键容器少用)
	/*
	hash_map_pair_range = x1.equal_range(500);																//超出范围的值搜索, 会直接等于end(), 表示被搜索的值不存在
	assert(hash_map_pair_range.first == x1.end());

	hash_map_pair_range = x1.equal_range(223);																//刚好搜索到最后一个值, hash_map_pair_range.second == x1.end()
	assert(hash_map_pair_range.second == x1.end());

	assert(hash_map_pair_range.first != hash_map_pair_range.second);					//hash_map_pair_range.first 永远不等于 hash_map_pair_range.second 
	*/
	hash_map_pair_range = x1.equal_range(4);
	if(hash_map_pair_range.first != x1.end())
		for(; hash_map_pair_range.first != hash_map_pair_range.second; hash_map_pair_range.first++)
			printf("13 -- x1.equal_range(4)=%d [pair.second=%d]\n", *hash_map_pair_range.first, *hash_map_pair_range.second);



	//14.删除范围区域中的元素(没有范围搜索函数, 无法遍历, 无法修改值, 因此也没办法做'范围区域的元素'删除)



	//15.桶接口
	//	 size_type bucket_count() const;																			//返回桶数
	printf("15 -- x1.bucket_count=%d\n", x1.bucket_count());

	//	 size_type max_bucket_count() const;																	//返回桶的最大数量
	printf("15 -- x1.max_bucket_count=%d\n", x1.max_bucket_count());

	//	 size_type bucket_size(size_type n) const;														//返回在特定的桶中的元素数量
	b_count = x1.bucket_count();																							//固定用法: 先求出桶的count数量, 然后遍历所有桶, 桶的下标从0 开始;
	for(tmp=0; tmp<b_count; tmp++)
		printf("15 -- x1.bucket_size(%d)首个桶的元素个数=%d\n", tmp, x1.bucket_size(tmp));

	//	 size_type bucket(const Key& key) const;															//输入指定key 值, 返回key 值所在的桶号(根据值, 索引桶号)
	printf("15 -- x1.bucket(20): 指定的key值=20, 指定key所在的桶号=%d\n", x1.bucket(20));



	//16.哈希策略
	//	 float load_factor() const;																						//返回每个桶的平均元素数量
	printf("16 -- x1.load_factor()目前每个桶存放的元素个数的平均值(结果为浮点数)=%f\n", x1.load_factor());

	//	 float max_load_factor() const;																				//查询每个桶的平均元素数量的最大值
	printf("16 -- x1.max_load_factor()每个桶存放的元素个数的最大值(结果为浮点数)=%f\n", x1.max_load_factor());

	//	 void max_load_factor(float ml);																			//管理(设置)每个桶的平均元素数量的最大值
	x1.max_load_factor(11);
	printf("16 -- x1.max_load_factor()每个桶存放的元素个数的最大值(结果为浮点数)=%f[强制修改后的结果]\n", x1.max_load_factor());

	//	 void rehash(size_type count);																				//指定预留多少个桶的存储空间(这会重新生成哈希表)
	x1.rehash(1);

	//	 void reserve(size_type count);																				//指定预留多少个元素空位(这会重新生成哈希表)
	x1.reserve(11);

	//由11 个hash 桶, 每个hash 桶存放一个元素, 改变为: 一个hash 桶存放11 个元素
	x1.rehash(1);
	x1.reserve(11);



	//17.观察器
	//	 hasher hash_function() const;																				//获取哈希的函数(自定义hash函数略, 在另外一个文件上); ps: 获取hash 函数没什么用, 自定义hash函数较为复杂, 高级用法;
	fn = x1.hash_function();

	//	 key_equal key_eq() const;																						//获取比较函数(如果值类型比较复杂, 需要自定义key 值比较函数, 用做find 查找时, 匹配具体的key 值, 高级用法)
	eq = x1.key_eq();


	return 0;
}
