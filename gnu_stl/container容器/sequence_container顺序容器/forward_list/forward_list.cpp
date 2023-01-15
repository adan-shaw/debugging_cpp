//编译:
//		g++ -std=c++11 -g3 ./forward_list.cpp -o x 



//forward_list api list
/*
	成员函数
		(构造函数)					构造 forward_list
		(析构函数)					析构 forward_list
		operator=					赋值给容器
		assign						将值赋给容器
		get_allocator			返回相关的分配器

	元素访问
		front							访问第一个元素

	迭代器
		before_begin			返回指向第一个元素之前迭代器
		cbefore_begin
		begin							返回指向容器第一个元素的迭代器
		cbegin
		end								返回指向容器尾端的迭代器
		cend

	容量
		empty							检查容器是否为空
		max_size					返回可容纳的最大元素数

	修改器
		clear							清除内容
		insert_after			在某个元素后插入新元素
		emplace_after			在元素后原位构造元素
		erase_after				擦除元素后的元素
		push_front				插入元素到容器起始
		emplace_front			在容器头部就地构造元素
		pop_front					移除首元素
		resize						改变容器中可存储元素的个数
		swap							交换内容

	操作
		merge							合并二个已排序列表
		splice_after			从另一 forward_list 移动元素
		remove						移除满足特定标准的元素
		remove_if
		reverse						将该链表的所有元素的顺序反转
		unique						删除连续的重复元素
		sort							对元素进行排序

	非成员函数
		operator==				按照字典顺序比较 forward_list 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::forward_list)	特化 std::swap 算法(C++11)
		erase(std::forward_list)			擦除所有满足特定判别标准的元素(C++20)
		erase_if(std::forward_list)

	ps:
		forward_list 没有成员函数size(), 但可以使用std::distance() 来代替;

	forward_list&list的erase()/insert()操作的区别:
		list:
			erase(): 删除迭代器指向的元素, 删除后, 迭代器指向下一个元素, 再执行list 收缩, 最后返回迭代器;
			insert(): 在迭代器指向的元素前面插入, 插入后, 迭代器指向当前插入元素, 再执行list 扩张, 最后返回迭代器;
		forward_list:
			erase_after() 删除迭代器所在的元素后面的元素, 删除后, 再后移一位(相当于后移了两位), 最后返回迭代器;
			insert_after() 在迭代器指向的元素后面插入, 插入后, 迭代器指向当前插入元素, 再执行list 扩张, 最后返回迭代器;
*/



#include <cstdio>
#include <forward_list>
#include <cassert>
#include <algorithm>



using namespace std;



