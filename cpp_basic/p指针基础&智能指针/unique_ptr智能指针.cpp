//编译:
//		g++ -std=c++11 -g3 ./unique_ptr智能指针.cpp -o x



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



//unique_ptr 智能指针的使用:
/*
	unique_ptr 对所指向的对象拥有"唯一"性:
		一个类实体, 一个unique_ptr;
		一个unique_ptr, 只能指向一个类实体(通过禁止拷贝语义、只有移动语义来实现);

	与原始指针相比, unique_ptr用于其RAII的特性, 使得在出现异常的情况下, 动态资源能得到释放;


	unique_ptr指针本身的生命周期:
		从unique_ptr指针创建时开始, 直到离开作用域;

	离开作用域时, 必然触发智能指针的构造函数释放, 如果指针非空, 则自动销毁指针指向的对象;
	(默认使用delete操作符, 用户可指定其他操作)
	[智能指针原理不明白, 请看: cpp智能指针的原理demo.cpp]

	离开作用域时, 如果是局部变量, 会自动释放,
	释放时, c++ 默认会调用构造函数释放类实体, 此时, 也是这样释放unique_ptr 智能指针的;


	unique_ptr指针与其所指对象的关系:
		在智能指针生命周期内, 可以改变智能指针所指对象, 如:
			- 创建智能指针时通过构造函数指定
			- 通过reset()方法重新指定
			- 通过release()方法释放所有权
			- 通过移动语义转移所有权


	使用技巧:
		* unique_ptr 等价于老式的auto_ptr;
		* shared_ptr 适合单例类实体, perfect, 类工厂实体, 用unique_ptr;
*/



#include <iostream>
#include <memory>
#include <string>



class Test{
public:
	Test(std::string s){
		str = s;
		std::cout << "Test creat\n" << std::endl;
	}

	~Test(){
		std::cout << "Test delete:" << str << std::endl;
	}

	std::string& getStr(){
		return str;
	}

	void setStr(std::string s){
		str = s;
	}

	void print(void){
		std::cout<<str<<std::endl;
	}

private:
	std::string str;
};



int main(void){
	std::unique_ptr<Test> ptest(new Test("123"));

	ptest->setStr("hello ");				//unique_ptr 智能指针调用类API
	ptest->getStr() += "world !";

	ptest->print();
	(*ptest).print();								//unique_ptr 智能指针调用类API(引用: 指针指向的值, 是类实体, 因此用'.')

	ptest.get()->print();						//unique_ptr 智能指针的get() 获取指针
	ptest.reset(new Test("123"));		//unique_ptr 智能指针的reset() 重置指针

	return 0;
}
