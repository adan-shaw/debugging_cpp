//编译:
//		g++ -g3 ./c++内部类.cpp -o x



//内部类的概念
/*
	如果一个类定义在另一个类的内部, 这个内部类就叫做内部类;

	此时这个内部类是一个独立的类, 它不属于外部类, 更不能通过外部类的对象去调用内部类; 
	(外部类对内部类, 没有任何优越的访问权限, protected、private 不通用)
*/



//内部类注意事项
/*
	* sizeof(外部类)=外部类的size, 和内部类没有任何关系, sizeof() 不会包含内部类的size;
		(内部类是一个独立的类, 不属于外部类)

	* 内部类可以定义在外部类的public、protected、private都是可以的;

	* 内部类可以在'外部类'中声明, 在'外部类'外定义(形式上很像友元类)
		如果内部类定义在public, 则可通过 外部类名::内部类名, 来补充定义内部类;
		如果定义在protected/private, 则外部不可补充定义内部类, 这可实现"实现一个不能被继承的类"问题;

	* 内部类可以直接访问外部类中的static、枚举成员, 不需要外部类的对象/类名;
		(但内部类不能直接访问外部类的成员) -- 极度不推荐使用这种方式访问static 变量, 扰乱思维, 浪费时间内耗, 尽量别用
*/



#include <iostream>



class A1{
public:
	class B{};
};



class A2{
private:
	static int k;//(尽量别用)
	int h=0;
public:
	class B{
	public:
		void foo(A2 &a){
			std::cout<<k<<std::endl;	//OK
			//std::cout<<h<<std::endl;//ERROR
			std::cout<<a.h<<std::endl;//OK(尽量别用)
		}
	};
};

int A2::k=3;
//int A2::h=0;//不能直接访问private 的成员(尽量别用)



class A3{
private:
	static int i;//(尽量别用)
public:
	class B;
};

class A3::B{
public:
	void foo(){std::cout<<i<<std::endl;}//这里也不需要加A3::i(尽量别用)
};

int A3::i=3;



int main(void){
	A1 a1;
	A1::B *b = new A1::B();
	A2 a2;
	A2::B *b2 = new A2::B();
	b2->foo(a2);
	delete b;
	delete b2;
	return 0;
}