int main(void){
	int tmp;
	int arr_tmp[] = {16,2,77,29};
	//1.创建forward_list
	forward_list<int> x1;												//empty forward_list of ints
	forward_list<int> x2 (4,100);								//four ints with value 100
	forward_list<int> x3 (x2.begin(),x2.end());	//iterating through x2
	forward_list<int> x4 (x3);									//a copy of x3
	//the iterator constructor can also be used to construct from arrays:
	forward_list<int> x5 (arr_tmp, arr_tmp + sizeof(arr_tmp)/sizeof(int));
	forward_list<int>::iterator it,it1,it2;;		//顺向迭代器(比较安全一点的方法)



	//2.void clear();//清空forward_list(所有forward_list 节点会被free()释放)
	x1.clear();
	x3.clear();
	//x1.pop_front();//测试forward_list 是否可以对空forward_list 进行pop()操作.



	//3.bool empty();//检查forward_list 是否为空
	if(x1.empty())
		printf("3 -- x1 forward_list is now empty\n");
	if(x2.empty())
		printf("3 -- x2 forward_list is now empty\n");
	if(x3.empty())
		printf("3 -- x3 forward_list is now empty\n");



	//4.forward_list 没有size() 成员函数, std::distance() 来代替;
	printf("4 -- x2 forward_list std::distance(std::begin(x2), std::end(x2))=%d\n", std::distance(std::begin(x2), std::end(x2)));



	//5.push 的方式插入元素(forward_list插入方法1)
	//void push_front(const T& x);//向forward_list 头部压入元素
	x2.push_front(123);



	//6.用迭代器+insert_after()的方式插入元素(forward_list插入方法2)
	it = x2.begin();
	x2.insert_after(it,555);//从头部插入
	printf("6 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//it = x2.end();
	//x2.insert_after(it,666);//从尾部插入(这种操作等同push_back(), 会导致forward_list溢出的, 禁止这样做)



	//7.迭代器遍历forward_list
	//iterator begin();//返回forward_list的顺向front()元素的迭代器
	//iterator end();//返回forward_list的顺向end()元素的迭代器
	//顺向迭代器遍历
	printf("7 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//利用std::find()函数做查找具体数值的位置, 返回迭代器
	tmp = 123;//查找目标值target
	it=find(x2.begin(),x2.end(),tmp);
	if(it != x2.end())
		printf("7 -- it=find(x2.begin(),x2.end(),%d); found it okay!\n",tmp);
	else
		printf("7 -- target-%d cant find\n",tmp);



	//8.forward_list 元素访问典范demo:
	//iterator erase_after(iterator position);//删除迭代器当前位置后面的一个元素, 删除后, 又后移一位(实际相当于后后移了2位), 最后返回迭代器
	/*
		由于forward_list 不随机访问元素, 所以必须是'迭代器指针'访问和操作.
		因此:
			当你需要访问forward_list迭代器后第n个元素时,可以用std::advance(*it,n)//forward_list 迭代器*it向前推进n位;
			'访问下一个元素时', 你可以用std::next();//相当于++it1;
			'访问上一个元素时', 你可以用std::prev();//相当于--it2;
	*/

	//从forward_list 后面插入元素[10-90]
	for(tmp=1; tmp<10; tmp++)
		x1.push_front(tmp*10);
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");
                              // 90 80 70 60 50 40 30 20 10
	it1 = it2 = x1.begin();     // ^^
	std::advance(it2,6);        // ^                 ^
	++it1;                      //    ^              ^

	it1 = x1.erase_after(it1);  // 90 80 60 50 40 30 20 10
                              //       ^        ^

	it2 = x1.erase_after(it2);  // 90 80 60 50 40 30 10
                              //       ^           ^
	++it1;                      //          ^        ^
	//++it2;                    //       ^            (再移动迭代器就失效了)

	x1.erase_after(it1,it2);    // 90 80 60 50 10
                              //          ^  ^
	printf("%d %d\n", *it1, *it2);

	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//9.弹出元素 && 直接访问头,尾元素
	//reference front();//直接访问第一个元素
	//void pop_front();//从头部弹出元素
	assert(!x2.empty());
	x2.pop_front();

	//空向前队列的x2.begin() == x2.end() !!
	assert(!x2.empty());
	printf("9 -- x2 front=%d,空forward_list x2.begin() == x2.end()!!\n", x2.front());

	x2.clear();
	if(x2.empty())
		printf("9 -- x2 forward_list is now empty\n");//元素已经删空
	printf("9 -- x2 forward_list std::distance(std::begin(x2), std::end(x2))=%d\n", std::distance(std::begin(x2), std::end(x2)));

	//对空forward_list 错误操作测试
	//x2.pop_front();//forward_list 由于是指针实现的, 绝对不能pop 空, 否则指针会溢出.
	//x2.pop_front();



	//10.void resize(size_type sz, T c = T());//重新修正forward_list 的容量(多出来的forward_list 节点会被free()释放)
	x2.resize(0);
	printf("10 -- x2 forward_list std::distance(std::begin(x2), std::end(x2))=%d\n", std::distance(std::begin(x2), std::end(x2)));
	if(x2.empty())
		printf("10-- x2 forward_list is now empty\n");

	x2.resize(10,123);//重新创建10个元素, 每个元素都是123
	printf("10 -- x2 forward_list std::distance(std::begin(x2), std::end(x2))=%d\n", std::distance(std::begin(x2), std::end(x2)));
	if(x2.empty())
		printf("10 -- x2 forward_list is now empty\n");



	//11.void sort();		//对元素进行排序处理(空forward_list也可以排序)
	//	 void reverse();//对元素进行反序处理
	x2.sort();
	printf("11 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("11 -- 插入元素后, 再排序:\n");
	x2.push_front(11111);//插入元素后, 再排序
	x2.push_front(111);
	x2.push_front(11112);
	x2.sort();
	printf("11 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//12.void swap(forward_list<T,Allocator>& lst);//将两个forward_list 的元素互换
	printf("12 -- 将两个forward_list 的元素互换\n");
	x1.swap(x2);//将x1 x2的所有元素互相交换

	printf("12 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	printf("12 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//13.void unique();//去除forward_list 中的'重复值'元素
	printf("13 -- 去除重复元素值测试\n");
	printf("13 -- before: x1 forward_list std::distance(std::begin(x1), std::end(x1))=%d\n", std::distance(std::begin(x1), std::end(x1)));
	x1.unique();
	printf("13 -- after: x1 forward_list std::distance(std::begin(x1), std::end(x1))=%d\n", std::distance(std::begin(x1), std::end(x1)));

	printf("13 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//14.根据删除条件, 删除符合条件的所有元素
	//void remove( const T& value );
	//void remove_if( UnaryPredicate p );
	x1.push_front(111);
	x1.remove(111);																//移除两个等于111的元素
	x1.remove_if([](int n){ return n < 11111; });	//移除全部小于11111的元素(内嵌钩子函数, 多重递归调用)
	printf("14 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//15.void splice_after(iterator position,forward_list<T,Allocator>&x);//剪切一个forward_list的所有元素,插入到另外一个forward_list中,完成后清空被剪切的forward_list
	printf("15 -- 剪切一个forward_list的所有元素,插入到另外一个forward_list中,完成后清空被剪切的forward_list.\n");
	it = x1.begin();
	++it;
	x1.splice_after(it,x2);//将x2所有元素插入x1的第二顺位的元素之间, 并清空x2
	if(x2.empty())
		printf("15 -- x2 forward_list is now empty\n");

	printf("15 -- x1:");
	for(it = x1.begin(); it != x1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");



	//16.void merge( forward_list& other );//合并两个forward_list, 不会自动排序, 但会清空其中一个forward_list
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


