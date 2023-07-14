//编译:
//		g++ -std=c++11 -g3 ./weak_ptr智能指针.cpp -o x



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



//weak_ptr 智能指针的使用:
/*
	weak_ptr 是为了配合shared_ptr 而引入的一种智能指针, 
	它不具有普通指针的行为, 没有重载operator*和->,
	它的最大作用在于协助shared_ptr工作, 像旁观者那样观测资源的使用情况(即: 对'单例资源'进行只读操作);

	weak_ptr 可以从一个shared_ptr, 或从另一个weak_ptr对象构造, 获得资源的观测权;

	但weak_ptr没有共享资源, 它的构造, 不会引起shared_ptr 指针引用计数的增加;


	weak_ptr 关键API:
		使用weak_ptr的成员函数use_count(), 可以观测shared_ptr 资源的引用计数;

		使用weak_ptr的成员函数expired(), 等价于if(use_count()==0), 但速度更快, 表示:
			被观测的shared_ptr 资源已不存在了(shared_ptr use_count()=0 时会自动释放资源);

		使用weak_ptr的成员函数lock(), 可从被观测的shared_ptr 中, 获得一个可用的shared_ptr对象, 从而进行写操作;
		(但当expired()==true的时候, lock()函数将返回一个存储空指针的shared_ptr, 即无效指针, 不能操作)


	总结:
		weak_ptr 指针适合操作shared_ptr 资源, 别无他用;
*/



#include <iostream>
#include <memory>



int main(void){
	std::shared_ptr<int> sPtr = std::make_shared<int>(10);
	std::weak_ptr<int> wPtr(sPtr);
	std::shared_ptr<int> sPtr3;

	std::cout << sPtr.use_count() << std::endl;		//一个shared_ptr, 一个weak_ptr, 但use_count()=1

	if(!wPtr.expired()){
		std::shared_ptr<int> sPtr2 = wPtr.lock();		//lock() 返回一个shared_ptr 指针, 新建一个shared_ptr 指针, 赋值接收lock()的返回
																								//(不推荐, gcc 会进行cpu 偷步优化, 弃用)[use_count()计数+1]

		sPtr3 = wPtr.lock();												//'='赋值拷贝shared_ptr 指针(推荐)[use_count()计数+1]

		//sPtr3(wPtr.lock());												//构造函数传递拷贝shared_ptr 指针(错误, 不能用这种方式!! 编译出错)

		*sPtr3 = 100;																//weak_ptr 引用指向的实体, 然后进行写操作(需要先lock(), 但不需要unlock())

		std::cout << wPtr.use_count() << std::endl;	//三个sPtr, use_count()=3
	}

	//delete memory(自动释放资源: 自动释放weak_ptr && shared_ptr)

	return 0;
}



