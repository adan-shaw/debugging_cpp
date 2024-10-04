//编译:
//		g++ -std=c++11 -g3 ./stack.cpp -o x 



//stack api list
/*
	成员函数
		(构造函数)			构造 stack
		(析构函数)			析构 stack
		operator=			赋值给容器适配器

	元素访问
		top						访问栈顶元素

	容量
		empty					检查底层的容器是否为空
		size					返回容纳的元素数

	修改器
		push					向栈顶插入元素
		emplace				于顶原位构造元素(C++11)
		pop						删除栈顶元素
		swap					交换内容

	成员对象
		Container c		底层容器(protected 保护对象, 外部不能访问, 仅stack 定义时, 内部可访问, stack 实体对象不能引用)

	非成员函数
		operator==		按照字典顺序比较 stack 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::stack)							特化 std::swap 算法

	辅助类
		std::uses_allocator<std::stack>		特化 std::uses_allocator 类型特性(C++11)



	栈是一种先进后出(FILO)的数据结构, 默认情况下, 'stack栈'只操作栈顶, 符合频繁push_back()/pop_back(), 
	因此, vector 是最理想的stack'原始线性容器', 无特殊情况, 请使用vector 构建'stack栈', 性能最好;
*/



#include <cstdio>
#include <cassert>
#include <stack>
#include <vector>
#include <deque>
#include <list>


using namespace std;



int main(void){
	vector<int> x1(11,100);		//初始化'原始线性容器'
	deque<int> x2(22,999);
	list<int> x3(33,33);

	//强制指定stack 的'原始线性容器'类型
	stack<int,vector<int>> s1(x1);
	stack<int,deque<int>> s2(x2);
	//stack<int> s2(x2);			//stack默认使用deque容器, 等价于上一条语句
	stack<int,list<int>> s3(x3);

	//stack<int>::iterator it;//'stack栈'没有迭代器
	vector<int>::iterator it;
	vector<int>::reverse_iterator rit;



	//2.bool empty();					//栈为空则返回真
	if(s1.empty())
		printf("2 -- 's1 stack' is now empty!!\n");



	//3.void push(const T& x);//在栈顶增加元素(vector 会自动增长, 不用担心爆栈)
	s1.push(1);
	s1.push(2);
	s1.push(3);
	printf("3 -- \n");



	//4.size_type size();			//返回栈中元素数目
	if(s1.empty())
		printf("4 -- 's1 stack' is still empty!!\n");
	else
		printf("4 -- 's1 stack' size()=%d\n", s1.size());



	//5.value_type& top();		//返回栈顶元素(只返回,不移除; 为了保证查看top值的有效性, 最好加断言)
	assert(!s1.empty());
	printf("5 -- 's1 stack' size()=%d\n", s1.size());
	printf("5 -- 's1 stack' top=%d\n", s1.top());
	printf("5 -- 's1 stack' size()=%d\n", s1.size());



	//6.void pop();						//移除栈顶元素
	assert(!s1.empty());
	s1.pop();
	assert(!s1.empty());
	s1.pop();
	assert(!s1.empty());
	printf("6 -- 's1 stack' top=%d\n", s1.top());
	printf("6 -- 's1 stack' size()=%d\n", s1.size());



	//'stack栈'不能遍历(因为'stack栈'没有迭代器,又不能通过'成员对象Container c',进行逆向访问底层容器,所以'stack栈'不能遍历)
	/*
	//7.1: 顺向迭代器
	printf("7.1 -- s1+it:");
	for(it=s1.begin(); it!=s1.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//7.2: 逆向迭代器
	printf("7.2 -- s1+rit:");
	for(rit=s1.rbegin(); rit!=s1.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");
	*/



	//8.快速pop() 空栈
	while(!s1.empty())
		s1.pop();
	printf("8 -- 's1 stack' size()=%d\n", s1.size());



	//9.最后清空栈后, 看看空栈的时候top() 会显示什么
	s1.pop();
	s1.pop();
	printf("9 -- 's1 stack' size()=%d\n", s1.size());
	if(s1.empty())
		printf("9 -- 's1 stack' is now empty!!\n");

	//测试已损坏的栈
	printf("9 -- 's1 stack' top=%d\n", s1.top());//栈顶=无效值
	s1.push(1);//崩溃, 根据vector 特性, 向已损坏的vector push_back(), 就会崩溃!! 跟随vector 的特性



	return 0;
}


