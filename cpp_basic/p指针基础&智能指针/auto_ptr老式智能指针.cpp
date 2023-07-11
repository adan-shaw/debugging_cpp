//编译:
//		g++ -g3 ./auto_ptr老式智能指针.cpp -o x



#include <iostream>
#include <memory>
#include <string>



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
	std::auto_ptr<Test> ptest(new Test("123"));

	ptest->setStr("hello ");				//auto_ptr 智能指针调用类API
	ptest->getStr() += "world !";

	ptest->print();
	(*ptest).print();								//auto_ptr 智能指针调用类API(引用: 指针指向的值, 是类实体, 因此用'.')

	ptest.get()->print();						//auto_ptr 智能指针的get() 获取指针
	ptest.reset(new Test("123"));		//auto_ptr 智能指针的reset() 重置指针

	return 0;
}
