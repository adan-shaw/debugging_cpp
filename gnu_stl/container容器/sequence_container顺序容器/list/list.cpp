//编译:
//		g++ -std=c++11 -g3 ./list.cpp -o x 



//list api list
/*
	成员函数
		(构造函数)					构造 list
		(析构函数)					析构 list
		operator=					赋值给容器
		assign						将值赋给容器
		get_allocator			返回相关的分配器

	元素访问
		front							访问第一个元素
		back							访问最后一个元素

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
		insert						插入元素
		emplace						原位构造元素(C++11)
		erase							擦除元素
		push_back					将元素添加到容器末尾
		emplace_back			在容器末尾就地构造元素(C++11)
		pop_back					移除末元素
		push_front				插入元素到容器起始
		emplace_front			在容器头部就地构造元素(C++11)
		pop_front					移除首元素
		resize						改变容器中可存储元素的个数
		swap							交换内容

	操作
		merge							合并二个已排序列表
		splice						从另一个list中移动元素
		remove						移除满足特定标准的元素
		remove_if
		reverse						将该链表的所有元素的顺序反转
		unique						删除连续的重复元素
		sort							对元素进行排序

	非成员函数
		operator==				按照字典顺序比较 list 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::list)	特化 std::swap 算法
		erase(std::list)			擦除所有满足特定判别标准的元素(C++20)
		erase_if(std::list)
*/



#include <cstdio>
#include <list>
#include <cassert>
#include <algorithm>



using namespace std;



