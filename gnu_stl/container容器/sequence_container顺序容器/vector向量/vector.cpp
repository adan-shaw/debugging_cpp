//编译:
//		g++ -std=c++11 -g3 ./vector.cpp -o x 



//vector api list
/*
	成员函数
		(构造函数)				构造vector
		(析构函数)				析构vector
		operator=				赋值给容器
		assign					将值赋给容器
		get_allocator		返回相关的分配器

	元素访问
		at							访问指定的元素,同时进行越界检查
		operator[]			访问指定的元素
		front						访问第一个元素
		back						访问最后一个元素
		data						返回指向内存中数组第一个元素的指针

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
		reserve					预留存储空间
		capacity				返回当前存储空间能够容纳的元素数
		shrink_to_fit		通过释放未使用的内存减少内存的使用(c++11)

	修改器
		clear						清除内容
		insert					插入元素
		emplace					原位构造元素(c++11)
		erase						擦除元素
		push_back				将元素添加到容器末尾
		emplace_back		在容器末尾就地构造元素(c++11)
		pop_back				移除末元素
		resize					改变容器中可存储元素的个数
		swap						交换内容

	非成员函数
		operator==			按照字典顺序比较 vector 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::vector)		特化std::swap 算法
		erase(std::vector)				擦除所有满足特定判别标准的元素(c++20)
		erase_if(std::vector)			(c++20)
*/



#include <cstdio>
#include <cassert>
#include <vector>
using namespace std;



