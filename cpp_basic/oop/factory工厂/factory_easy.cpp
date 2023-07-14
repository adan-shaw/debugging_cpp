//编译:
//		g++ -g3 factory_easy.cpp -o x



#include <iostream>



class Product{
	public:
		virtual ~Product(){};
	protected:
		Product(){};
	private:
};

//public 继承from 父类: 父类中的public, protected 直接移到子类中, 属性不变;
class sonProduct:public Product{
	public:
		//~Product(){};
		sonProduct(){ std::cout<<"sonProduct..."<<std::endl; }
		~sonProduct(){}
	protected:
		//Product(){};
	private:
};



class Factory{
	public:
		virtual ~Factory(){};
		virtual Product* CreateProduct() = 0;//带重写的虚函数API
	protected: 
		Factory(){};
	private:
};

//public 继承from 父类: 父类中的public, protected 直接移到子类中, 属性不变;
class sonFactory:public Factory{
	public:
		//~Factory(){};
		//virtual 虚函数重写了CreateProduct()
		Product* CreateProduct(){ return new sonProduct(); }

		sonFactory(){ std::cout<<"sonFactory..."<<std::endl; }
		~sonFactory(){}
	protected:
		//Factory(){};
	private:
};



int main(void){
	Factory* f = new sonFactory();					//创建工厂
	Product* p = f->CreateProduct();				//在工厂中创建产品
	delete p;
	delete f;
	return 0;
}
