//编译:
//		g++ -std=c++11 -g3 ./it迭代器空if判断.cpp -o x 



//1.如何判断'it迭代器'指向的元素为空?
/*
	在某些场合, 你需要知道'it迭代器'指向的元素, 是否为一个空元素;

	这时候, 你或许会想到:
		if(it == NULL)
	但c++ 不允许'it迭代器'与NULL 进行比较, 因此这种方式, 是不可行的!!

	正确判断找不到元素的方法是:
		//判断迭代器是否已经跑到结束位置上了, 如果it == vec.end(), 说明元素找不到/不存在;
		if(it == vec.end())
*/



//2.另类判断容器为空的方法:
//	if(vec.begin() == vec.end())



//3.让'it迭代器'强制超越最后一个迭代器vec.end():
/*
	在空容器中, 强行移动'it迭代器', 是可以让'it迭代器'强制超越最后一个迭代器vec.end()的;
	超越之后, 'it迭代器'会大于vec.end();

	虽然在默认情况下, 'it迭代器'是不会超越最后一个迭代器vec.end()的, 
	但如果你强行操作, 'it迭代器'是可以强制超越最后一个迭代器vec.end()的;
	这时候必然导致:
		if(it != vec.end())
	但此时'it迭代器'已经不具备任何实际意义了, 就是一个类似野指针的'野it迭代器', 一旦访问, 即崩溃/出现错乱值;

	所以, 使用'it迭代器', 必须自己保证不会超越最后一个迭代器vec.end();

	判断'it迭代器'有效性的办法:
		if(it>=vec.end() && it<=vec.end()
*/



//4.'it迭代器'快速取值函数front(), back() 不安全, 慎用:
/*
	if(vec.begin() == vec.end()) 的确可以判断stl 容器为空, 但即便stl 容器为空, 但一般默认都会预分配存储空间的, 如: vector;
	因此, front(), back() 这两个快速访问函数, 并不安全;
	因此, *vec.begin(), *vec.end()  这两个'it迭代器'访问, 也不安全;
	因为此时stl 容器可能已经为空;

	你必须保证stl 容器不为空:
		assert(!vec.empty());
	否则迭代器快速访问, 得到的数值不一定准确;

	另外:
		front() 不一定等于*vec.begin();
		back() 不一定等于*vec.end();

	front() = stl容器'已申请的内存空间地址'中第一个元素;
	back() = stl容器'已申请的内存空间地址'中最后一个元素;

	*vec.begin() = stl容器'现存的元素'的第一个;
	*vec.end() = stl容器'现存的元素'的最后一个;
*/



#include <cstdio>
#include <vector>



int main(void){
	std::vector<int> vec;
	std::vector<int>::iterator it;



	//1.另类判断容器为空的方法:
	if(vec.begin() == vec.end())
		printf("(vec.begin() == vec.end())\n");



	//2.判断是否'it迭代器'找到元素
	it = vec.begin();			//假设值, 并不重要
	if(it == vec.end())
		printf("(it == vec.end(), cant find element in vector)\n");



	//3.让'it迭代器'强制超越最后一个迭代器vec.end();
	it = vec.begin();			//假设值, 并不重要
	if(++it == vec.end())	//明知vector容器为空, 故意移动'it迭代器', 看看移动后'it迭代器'是否还等于vec.end()
		printf("(++it == vec.end(), iterator cant overstep the last iterator vec.end())\n");
	else
		printf("(++it != vec.end()\n");

	it += 3;
	if(it > vec.end())		//超越之后, 'it迭代器'会大于vec.end()
		printf("(it > vec.end()\n");
	else
		printf("(it < vec.end()\n");



	//4.'it迭代器'快速取值函数front(), back() 不安全的论证:
	vec.push_back(123);
	vec.clear();
	printf("vec.size()=%d\n",vec.size());
	printf("vec.front()=%d\n",vec.front());
	printf("vec.back()=%d\n",vec.back());
	printf("*vec.begin()=%d\n",*vec.begin());
	printf("*vec.end()=%d\n",*vec.end());

	return 0;
}