int main(void){
	int tmp;
	int arr_tmp[] = {16,2,77,29};
	//1.创建list
	list<int> x1;												//empty list of ints
	list<int> x2 (4,100);								//four ints with value 100
	list<int> x3 (x2.begin(),x2.end());	//iterating through x2
	list<int> x4 (x3);									//a copy of x3
	//the iterator constructor can also be used to construct from arrays:
	list<int> x5 (arr_tmp, arr_tmp + sizeof(arr_tmp)/sizeof(int));
	list<int>::iterator it;							//顺向迭代器(比较安全一点的方法)
	list<int>::reverse_iterator rit;		//逆向迭代器



	//2.void clear();//清空list(所有list 节点会被free()释放)
	x1.clear();
	x3.clear();
	//x1.pop_front();//测试list 是否可以对空list 进行pop()操作.
	//x3.pop_back();//测试结果: 内存溢出! 不能空list 进行pop()操作.



	//3.bool empty();//检查list 是否为空
	if(x1.empty())
		printf("3 -- x1 list is now empty\n");
	if(x2.empty())
		printf("3 -- x2 list is now empty\n");
	if(x3.empty())
		printf("3 -- x3 list is now empty\n");



	//4.size_type size();//返回list 的size()
	printf("4 -- x2 list size()=%d\n", x2.size());



	//5.push 的方式插入元素(list插入方法1)
	//void push_back(const T& x);//向list 尾部压入元素
	x2.push_back(321);

	//void push_front(const T& x);//向list 头部压入元素
	x2.push_front(123);



	//6.用迭代器+insert()的方式插入元素(list插入方法2)
	it = x2.begin();
	x2.insert(it,555);//从头部插入
	//rit = x2.rend();
	//x2.insert(rit,5555);//不能用反向迭代器操作!!

	it = x2.end();
	x2.insert(it,666);//从尾部插入
	//rit = x2.rbegin();
	//x2.insert(rit,6666);//不能用反向迭代器操作!!



	//7.迭代器遍历list
	//iterator begin();//返回list的顺向front()元素的迭代器
	//iterator end();//返回list的顺向end()元素的迭代器
	//顺向迭代器遍历
	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//iterator rbegin();//返回list的逆向front()元素的迭代器
	//iterator rend();//返回list的逆向end()元素的迭代器
	//逆向迭代器遍历
	printf("7 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");

	//利用std::find()函数做查找具体数值的位置, 返回迭代器
	tmp = 123;//查找目标值target
	//rit=std::find(x2.rbegin(),x2.rend(),tmp);
	it=find(x2.begin(),x2.end(),tmp);
	if(it != x2.end())
		printf("7 -- it=find(x2.begin(),x2.end(),%d); found it okay!\n",tmp);
	else
		printf("7 -- target-%d cant find\n",tmp);



	//8.list 元素访问典范demo:
	//iterator erase(iterator position);//通过迭代器删除元素
	/*
		由于list 不随机访问元素, 所以必须是'迭代器指针'访问和操作.
		因此:
			当你需要访问list迭代器后第n个元素时,可以用std::advance(*it,n)//list 迭代器*it向前推进n位;
			'访问下一个元素时', 你可以用std::next();//相当于++it1;
			'访问上一个元素时', 你可以用std::prev();//相当于--it2;
	*/

	//从list 后面插入元素[10-90]
	for(tmp=1; tmp<10; tmp++)
		x1.push_back(tmp*10);
                          // 10 20 30 40 50 60 70 80 90
	it1 = it2 = x1.begin(); // ^^
	std::advance(it2,6);    // ^                 ^
	++it1;                  //    ^              ^

	it1 = x1.erase(it1);    // 10 30 40 50 60 70 80 90
													//    ^           ^

	it2 = x1.erase(it2);    // 10 30 40 50 60 80 90
													//    ^           ^
	++it1;                  //       ^        ^
	--it2;                  //       ^     ^

	x1.erase(it1,it2);      // 10 30 60 80 90
													//       ^^

	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.弹出元素 && 直接访问头,尾元素
	//reference front();//直接访问第一个元素
	//reference back()//直接访问最后一个元素
	//void pop_back();//从尾部弹出元素
	//void pop_front();//从头部弹出元素
	assert(!x2.empty());
	x2.pop_back();

	assert(!x2.empty());
	x2.pop_front();

	//空队列的x2.front() == NULL == 0 !!(c/c++ 中, NULL == 0)
	assert(!x2.empty());
	printf("9 -- x2 front=%d,back=%d,空list 头,尾 == NULL!!\n", x2.front(), x2.back());

	x2.clear();
	if(x2.empty())
		printf("9 -- x2 list is now empty\n");//元素已经删空
	printf("9 -- x2 list size()=%d\n", x2.size());

	//对空list 错误操作测试
	//x2.pop_front();//list 由于是指针实现的, 绝对不能pop 空, 否则指针会溢出.
	//x2.pop_front();



	//10.void resize(size_type sz, T c = T());//重新修正list 的容量(多出来的list 节点会被free()释放)
	x2.resize(0);
	printf("10 -- x2 list size()=%d\n", x2.size());
	if(x2.empty())
		printf("10-- x2 list is now empty\n");

	x2.resize(10,123);//重新创建10个元素, 每个元素都是123
	printf("10 -- x2 list size()=%d\n", x2.size());
	if(x2.empty())
		printf("10 -- x2 list is now empty\n");



	//11.void sort();		//对元素进行排序处理(空list也可以排序)
	//	 void reverse();//对元素进行反序处理
	x2.sort();
	printf("11 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("11 -- 插入元素后, 再排序:\n");
	x2.push_back(11111);//插入元素后, 再排序
	x2.push_back(111);
	x2.push_front(11112);
	x2.sort();
	printf("11 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//12.void swap(list<T,Allocator>& lst);//将两个list 的元素互换
	printf("12 -- 将两个list 的元素互换\n");
	x1.swap(x2);//将x1 x2的所有元素互相交换

	printf("12 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("12 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//13.void unique();//去除list 中的'重复值'元素
	printf("13 -- 去除重复元素值测试\n");
	printf("13 -- before: x1 list size()=%d\n", x1.size());
	x1.unique();
	printf("13 -- after: x1 list size()=%d\n", x1.size());

	printf("13 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//14.根据删除条件, 删除符合条件的所有元素
	//void remove( const T& value );
	//void remove_if( UnaryPredicate p );
	x1.push_back(111);
	x1.remove(111);																//移除两个等于111的元素
	x1.remove_if([](int n){ return n < 11111; });	//移除全部小于11111的元素(内嵌钩子函数, 多重递归调用)
	printf("14 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//15.void splice(iterator position,list<T,Allocator>&x);//剪切一个list的所有元素,插入到另外一个List中,完成后清空被剪切的list
	printf("15 -- 剪切一个list的所有元素,插入到另外一个List中,完成后清空被剪切的list.\n");
	it = x1.begin();
	++it;
	x1.splice(it,x2);//将x2所有元素插入x1的第二顺位的元素之间, 并清空x2
	if(x2.empty())
		printf("15 -- x2 list is now empty\n");

	printf("15 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//16.void merge( list& other );//合并两个list, 不会自动排序, 但会清空其中一个list
	x2.merge(x1);
	//x2.merge(x1, [](int n){ return n < 11111; });//不能植入筛选钩子函数?
	printf("16 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("16 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	return 0;
}