int main(void){
	int tmp,size;
	//1.创建空的'vector向量'
	vector<int> x1;											// empty vector of ints
	vector<int> x2(4,100);							// four ints with value 100
	vector<int> x3(x2.begin(),x2.end());// iterating through x2(极度不推荐这样拷贝, 性能地狱)
	vector<int> x4(x3);									// a copy of x3
	vector<int>::iterator it;
	vector<int>::reverse_iterator rit;



	//2.void clear();//清空'vector向量'
	x1.clear();
	x3.clear();



	//3.bool empty();//判断'vector向量'是否为空
	if(x1.empty())
		printf("3 -- x1 vector is now empty\n");
	if(x2.empty())
		printf("3 -- x2 vector is now empty\n");
	if(x3.empty())
		printf("3 -- x3 vector is now empty\n");

	//it迭代器判断'vector向量'是否为空(后续有大用!!)
	if(x1.begin() == x1.end())
		printf("3 -- x1 vector is now (x1.begin() == x1.end())\n");
	if(x2.begin() == x2.end())
		printf("3 -- x2 vector is now (x2.begin() == x2.end())\n");



	//4.size_type size();//返回'vector向量'的size()
	printf("4 -- x2 vector size()=%d\n", x2.size());



	//5.void push_back(const T& x);//向'vector向量'尾部压入元素
	x2.push_back(321);
	x2.push_back(123);



	//6.遍历方式1: 随机访问式遍历, 必须知道'vector向量'非空, 否则访问到空'vector向量', 会触发内存访问越界;
	//6.1: at() 遍历(一旦内存访问越界, at()会有更多报错信息)
	printf("6.1 -- x2:");
	assert(!x2.empty());
	for(tmp=0,size=x2.size(); tmp<size; tmp++)
		printf("at(%d)=%d  ", tmp, x2.at(tmp));
	printf("\n");
	//6.2: 下标访问遍历
	printf("6.2 -- x2:");
	assert(!x2.empty());
	for(tmp=0,size=x2.size(); tmp<size; tmp++)
		printf("x2[%d] = %d  ", tmp, x2[tmp]);
	printf("\n");



	//7.遍历方式2: 迭代器遍历(好处: 永远不会崩溃, 即便访问到空'vector向量', 迭代器也安全)
	//7.1: 顺向迭代器(故意访问empty空'vector向量', 证明迭代器的安全性)
	printf("7.1 -- x1:");
	for(it=x1.begin(); it!=x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//7.2: 逆向迭代器
	printf("7.2 -- x2:");
	for(rit=x2.rbegin(); rit!=x2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");



	//8.iterator erase(iterator position);//通过it迭代器删除元素(不能删除空元素!!)
	//	(该删除元素操作, 会触发'vector向量'元素整体向前移动n 位, 删除n 个, 移动n 位)
	//erase() 删除单个元素
	it = x1.begin();	//(假赋值, 不用太在意, 只是想取一个指向x1 的迭代器而已)
	if(it != x1.end())//安全使用erase() 删除'vector向量'的元素
		x1.erase(it);

	//erase() + it迭代器遍历, 删除多个元素(非连续, if筛选式删除, 好用)
	for(it=x1.begin(); it!=x1.end(); *it++)
		if(*it%2 == 0)
			x1.erase(it);

	//erase() 删除多个连续元素(不安全, 两个迭代器的有效性, 必须自己保证)
	if((x2.begin()+3) <= x2.end())//最好的保障手段: 保障最后一个连续元素, 不超出x2.end()
		x2.erase(x2.begin()+1,x2.begin()+3);

	//打印剩余的123,321
	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.void pop_back(void);//从尾部弹出元素
	x1.pop_back();					//对空'vector向量'进行pop_back(), 会得到x1.size() = -1, 但此时'vector向量'已坏!! 再使用即崩溃!!
	printf("9 -- x1.size()=%d\n",x1.size());

	x1.pop_back();					//对已损坏的'vector向量', 可以继续pop_back();
	//x1.push_back(111);		//对已损坏的'vector向量'push_back()元素, 会崩溃;
	//x1[1] == 111;					//对已损坏的'vector向量', 任何形式的push 操作, 都会崩溃, 包括随机访问push;
	//x1.at(0) == 111;
	x1.clear();							//clear() 可以修复已损坏的'vector向量'
	x1.push_back(123);
	printf("9 -- x1.size()=%d\n",x1.size());

	//void pop_front(void);	//从头部弹出元素
	/*
	if(!x1.empty())					//正确的弹出元素操作(低效率)
		x1.pop_back();
	*/
	assert(!x1.empty());		//正确的弹出元素操作(高效率, 减少if的使用)
	x1.pop_back();



	//10.reference front();	//返回stl容器'已申请的内存空间地址'中第一个元素;
	//	 reference back();	//返回stl容器'已申请的内存空间地址'中最后一个元素;
	//	 *vec.begin()				//返回stl容器'现存的元素'的第一个;
	//	 *vec.end()					//返回stl容器'现存的元素'的最后一个;
	printf("10 -- x1 front()=%d, back()=%d, x1.size()=%d; 只有一个元素时, 头等于尾.\n", x1.front(), x1.back(), x1.size());

	//访问越界, 报错(但不是内存越界, 只是at() 函数越界警告, at() 函数十分安全)
	//printf("10 -- x1 at(0)=%d, back()=%d, x1.size()=%d; 只有一个元素时, 头等于尾.\n", x1.at(0), x1.back(), x1.size());

	printf("10 -- x1 *x1.begin()=%d, *x1.end()=%d, x1.size()=%d; 只有一个元素时, 头等于尾.\n", *x1.begin(), *x1.end(), x1.size());
	if(x1.begin() == x1.end())
		printf("10 -- x1 vector is now (x1.begin() == x1.end())\n");



	//11.void resize(size_type sz, T c = T());//重新修改'vector向量'的容量, 一般用来扩容, 注意: 该功能不能缩容;
	x2.resize(0);
	printf("11 -- x2 vector size()=%d\n", x2.size());
	if(x2.empty())
		printf("11 -- x2 vector is now empty\n");


	x2.resize(10,123);//重新创建10个元素, 每个元素都是123
	printf("11 -- x2 vector size()=%d\n", x2.size());
	if(x2.empty())
		printf("11 -- x2 vector is now empty\n");



	//12.void shrink_to_fit(void);//'vector向量'强制缩容(唯一的缩容操作)
	x4.resize(100,99);
	printf("12 -- x4 vector capacity is %d\n", x4.capacity());
	x4.clear();
	x4.shrink_to_fit();
	printf("12 -- x4 vector capacity is %d\n", x4.capacity());



	//13.void swap(vector<T,Allocator>& vec);//将两个vector 的元素互换
	x1.swap(x2);//将x1 x2的所有元素互相交换

	printf("13 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("13 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	return 0;
}

