//编译:
//		g++ -g3 ./c++类型转换.cpp -o x



//c++类型转换详解
/*
	C++类型转换主要分为两种: 
		'隐式类型转换'
		显式类型转换(强制类型转换)

	(1)C风格是(type)expression
	(2)C++风格是static_cast<type>(expression)



	* '隐式类型转换':
		所谓'隐式类型转换', 是指不需要用户干预, 编译器默认进行的类型转换行为.
		(很多时候, 用户可能都不知道到底进行了哪些转换, 所以'隐式类型转换', 比较不好用)

		'隐式类型转换'一般分为两种:
			内置数据类型(基本数据类型的'隐式类型转换')
			自定义数据类型(一般侧重于使用: explicit关键字, 禁用'隐式类型转换')



	* 显式类型转换(强制类型转换)
		四种强制类型转换操作符:
			static_cast(比c 语言隐式类型强转好, 这个起码是显式的, 跟c 语言显式强转差别也不大)
			const_cast
			dynamic_cast(c++ 类的类型转换中常用)
			reinterpret_cast(很少用, 跟c 语言void* 转任意类型一样, 不太靠谱)
*/

#include <iostream>
#include <cstring>
using namespace std;





//1.内置数据类型test(基本数据类型)
void t1(void){
//	混合类型的算术运算表达式中
	int nValue = 8;
	float fValue = 10.7;
	double dValue = 10.7;
	double dSum = nValue + dValue;//nValue会被自动转换为double类型, 用转换的结果再与dValue相加
	dSum = fValue + dValue;				//fValue会被自动转换为double类型;

//	不同类型的赋值操作时
	nValue = true;								//bool类型被转换为int类型
}

//	函数参数传值时
double func(double dArg) { return dArg; };
double add(int na){
	int nb = func(1);							//调用函数时, 整型数值1被转换为double类型数值1.0, 与函数形参对齐;
	return (na + nb);							//返回运算结果时, 会被'隐式类型转换'为double类型返回, 与返回值类型对齐;
}


void t2(void){
	//简单的'隐式类型转换', 都满足了一个基本原则: 由低精度向高精度的转换; 若不满足该原则, 编译器会提示编译警告; 如下:
	double dValue = 100.2;
	int nValue = dValue;					//warning C4244: "初始化": 从"double"转换到"int", 可能丢失数据

	//当然, 这时我们若不想看到警告, 可以选择显式类型转换(又称强制类型转换), 如下:
	//(这种生硬强制转换, 可能会丢失数据, 非常不建议这样用)
	dValue = 100.2;
	nValue = (int)dValue;
}





//2.自定义数据类型
//'隐式类型转换'的风险, 一般存在于自定义类型转换间, 尤其需要注意自定义类的构造函数, 例如:
class MyString{
public:
	MyString(int n) {};						//本意: 预先分配n个字节给字符串
	MyString(const char* p) {};		//用C风格的字符串p作为初始化值
	//...
};

void t3(void){
	MyString s1 = "China";				//OK '隐式类型转换', 等价于MyString s1 = MyString("China")
	MyString s2(10);							//OK 分配10个字节的空字符串
	MyString s3 = MyString(10);		//OK 分配10个字节的空字符串

	//s4 和s5 分别把一个int型和char型, '隐式类型转换'成了分配若干字节的空字符串, 容易令人误解(有歧义)
	MyString s4 = 10;							//OK, 编译通过, 也是分配10个字节的空字符串
	MyString s5 = 'A';						//编译通过, 分配int('A')个字节的空字符串(有歧义)

	//如上例, 要想禁止此种'隐式类型转换', 可以使用C++关键字explicit(详细请参见: explicit关键字:禁用'隐式类型转换'.cpp)
}





//3.static_cast
//static_cast 主要用于内置数据类型之间的相互转换:
void t4(void){
	double dValue = 12.12;
	float fValue = 3.14;					//VS2013 warning C4305: "初始化": 从"double"到"float"截断
	int nDValue = static_cast<int>(dValue);//12
	int nFValue = static_cast<int>(fValue);//3
}

//static_cast也可以转换自定义类型(如果涉及到类, static_cast只能在有相互联系(继承)的类型间进行转换, 且不一定包含虚函数):
//(不安全)
class A1 {};
class B1 : public A1 {};
class C1 {};
void t5(void){
	C1 *pC1;
	B1 *pB1;
	A1 *pA1 = new A1;

	pB1 = static_cast<B1*>(pA1);	//编译不会报错, B类继承于A类(转换后, 原则上不能调用私有数据, 只能调用pulbic, 你也不应该打破这种原则, 不建议使用)
	pB1 = new B1;
	pA1 = static_cast<A1*>(pB1);	//编译不会报错, B类继承于A类(转换后, 原则上不能调用私有数据, 只能调用pulbic, 你也不应该打破这种原则, 不建议使用)
	//pC1 = static_cast<C1*>(pA1);//编译报错, C类与A类没有任何关系. error C2440: "static_cast": 无法从"A *"转换为"C *"

	void *p = pB1;
	pA1 = static_cast<A1*>(p);		//把void类型指针转换成目标类型的指针(不安全, 慎用, 在c++ 类中, 都不推荐使用void* 指针, 扔掉c 语言那套方式!!)
}





//4.const_cast(详细请参见: const_cast转换.cpp)
/*
	一、常量指针 被强转为: 非常量指针, 且仍然指向原来的对象;
	二、常量引用 被强转为: 非常量引用, 且仍然指向原来的对象;
	三、常量对象 被强转为: 非常量对象;
*/





