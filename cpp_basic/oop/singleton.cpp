//编译:
//		g++ -std=c++11 -g3 singleton.cpp -o x



#include <iostream>



class A{
	public:
		static A* getInstace(){ return a; }

	private :
		A(){ a = new A; }
		static A* a;
};
A* A::a = NULL;



//懒汉式(对象的创建在第一次调用getInstance函数时创建), 懒汉式是线程不安全的
class SingletonLazy{
	public:
		static SingletonLazy* getInstance(){
			if(pSingleton == NULL)
				pSingleton = new SingletonLazy;
			return pSingleton;
		}
	private:
		SingletonLazy(){}
		static SingletonLazy* pSingleton;
};
//在类外面执行: 初始化
SingletonLazy* SingletonLazy::pSingleton=NULL;



//饿汉式(对象在程序执行时优先创建), 饿汉式是线程安全的
class SingletonHungry{
public:
	static SingletonHungry* getInstance(){ return pSingleton; }

	static void freeSpace(){
		if(pSingleton != NULL)
			delete pSingleton;
	}
private:
	SingletonHungry(){}
	static SingletonHungry* pSingleton;
};
//在main函数运行前执行: 创建实体 && 初始化
SingletonHungry* SingletonHungry::pSingleton=new SingletonHungry;



//测试函数
void test01(){
	SingletonLazy* p1 = SingletonLazy::getInstance();
	SingletonLazy* p2 = SingletonLazy::getInstance();
	SingletonHungry* p3 = SingletonHungry::getInstance();
	SingletonHungry* p4 = SingletonHungry::getInstance();


	if(p1 == p2)
		std::cout << "SingletonLazy: 单例模式" << std::endl;
	else
		std::cout << "SingletonLazy: 不是单例模式" << std::endl;

	if(p3 == p4)
		std::cout << "SingletonHungry: 单例模式" << std::endl;
	else
		std::cout << "SingletonHungry: 不是单例模式" << std::endl;
}



int main(void){
	test01();
	return 0;
}
