//编译:
//		g++ -std=c++11 -g3 ./priority_queue.cpp -o x

//std::priority_queue 简介
/*
	std::priority_queue 是C++ STL中的一个容器适配器, 可更改的数据底座有: vector/deque
	std::priority_queue 提供了一个FIFO(先进先出)的数据结构, 但它保证在任何时间top 元素是最大(或最小)
	(强制根据值的优先级调度, 不根据时间先后顺序调度)
	这就意味着, 虽然std::priority_queue 的接口看起来是先进后出, 但实际上它是基于优先级的先出;
	调度权值:
		值大小优先级 > 时间先后大小优先级
*/

//priority_queue api list
/*
	成员函数
		(构造函数)			构造 priority_queue
		(析构函数)			析构 priority_queue
		operator=			赋值给容器适配器

	元素访问
		top						访问栈顶元素

	容量
		empty					检查底层的容器是否为空
		size					返回容纳的元素数

	修改器
		push					插入元素,并对底层容器排序
		emplace				原位构造元素并排序底层容器(C++11)
		pop						删除栈顶元素
		swap					交换内容

	成员对象
		Container c		底层容器(受保护成员对象)
		Compare comp	比较函数对象(受保护成员对象)

	非成员函数
		std::swap(std::priority_queue)						特化 std::swap 算法

	辅助类
		std::uses_allocator<std::priority_queue>	特化 std::uses_allocator 类型特性(C++11)
*/



#include <cstdio>
#include <cassert>
#include <queue>
#include <vector>
#include <functional>
#include <string>



using namespace std;



void normal_test(void){
	//强制指定priority_queue 的各种细节, 这种初始化模式, 不支持从'原始线性容器'中提取已存在的值!!
	priority_queue<int,vector<int>,std::greater<int>> xx1;//升序, [0] 最大(定义可以这么定义, 用千万不要这么用)
	priority_queue<int,deque<int>,std::less<int>> xx2;		//降序, [0] 最小[最佳用法]

	vector<int> x1(11,100);																//初始化'原始线性容器'
	deque<int> x2(22,999);

	//强制指定priority_queue 的'原始线性容器'类型
	priority_queue<int> pq2(std::less<int>(),x1);					//降序, [0] 最小
	//priority_queue<int> pq3(std::less<int>(),x2);				//降序, [0] 最小(不能用deque, 好神奇, 报错报怕了)



	//2.返回'优先队列'size()
	printf("2 -- pq2 size()=%d\n",pq2.size());



	//3.判断'优先队列'是否为空
	if(pq2.empty())
		printf("3 -- pq2 is now empty\n");



	//4.返回'优先队列'top元素的值(只返回,不移除; 为了保证查看top值的有效性, 最好加断言)
	assert(!pq2.empty());
	printf("4 -- pq2 top()=%d\n", pq2.top());



	//5.pop弹出'优先队列'top元素的值[pop_back()]
	assert(!pq2.empty());
	pq2.pop();
	printf("5 -- pq2 size()=%d\n", pq2.size());



	//6.push压入一个值[push_back()]
	pq2.push(1111);
	assert(!pq2.empty());
	printf("6 -- pq2 top()=%d\n", pq2.top());
	printf("6 -- pq2 size()=%d\n", pq2.size());



	//7.清空优先队列 && 溢出测试
	assert(!pq2.empty());
	pq2.pop();
	assert(!pq2.empty());
	pq2.pop();
	assert(!pq2.empty());
	pq2.pop();
	printf("7 -- pq2 size()=%d\n", pq2.size());
	if(pq2.empty())
		printf("7 -- pq2 is now empty\n");



	//8.快速pop() 空队列
	while(!pq2.empty())
		pq2.pop();
	printf("8 -- pq2 size()=%d\n", pq2.size());



	//9.溢出测试: 程序退出时会报错: munmap_chunk(): invalid pointer, 估计是容器坏了之后, 收回内存的时候报错!!
	pq2.pop();
	pq2.pop();
	printf("9 -- pq2 size()=%d\n", pq2.size());
	printf("9 -- pq2 top()=%d\n", pq2.top());					//溢出后查看top值, top值已经失效!!
	pq2.push(1111);																		//溢出后push, 容器已损坏, 不能再用, 但不会崩溃
	pq2.push(1111);
	pq2.push(1111);
	printf("9 -- pq2 queue size()=%d\n", pq2.size());	//容器容量, 无效值



	printf("end\n");
	return ;
}



//自定义结构体
struct body{
	int id;
	double x;
	string s;
};

//自定义比较函数(根据结构体id 进行比较) [类似lambda 关键字]
auto pg_cmp = [](struct body &left, struct body &right) { return left.id < right.id; };

void struct_test(void){
	struct body x1,x2,x3,x4,x5,*px;
	//定义priority_queue 特殊实体
	priority_queue<struct body, std::vector<struct body>, decltype(pg_cmp)> pg(pg_cmp);

	x1.id = 1;
	x2.id = 2;
	x3.id = 3;
	x4.id = 4;
	x5.id = 5;

	pg.push(x1);
	pg.push(x3);
	pg.push(x5);
	pg.push(x2);
	pg.push(x4);

	//px = pg.top();
	printf("struct_test: pg.top()=%d\n", pg.top().id);

	return;
}



int main(void){
	struct_test();
	normal_test();
	return 0;
}
