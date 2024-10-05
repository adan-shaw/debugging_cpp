//编译:
//		g++ -std=c++11 -g3 ./deque_old.cpp -o x 



#include <cstdio>
#include <cassert>
#include <deque>



using namespace std;



int main(void){
	int tmp,size;
	int myints[] = {16,2,77,29};
	//1.创建双向队列 - 多种创建, 集成创建方式.
	deque<int> x1;											//empty deque of ints
	deque<int> x2 (4,100);							//four ints with value 100
	deque<int> x3 (x2.begin(),x2.end());//iterating through x2
	deque<int> x4 (x3);									//a copy of x3
	//the iterator constructor can also be used to construct from arrays:
	deque<int> x5 (myints, myints + sizeof(myints) / sizeof(int) );
	deque<int>::iterator it;						//顺向迭代器
	deque<int>::reverse_iterator rit;		//逆向迭代器



	//2.void clear();//清空双向队列
	x1.clear();
	x3.clear();



	//3.bool empty();//检查双向队列是否为空
	if(x1.empty())
		printf("3 -- x1 deque is now empty\n");
	if(x2.empty())
		printf("3 -- x2 deque is now empty\n");
	if(x3.empty())
		printf("3 -- x3 deque is now empty\n");



	//4.size_type size();//返回deque 的size()
	printf("4 -- x2 deque size()=%d\n", x2.size());



	//5.void push_back(const T& x);//向deque 尾部压入元素
	x2.push_back(321);

	//6.void push_front(const T& x);//向deque 头部压入元素
	x2.push_front(123);



	//7.reference at(size_type n);//下表访问元素
	assert(!x2.empty());
	//第一种遍历元素的方法: at() 遍历
	printf("7 -- x2:");
	for(tmp=0,size=x2.size();tmp < size;tmp++)
		printf("at(%d)=%d,", tmp, x2.at(tmp));
	printf("\n");

	assert(!x2.empty());
	//直接下标访问
	printf("7 -- x2:");
	for(tmp = 0;tmp < size;tmp++)
		printf("x2[%d]=%d,", tmp, x2[tmp]);
	printf("\n");



	//8.
	//iterator begin();	//返回双向队列--顺向front() 元素的迭代器
	//iterator end();		//返回双向队列--顺向end() 元素的迭代器
	assert(!x2.empty());
	//顺向迭代器遍历(比较安全一点的方法)
	printf("8 -- x2:");
	for(it = x2.begin(); it != x2.end(); *it++)
		printf("%d  ", *it);
	printf("\n");

	//9.
	//iterator rbegin();//返回双向队列--逆向front() 元素的迭代器
	//iterator rend();	//返回双向队列--逆向end() 元素的迭代器
	assert(!x2.empty());
	//逆向迭代器遍历
	printf("9 -- x2:");
	for(rit = x2.rbegin(); rit != x2.rend(); *rit++)
		printf("%d  ", *rit);
	printf("\n");



	//10.deque 迭代器位移
	//iterator erase(iterator position);//通过迭代器删除元素
	if(!x2.empty() && x2.size() == 6){
		//***************
		for(tmp=0,it=x2.begin();tmp<2;tmp++){//移除begin()头部的两个元素
			//x2.erase(*it+=1);//错误写法
			assert(!x2.empty());
			x2.erase(it+=1);//等价于*++it 移动迭代器(*it++ 是操作完之后再加1)
		}
		//***************

		//再删除2 个元素, 这次是迭代器段删除
		if(x2.size() >=2 )//多个删除, 必须用size()
			x2.erase(x2.begin()+1,x2.begin()+3);

		//遍历打印剩余的123,321
		printf("10 -- x2:");
		for(it = x2.begin(); it != x2.end(); *it++)
			printf("%d,", *it);
		printf("\n");
	}



	//11.弹出元素 && 直接访问头,尾元素


	//void pop_back();		//从尾部弹出元素
	assert(!x2.empty());
	x2.pop_back();

	//reference front();	//直接访问第一个元素
	//reference back();		//直接访问最后一个元素
	assert(!x2.empty());
	printf("11 -- x2 front=%d, back=%d, 只有一个元素时, 头等于尾.\n", x2.front(), x2.back());

	//void pop_front();		//从头部弹出元素
	assert(!x2.empty());
	x2.pop_front();
	assert(x2.empty());
	printf("11 -- x2 deque is now empty\n");//元素已经删空


	//12.对空deque 错误弹出操作测试
	/*
	注意:
		pop_front()只对头操作, 可以pop 成负数;
		所以任何pop 操作都要检索deque 是否为empty();
		否则deque 可能会被pop 坏;
		deque 内部计数出现严重错误, 影响后续运作.
	 */
	x2.pop_front();
	x2.pop_front();
	x2.pop_front();
	printf("12 -- x2 front=%d, back=%d, 对空deque错误弹出\n", x2.front(), x2.back());
	if(x2.empty())
		printf("12 -- x2 deque is now empty\n");
	else
		printf("12 -- x2 deque size()=%d\n", x2.size());



	//13.void resize(size_type sz, T c = T());
	//	如果容器已经坏了,那么resize()一下,重新修改空deque为0,清空元素.
	x2.resize(0);
	printf("13 -- x2 deque size()=%d\n", x2.size());
	if(x2.empty())
		printf("13 -- x2 deque is now empty\n");

	//重新创建10个元素, 每个元素都是123
	x2.resize(10,123);
	printf("13 -- x2.resize(10,123)\n");
	printf("13 -- x2 deque size()=%d\n", x2.size());
	if(x2.empty())
		printf("13 -- x2 deque is now empty\n");



	//14.void swap(list<T,Allocator>& dqe);//将两个deque 的元素互换
	printf("14 -- 将两个deque 的元素互换\n");
	x1.push_front(8888);//x1 此时为空, 插入一个元素,
	x1.swap(x2);//将x1 x2的所有元素互相交换


	//打印x1
	if(!x1.empty()){
		printf("14 -- x1:");
		for(it = x1.begin(); it != x1.end(); *it++)
			printf("%d  ", *it);
		printf("\n");
	}

	//打印x2
	if(!x2.empty()){
		printf("14 -- x2:");
		for(it = x2.begin(); it != x2.end(); *it++)
			printf("%d  ", *it);
		printf("\n");
	}


	return 0;
}


