//编译:
//		g++ -std=c++11 -g3 ./queue.cpp -o x 



//queue api list
/*
	成员函数
		(构造函数)			构造 queue
		(析构函数)			析构 queue
		operator=			赋值给容器适配器

	元素访问
		front					访问第一个元素
		back					访问最后一个元素

	容量
		empty					检查底层的容器是否为空
		size					返回容纳的元素数

	修改器
		push					向队列尾部插入元素
		emplace				于尾部原位构造元素(C++11)
		pop						删除队列头部元素
		swap					交换内容

	成员对象
		Container c		底层容器(protected 保护对象, 外部不能访问, 仅queue 定义时, 内部可访问, queue 实体对象不能引用)

	非成员函数
		operator==		按照字典顺序比较 queue 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::queue)							特化 std::swap 算法

	辅助类
		std::uses_allocator<std::queue>		特化 std::uses_allocator 类型特性(C++11)
*/



#include <cstdio>
#include <cassert>
#include <queue>
#include <deque>
#include <list>



using namespace std;



int main(void){
	deque<int> x2(22,999);		//初始化'原始线性容器'
	list<int> x3(33,33);

	//强制指定queue 的'原始线性容器'类型
	queue<int,deque<int>> q2(x2);
	//queue<int> s2(x2);			//queue默认使用deque容器, 等价于上一条语句
	queue<int,list<int>> q3(x3);
	

	//queue<int>::iterator it;//'queue队列'没有迭代器
	deque<int>::iterator it;
	deque<int>::reverse_iterator rit;



	//2.bool empty();					//队列为空则返回真
	if(q2.empty())
		printf("2 -- 'q2 queue' is now empty!!\n");



	//3.void push(const T& x);//在队列尾部增加元素(deque 会自动增长, 不用担心爆队列)
	q2.push(1);
	q2.push(2);
	q2.push(3);
	printf("3 -- \n");



	//4.size_type size();			//返回队列中元素数目
	if(q2.empty())
		printf("4 -- 'q2 queue' is still empty!!\n");
	else
		printf("4 -- 'q2 queue' size()=%d\n", q2.size());



	//5.value_type& back();		//查看队列尾部元素(只返回,不移除; 为了保证查看back值的有效性, 最好加断言)
	//	value_type& front();	//查看队列头部元素(只返回,不移除; 为了保证查看front值的有效性, 最好加断言)
	assert(!q2.empty());
	printf("5 -- 'q2 deque' size()=%d\n", q2.size());
	printf("5 -- 'q2 queue' back()=%d\n", q2.back());
	printf("5 -- 'q2 deque' size()=%d\n", q2.size());

	assert(!q2.empty());
	printf("5 -- 'q2 queue' front()=%d\n", q2.front());



	//6.void pop();						//移除队列头部元素
	assert(!q2.empty());
	q2.pop();
	assert(!q2.empty());
	q2.pop();
	assert(!q2.empty());
	printf("6 -- 'q2 queue' back()=%d\n", q2.back());
	printf("6 -- 'q2 queue' front()=%d\n", q2.front());
	printf("6 -- 'q2 queue' size()=%d\n", q2.size());



	//'queue队列'不能遍历(因为'queue队列'没有迭代器,又不能通过'成员对象Container c',进行逆向访问底层容器,所以'queue队列'不能遍历)
	/*
	//7.1: 顺向迭代器
	printf("7.1 -- q2+it:");
	for(it=q2.begin(); it!=q2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//7.2: 逆向迭代器
	printf("7.2 -- q2+rit:");
	for(rit=q2.rbegin(); rit!=q2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");
	*/



	//8.快速pop() 空队列
	while(!q2.empty())
		q2.pop();
	printf("8 -- 'q2 queue' size()=%d\n", q2.size());



	//9.最后清空队列后, 看看空队列的时候top() 会显示什么
	q2.pop();
	q2.pop();
	printf("9 -- 'q2 queue' size()=%d\n", q2.size());
	if(q2.empty())
		printf("9 -- 'q2 queue' is now empty!!\n");

	//测试已损坏的队列
	printf("9 -- 'q2 queue' back()=%d\n", q2.back());  //队列尾=无效值
	printf("9 -- 'q2 queue' front()=%d\n", q2.front());//队列头=无效值
	q2.push(1);//不会崩溃, 但根据deque 特性, 向已损坏的deque push_back(), 会丢失值, 造成数值错误!!(跟随deque 的特性)
	q2.push(1);
	q2.push(1);
	printf("9 -- 'q2 queue' size()=%d\n", q2.size());

	return 0;
}



