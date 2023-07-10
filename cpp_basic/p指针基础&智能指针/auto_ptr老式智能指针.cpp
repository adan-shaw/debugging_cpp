//编译:
//		g++ -g3 ./auto_ptr老式智能指针.cpp -o x



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
	std::auto_ptr<Test> ptest(new Test("123"));

	ptest->setStr("hello ");				//auto_ptr 智能指针调用类API
	ptest->getStr() += "world !";

	ptest->print();
	(*ptest).print();								//auto_ptr 智能指针调用类API(引用: 指针指向的值, 是类实体, 因此用'.')

	ptest.get()->print();						//auto_ptr 智能指针的get() 获取指针
	ptest.reset(new Test("123"));		//auto_ptr 智能指针的reset() 重置指针

	return 0;
}
