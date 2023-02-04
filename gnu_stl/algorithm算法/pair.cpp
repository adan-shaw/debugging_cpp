//编译:
//		g++ -std=c++11 -g3 ./pair.cpp -o x 



//pair 简介
/*
	pair类型, 定义在#include<utility>中, 定义如下:
	类模板: 
		template<class T1,class T2> struct pair

	参数: 
		T1是第一个值的数据类型
		T2是第二个值的数据类型

	功能: 
		pair将一对值(T1,T2)组合成一个值, 这一对值可以具有不同的数据类型(T1,T2);
		这两个值可用pair的两个公有成员first和second访问;

	ps:
		pair 在关联容器中, 用得比较多;
*/



//pair API 简介
/*
	成员对象
		first					T1
		second				T2

	成员函数
		(构造函数)			构造新的pair
		operator=			赋值
		swap					交换内容(C++11)

	非成员函数
		make_pair			创建一个pair 对象, 其类型根据各实参类型定义
		operator==		按字典序比较pair 中的值
		operator!=
		operator<
		operator<=
		operator>
		operator>=
		std::swap(std::pair)					特化std::swap 算法(C++11)
		std::get(std::pair)						访问pair 的一个元素(C++11)

	辅助类
		std::tuple_size<std::pair>		获得pair的大小(C++11)
		std::tuple_element<std::pair>	获得pair中元素的类型(C++11)
*/



#include<utility>
#include<cstdio>
#include<string>



int main(void){
	//std::pair<T1, T2> pair_tmp;														//初始化pair, 模板必须实体化成具体的类型
	std::pair<std::string, int> pair_tmp("only shit", 999);	//固定类型pair初始化
	auto p1 = std::make_pair(1, "idiot");										//make_pair 自动根据实参数据, 由编译器自动识别类型, 跟auto 变量是一样的;
	auto p2 = std::make_pair("222", "shit");

	//访问pair 成员
	printf("%s,%d\n", pair_tmp.first.c_str(), pair_tmp.second);
	printf("%d,%s\n", p1.first, p1.second);
	printf("%s,%s\n", p2.first, p2.second);

	return 0;
}
