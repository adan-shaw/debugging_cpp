//编译:
//		g++ -std=c++11 -g3 ./array.cpp -o x 



//array api list
/*
	成员函数
		(构造函数)			遵循聚合初始化的规则初始化array(注意默认初始化可以导致非类的T的不确定值)
		(析构函数)			销毁array 的每个元素
		operator=			以来自另一array 的每个元素重写array 的对应元素

	元素访问
		at						访问指定的元素,同时进行越界检查
		operator[]		访问指定的元素
		front					访问第一个元素
		back					访问最后一个元素
		data					返回指向内存中数组第一个元素的指针

	迭代器
		begin					返回指向容器第一个元素的迭代器
		cbegin
		end						返回指向容器尾端的迭代器
		cend
		rbegin				返回指向容器最后元素的逆向迭代器
		crbegin
		rend					返回指向前端的逆向迭代器
		crend

	容量
		empty					检查容器是否为空
		size					返回容纳的元素数
		max_size			返回可容纳的最大元素数


	操作
		fill					以指定值填充容器
		swap					交换内容

	非成员函数
		operator==		按照字典顺序比较array 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::get(std::array)						访问array 的一个元素
		std::swap(std::array)						特化std::swap 算法(C++11)
		to_array												从内建数组创建std::array 对象(C++20)

	辅助类
		std::tuple_size<std::array>			获得array 的大小
		std::tuple_element<std::array>	获得array 元素的类型
*/



#include <array>
#include <cstdio>
#include <algorithm>



int main(void){
	int tmp, len;
	//
	//1.array 容器初始化
	//
	//初始化为普通array 数组, 与c 语言无异, 这样用的话, 可以直接用回c 语言array 数组
																				//由编译器填充常量的初始化方式
	std::array<int, 3> a1{ {1, 2, 3} };		//c++11 的标准写法
	std::array<int, 3> a11{ 1, 2, 3 };		//c++14(以上的写法, 但不知道为何, g++ -std=c++11 并没有报错, 应该被兼容了)

	std::array<int, 3> a2 = { 1, 2, 3 };	//'='号拷贝初始化(可能会被编译器优化)

	//初始化为内嵌array 数组, c++ 的优势用法
	std::array<std::string, 2> b1{{ std::string("i'm b1[0]"), "i'm b1[1]" }};//内嵌元素: b1[0]=std::string, b1[1]="i'm b1[1]"
	std::array<std::string, 2>::iterator iter;//定义遍历迭代器
	printf("b1[0]=%s\n", b1.at(0).c_str());
	printf("b1[1]=%s\n", b1.at(1).c_str());		//b1[1]="i'm b1[1]", 并不会初始化为char*, 而是统一初始化为std::string



	//
	//2.迭代器访问
	//
	//元素的空间大小固定的情况下, 可以使用传统c语言数组的遍历方式
	len = a1.size();
	for(tmp=0; tmp<len; tmp++)
		printf("%d ",a1.at(tmp));

	//元素的空间大小未知的情况下, 最好使用新的遍历方式(都需要用到中间变量std::string i, 性能一般, 靠c++编译器优化, 写法还很诡异)
	std::for_each(b1.begin(), b1.end(), [](std::string &i){printf("%s\n",i.c_str());});	//遍历新写法[多重递归函数调用, 劣质, 还内嵌了遍历子函数, java写法]
																																											//for_each() 第三参数, 其实是函数指针, 需要传入遍历子函数, 负责操作每一个元素;
																																											//[](std::string &i){printf("%s ",i.c_str());} 实际上是一个子函数;
																																											//for_each的实现, 实际也有用到if, 也会有逻辑分支产生, 具体请看for_each源码
	for(const auto i : b1){printf("%s\n",i.c_str());}																		//遍历新写法2

	//最优的通用迭代方式(c/c++ 程序员最优选择)
	for(iter = b1.begin(); iter != b1.end(); ++iter)																		//最优遍历方式, 安全高效, 反正你都定义了array 容器类型了, 再声明一个迭代器也合理;
		printf("%s\n",iter->c_str());																											//实在怕出错, 这种方式也是使用迭代器遍历的, 相对来说十分安全(最优)



	//
	//3.排序
	//
	std::sort(b1.begin(), b1.end());		//排序
	for(iter = b1.begin(); iter != b1.end(); ++iter)
		printf("%s\n",iter->c_str());

	std::reverse(b1.begin(), b1.end());	//反序
	for(iter = b1.begin(); iter != b1.end(); ++iter)
		printf("%s\n",iter->c_str());



	//
	//4.拷贝
	//
	//普通元素类型的array(浅拷贝)
	auto copy = a1;
	len = copy.size();
	for(tmp=0; tmp<len; tmp++)
		printf("%d ",copy.at(tmp));

	//复杂内嵌元素类型的array('='号自动深拷贝, 不需要逻辑遍历拷贝了)
	auto copy2 = b1;
	b1.at(0) = "idiot !!";
	for(iter = copy2.begin(); iter != copy2.end(); ++iter)
		printf("%s\n",iter->c_str());
	for(iter = b1.begin(); iter != b1.end(); ++iter)
		printf("%s\n",iter->c_str());

	//如果'='号不会自动深拷贝, 请自己使用逻辑深拷贝(略), 建议使用迭代器逻辑遍历, 然后逐个拷贝;





	//
	//5.杂项
	//
	//填充值, 相当于memset()
	a11.fill(999);
	//a11.fill("idiot");不能乱填充不同数据类型的值, 否则编译器报错
	len = a11.size();
	for(tmp=0; tmp<len; tmp++)
		printf("%d ",a11.at(tmp));

	//交换值(不能乱填充不同数据类型的值, 否则编译器报错)
	a11.swap(a1);
	len = a11.size();
	for(tmp=0; tmp<len; tmp++)
		printf("%d ",a11.at(tmp));

	len = a1.size();
	for(tmp=0; tmp<len; tmp++)
		printf("%d ",a1.at(tmp));



	return 0;
}