//5.reinterpret_cast(很少用)
/*
	有着与C风格的强制转换同样的能力(因此在c++ 类转换中, 用得很少, 纯c 语言中, 又没有c 风格强转那么方便, 所以很少用):
		* 可以转化任何内置的数据类型为其他任何的数据类型;
		* 可以转化任何指针类型为其他的类型;
		* 可以转化内置的数据类型为指针, 无须考虑类型安全或者常量的情形;
	因此, 使用reinterpret_casts的代码很难移植;
	所以, 不到万不得已绝对不用;
*/





//6.dynamic_cast(常用)
/*
	编译时, 由编译器处理的强转:
		static_cast
		const_cast
		reinterpret_cast
	运行时, 由.exe 程序自己检查, 完成强转的(因此, 不能用于内置基本数据类型间的强制转换):
		dynamic_cast

	* 不能用于内置基本数据类型间的强制转换
	double dValue = 12.12;
	int nDValue = dynamic_cast<int>(dValue);//error C2680 : "int" : dynamic_cast 的目标类型无效. 目标类型必须是指向已定义类的指针或引用

	* dynamic_cast 基本只用在class 类型之间的转换;
		且使用dynamic_cast进行转换时, 基类中一定要有虚函数, 否则编译不通过;
		需要有虚函数的原因: 
			类中存在虚函数, 就说明它有想要让基类指针或引用指向派生类对象的必要, 此时转换才有意义;
		由于运行时类型检查需要运行时类型信息, 而这个信息存储在类的虚函数表中, 
		所以, 只有定义了虚函数的类才有虚函数表;

	* dynamic_cast转换若成功, 返回的是指向类的指针或引用; 若失败则会返回NULL;

	* 在类的转换时, 在类层次间进行上行转换时, dynamic_cast和static_cast的效果是一样的;
		在进行下行转换时, dynamic_cast具有类型检查的功能, 比static_cast更安全;
			向上转换即为: 指向子类对象的向上转换, 即将子类指针转化父类指针;
			向下转换的成败取决于将要转换的类型, 即要强制转换的指针所指向的对象实际类型与将要转换后的类型一定要相同, 否则转换失败;

	* 使用dynamic_cast的类型转换, 其转换结果几乎都是执行期定义(implementation-defined)
*/

class A2 {};
class B2 : public A2 {};
class C2 {};
void t6(void){
	A2 *pA2 = new A2;
	B2 *pB2;
	//pB2=dynamic_cast<B2*>(pA2); //编译错误error C2683: "dynamic_cast":"A2"不是多态类型
}

class A {
public:
	virtual void f() { cout << "A::f()" << endl; }
};

class B : public A {
public:
	void f() { cout << "B::f()" << endl; }
	void bf() { cout << "B::bf()" << endl; }
};

class C {
	void pp() { return; }
};

void t7(void){
	A* pAB = new B;								//pAB是A类型的指针指向一个B类型的对象
	A* pAA = new A;								//pAA是A类型的指针指向一个A类型的对象
	B* pB = nullptr;
	C* pC = nullptr;
	pB = dynamic_cast<B*>(pAB);		//结果为not nullptr, 向下转换成功, pAB指向的就是B类型的对象, 所以可以转换成B类型的指针;
	if(nullptr == pB)
		cout << "dynamic_cast :: nullptr" << endl;
	else
		cout << "dynamic_cast :: not nullptr" << endl;

	//等价于static_cast
	pB = static_cast<B*>(pAB);		//结果为not nullptr, 向下转换成功, pAB指向的就是B类型的对象, 所以可以转换成B类型的指针;
	if(nullptr == pB)
		cout << "static_cast :: nullptr" << endl;
	else
		cout << "static_cast :: not nullptr" << endl;

	pB = dynamic_cast<B*>(pAA);		//结果为nullptr, 向下转换失败;pAA指向的是A类型的对象, 所以无法转换为B类型的指针;
	if(nullptr == pB)
		cout << "dynamic_cast :: nullptr" << endl;
	else
		cout << "dynamic_cast :: not nullptr" << endl;

	//static_cast的不安全性测试
	pB = static_cast<B*>(pAA);		//结果为not nullptr, 向下转换成功;pAA指向的是A类型的对象, 竟然转换为B类型的指针!
	if(nullptr == pB)
		cout << "static_cast :: nullptr" << endl;
	else{
		cout << "static_cast :: not nullptr" << endl;//不安全性
		pB->f();										//A::f()
		pB->bf();										//B::bf()
	}
	//static_cast 在c++类转换中的不安全性:
	/*
		pB = static_cast<B*>(pAA); 
		向下转换结果为not nullptr. 
		pAA指向的是A类型的对象, 竟然可以转换为B类型的指针! 相当危险!
		(
			不能调用私有数据;
			public API方法不是线程安全函数, 也不能用;
			业务逻辑不允许, 也不能这样强转;
			所以:
				这种强转, 在c++ 类的类型转换中, 是没有什么用的;
		)
	*/

	pC = dynamic_cast<C*>(pAB);		//结果为nullptr, 向下转换失败;pAB指向的是B类型的对象, 所以无法转换为C类型的指针;
	if(nullptr == pC)
		cout << "dynamic_cast :: nullptr" << endl;
	else
		cout << "dynamic_cast :: not nullptr" << endl;

	//pC = static_cast<C*>(pAB);	//error C2440: "static_cast": 无法从"A *"转换为"C *" 与指向的类型无关;
																//转换要求reinterpret_cast、C样式转换或函数样式转换

	delete pAB;
	delete pAA;
	return;
}
//run out(执行结果):
/*
	dynamic_cast :: not nullptr
	static_cast :: not nullptr
	dynamic_cast :: nullptr
	static_cast :: not nullptr
	A::f()
	B::bf()
	dynamic_cast :: nullptr
*/

int main(void){
	t7();
	return 0;
}
