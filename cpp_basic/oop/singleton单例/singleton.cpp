//编译:
//		g++ -std=c++11 -g3 singleton.cpp -o x



#include <iostream>



//懒汉式(对象的创建在第一次调用getInstance函数时创建), 懒汉式是线程不安全的
class SingletonLazy{
public:
	static SingletonLazy* getInstance(){
		if(pSingleton == NULL)
			pSingleton = new SingletonLazy;
		return pSingleton;
	}
	static void freeSpace(){
		if(pSingleton != NULL)
			delete pSingleton;
		pSingleton = NULL;
	}

private:
	SingletonLazy(){}
	~SingletonLazy(){}
	static SingletonLazy* pSingleton;
};
//在类外面执行: 初始化
SingletonLazy* SingletonLazy::pSingleton = NULL;



//饿汉式(对象在程序执行时优先创建), 饿汉式是线程安全的
//饿汉式推荐使用: SingletonHungry实体变量版(非指针), 不用new, 直接使用static 变量来实化自身, 挺好用;
class SingletonHungry{
public:
	static SingletonHungry* getInstance(){ return pSingleton; }

	static void freeSpace(){
		if(pSingleton != NULL)
			delete pSingleton;
		pSingleton = NULL;
	}
private:
	SingletonHungry(){}
	~SingletonHungry(){}
	static SingletonHungry* pSingleton;
};
//在main函数运行前执行: 创建实体 && 初始化
SingletonHungry* SingletonHungry::pSingleton = new SingletonHungry;



int main(void){
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

	p1->freeSpace();
	p3->freeSpace();
	return 0;
}
