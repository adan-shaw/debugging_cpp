//编译:
//		g++ -g3 ./factory_abstact.cpp -o x



//abstact_factory 抽象工厂模式: 
/*
	多层工厂内嵌, 最后必然会导致类数据抽象化;
	因为:
		底层基类, 肯定是面向具体的业务, 具体的数据, 一般都是具体的;
		高层类, 肯定是面向管理的, 管理底层基类, 一般都是抽象的;

	因此:
		只要类工厂堆叠2-3 层之后, 必然性会成为抽象类工厂;
		简单1-2 层, 可能还是简单类工厂, 这是自然现象;

	因此, 抽象类工厂, 其实没有啥别特的概念, 只是一个堆叠度较大的类工厂而已;
*/



#include <iostream>



//苹果的抽象
class AbstractApple{
	public:
		virtual void showName() = NULL;
};

//中国苹果(public 公有继承)
class ChinaApple:public AbstractApple{
	public:
		virtual void showName(){ std::cout << "中国苹果" << std::endl; }
};

//美国苹果
class USAApple:public AbstractApple{
	public:
		virtual void showName(){ std::cout << "美国苹果" << std::endl; }
};

//日本苹果
class JapanApple:public AbstractApple{
	public:
		virtual void showName(){ std::cout << "日本苹果" << std::endl; }
};



//香蕉的抽象
class AbstractBanana{
	public:
		virtual void showName() = NULL;
};

//中国香蕉(public 公有继承)
class ChinaBanana:public AbstractBanana{
	public:
		virtual void showName(){ std::cout << "中国香蕉" << std::endl; }
};

//美国香蕉
class USABanana:public AbstractBanana{
	public:
		virtual void showName(){ std::cout << "美国香蕉" << std::endl; }
};

//日本香蕉
class JapanBanana:public AbstractBanana{
	public:
		virtual void showName(){ std::cout << "日本香蕉" << std::endl; }
};



//鸭梨的抽象
class AbstractPear{
	public:
		virtual void showName() = NULL;
};

//中国鸭梨(public 公有继承)
class ChinaPear:public AbstractPear{
	public:
		virtual void showName(){ std::cout << "中国鸭梨" << std::endl; }
};

//美国鸭梨
class USAPear:public AbstractPear{
	public:
		virtual void showName(){ std::cout << "美国鸭梨" << std::endl; }
};

//日本鸭梨
class JapanPear:public AbstractPear{
	public:
		virtual void showName(){ std::cout << "日本鸭梨" << std::endl; }
};



//抽象工厂(针对产品族)
class AbstractFactory{
	public:
		virtual AbstractApple* CreateApple() = NULL;
		virtual AbstractBanana* CreateBanana() = NULL;
		virtual AbstractPear* CreatePear() = NULL;
};

//中国工厂
class ChinaFactory:public AbstractFactory{
	virtual AbstractApple* CreateApple(){ return new ChinaApple; }
	virtual AbstractBanana* CreateBanana(){ return new ChinaBanana; }
	virtual AbstractPear* CreatePear(){ return new ChinaPear; }
};

//美国工厂
class USAFactory:public AbstractFactory{
	virtual AbstractApple* CreateApple(){ return new USAApple; }
	virtual AbstractBanana* CreateBanana(){ return new USABanana; }
	virtual AbstractPear* CreatePear(){ return new USAPear; }
};

//日本工厂
class JapanFactory:public AbstractFactory{
	virtual AbstractApple* CreateApple(){ return new JapanApple; }
	virtual AbstractBanana* CreateBanana(){ return new JapanBanana; }
	virtual AbstractPear* CreatePear(){ return new JapanPear; }
};



int main(void){
	AbstractFactory* factory = NULL;
	AbstractApple* apple = NULL;
	AbstractBanana* Banana = NULL;
	AbstractPear* Pear = NULL;

	//实化为中国工厂, 生产产品: Apple, Banana, Pear
	factory = new ChinaFactory;
	apple = factory->CreateApple();
	Banana = factory->CreateBanana();
	Pear = factory->CreatePear();

	apple->showName();
	Banana->showName();
	Pear->showName();

	delete Pear;
	delete apple;
	delete Banana;
	delete factory;
	return 0;
}
