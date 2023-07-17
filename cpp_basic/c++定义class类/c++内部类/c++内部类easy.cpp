//编译:
//		g++ -g3 ./c++内部类easy.cpp -o x



//本demo 主要展示什么是c++ 内部类: 实际上就是在类的内部, 定义'其它类的实体/指针'作为类成员, 非常简单&常见;



#include <iostream>



class B;

class A{
public:
	int i;
	B *b;
};

class B{
public:
	int i;
	A a;
};



//直接内嵌的写法(不推荐, 非常混乱的写法)
class C{
public:
	void test1(){ std::cout<<"test1"<<test->test2()<<std::endl; }
	class D{
	public:
		char* test2(){return "test2";}
	protected:
	private:
	};
	D *test;
	D test2;
};



int main(void){
	B b;
	A a;
	C c;
	C::D d;//直接内嵌, 创建被内嵌的子类的时候, 需要像namespace一样, 使用::域操作符, 才能找到这个类(并不是找不到, 找得到的)
	a.b = &b;
	c.test = &d;
	return 0;
}



