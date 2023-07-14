//编译:
//		g++ -std=c++11 -g3 ./shared_ptr智能指针.cpp -o x



//cpp 智能指针简介<memory>
/*
	cpp98 只有一个智能指针auto_ptr, 但不建议使用!!(可用unique_ptr 完美替代, 直接改名字即可)

	cpp11 有shared_ptr, weak_ptr, unique_ptr 三个智能指针, 其中:
		* unique_ptr 用来取代老式的auto_ptr;
		* shared_ptr, weak_ptr 是联用指针, 一般配套使用, 没有其它用法;
	因此, cpp11 实际智能指针也只有两种用法:
		* 单例模式, 用: shared_ptr + weak_ptr 组合;
		* 工厂模式, 用: unique_ptr [c++11已弃用了老式auto_ptr 智能指针]
*/



//shared_ptr 智能指针的使用:
/*
	* shared_ptr 常用在多个指针指向相同的对象(多个指针共享一个对象的情况);

	* shared_ptr 使用引用计数:
		每增加一个shared_ptr 指针拷贝(这些指针都指向同一个实体), 内部的引用计数加1;
		每析构一次, 内部的引用计数减1;
		减为0时, 自动删除所指向的堆内存;
		(shared_ptr 内部的引用计数是线程安全的, 但共享对象时产生的数据读写安全问题, 还是需要自己设计)

	使用细节:
		* 不能将指针直接赋值给一个智能指针, 智能指针是一个类, 不是一根指针!!
		# shared_ptr 错误的初始化写法
		std::shared_ptr<int> p4 = new int(1);

		shared_ptr 初始化:
			shared_ptr 是个模板类, 可以指定类型, 创建未初始化的shared_ptr, 后面调用构造函数进行初始化;
			(也可以使用make_shared() 函数进行shared_ptr 初始化)

		* get()函数获取原始指针

		* 一根原始指针, 不能初始化多个shared_ptr, 否则会造成二次释放同一内存(free 空指针程序会崩溃);

		* 避免循环引用, shared_ptr 的一个最大的陷阱是循环引用;
			循环引用会导致堆内存无法正确释放, 导致内存泄漏;(循环引用在weak_ptr中介绍)

		* 一旦同类型type 的shared_ptr 智能指针, 进行'='赋值传递, 就会出现此消彼长的情况:
				class A a;
				class A aa;
				std::shared_ptr<A> p(a);
				std::shared_ptr<A> pp(aa);
				a=aa;
				此时:
					p 智能指针计数为0, 自动释放;
					pp 智能指针计数为2, 继续使用;
				因此std::shared_ptr 智能指针容易出现: 此消彼长, 也很容易导致内存泄漏;
				因此:
					* 如果使用std::shared_ptr, 最好指向的实体对象, 只有一个, 永远没有第二个; 
					* 不能在两个同类型的实体之间, 传递std::shared_ptr 智能指针;
						(这样传递, 逻辑非常混乱, 别下折腾, 用智能指针就是图方便的, 比new/delete 还复杂, 没意义)
					* 同一个实体之间, '='赋值传递std::shared_ptr, 引用计数会+1;
					* 同一个实体之间, weak_ptr 智能指针'='赋值传递, 引用计数不会变(与weak_ptr智能指针联用时, 计数不变);

		* shared_ptr 适合单例类实体, perfect, 类工厂实体, 用unique_ptr;

		* shared_ptr 一般只在创建shared_ptr 智能指针时出现, 
			操作shared_ptr 智能指针, 一般用weak_ptr 智能指针;
			详情自己阅读: weak_ptr智能指针.cpp
*/



#include <iostream>
#include <memory>



class B;

class A{
public:
	std::shared_ptr<B> sPtr_b;
	A(){};
	~A(){ std::cout << "~A()" << std::endl; };
};

class B{
public:
	std::shared_ptr<A> sPtr_a;
	B(){};
	~B(){ std::cout << "~B()" << std::endl; };
};

//std::shared_ptr 智能指针: 循环引用的问题(导致内部计数永远不为0, 因此无法释放, 造成内存泄漏)
//ps: 进程结束时, 同样会被内核回收, 但进程不接受的话, 内存消耗一直存在
void test(void){
	std::shared_ptr<A> sPtr = std::make_shared<A>();
	std::shared_ptr<B> sPtr2 = std::make_shared<B>();
	a->sPtr_b = b;
	b->sPtr_a = a;
	return;
}



int main(void) {
	int a = 10, b = 20;
	int *pb = &a;

	std::shared_ptr<int> sPtr = std::make_shared<int>(a);		//make_shared() 初始化shared_ptr
	std::shared_ptr<int> sPtr2(sPtr);												//构造函数赋值拷贝
	//std::shared_ptr<int> sPtr2 = pb;											//error(智能指针是一个类, 不能直接赋值变量指针)
	std::shared_ptr<int> sPtr3 = std::make_shared<int>(b);

	std::cout << "a: " << sPtr.use_count() << "-" << sPtr2.use_count() << std::endl;
	std::cout << "b: " << sPtr3.use_count() << std::endl;		//打印shared_ptr 的引用计数

	sPtr2 = sPtr3;																					//assign shared_ptr 智能指针之间的赋值拷贝

	pb = sPtr3.get();																				//获取原始指针

	std::cout << "a: " << sPtr.use_count() << std::endl;
	std::cout << "b: " << sPtr3.use_count() << "-" << sPtr2.use_count() << std::endl;

	return 0;
}






