//编译:
//		g++ -g3 ./template类模板.cpp -o x



#include<iostream>
#include<string>



//c++ 模板类的基本使用:
/*
	1.模板类的'成员函数定义'格式:
	'class_name'<T>::'func_name'(){}


	2.使用模板类时, 定义实体变量(必须初始化模板类型):
	map<int, char> m_map;
	map<int, string> m_map;


	3.模板类不支持'分开文件'定义, .h .cpp 文件分离之后, 
		模板类会找不到class 中的成员函数&变量, 解决办法是:
			* class 成员函数&变量, 所有内容都定义在.h 头文件中;
			* 使用export 关键字, 导入类实现函数(export关键字在c11中被删除,这种方法不推荐)


	4.模板关键字有:
		定义模板变量: template<typename T>
		定义模板类: template<class T>
	template<typename T> 作用范围广, 是template<class T> 的超集;
	用template<class T> 定义模板类, 编译器解析快一点, 
	用template<typename T> 定义模板类, 编译器解析慢一点, 
	但不影响最终编译结果;


	5.泛型(即模板类, 参数类型模板化)
	泛型不属于c++ 面向对象的特性, 但泛型支撑起c++ 面向对象, 抽象描述的关键任务:
		类型自识别(一般是交给编译器识别[在-E 预编译, 预处理过程中识别], 
						 编译器识别后, 写机器码的时候, 也是固定类型的, 
						 只不过在编译期间, 自动识别类型, 更方便程序员做抽象描述, 更方便程序员做快速设计)

	泛型的好处:
		一个模板, 应付所有类型, 节约代码量, 也提高抽象编程的描述能力
*/





//c++ 模板特异性(函数重载):
/*
	如果两个函数名相同, 其参数的'类型/个数/顺序'不相同, 则成为重载函数;
	(编译器会自动根据程序逻辑,选择适合的函数,作为build烧写对象)
*/

template<typename T> void fun(T *data){
	//T == 模板 == 任意类型
}
template<typename T> void fun(char *data){
	//T == char == 当T=char时, 优先选择这个函数
}
template<typename T> void fun(int *data){
	//T == int == 当T=int时, 优先选择这个函数
}





//'typeid操作符'打印'模板T'的数据类型:
/*
	typeid是操作符, 不是函数!!
	主要是告诉编译器, 在预处理(预编译)的过程中, 识别'模板类/变量'的类型, 进而做下一步处理;

	获知变量类型名称, 使用 typeid(变量实体).name();
*/
template<typename T> void PrintType(T){
	std::cout << typeid(T).name() << std::endl;
}



int main(void){
	int xint;
	double xdouble;
	float xfloat;
	char xchar;

	void* pvoid;
	char* pchar;
	int* pint;

	int array[10];
	char carray[10];



	PrintType(xint);		//i = int
	PrintType(xdouble);	//d = double
	PrintType(xfloat);	//f = float
	PrintType(xchar);		//c = char

	PrintType(pvoid);		//Pv = point void
	PrintType(pchar);		//Pc = point char
	PrintType(pint);		//Pi = point int

	PrintType(array);		//Pi = point int
	PrintType(carray);	//Pc = point char

	//其他非<常值类型>的数据类型, 如string 等c++ 标准容器类型, 编译器翻译码很乱:
	//NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	PrintType(std::string("string"));
	return 0;
}
