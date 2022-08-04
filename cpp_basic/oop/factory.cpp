#include <iostream>



class Product {
	public:
		virtual ~Product(){}
	protected:
		Product(){}
	private:
};

class ConcreteProduct:public Product{
	public:
		~ConcreteProduct(){}
		ConcreteProduct(){ std::cout<<"ConcreteProduct..."<<std::endl; }
	protected:
	private:
};



class Factory{
	public:
		virtual ~Factory(){}
		virtual Product* CreateProduct() = 0;
	protected: 
		Factory(){}
	private:
};

class ConcreteFactory:public Factory{
	public:
		~ConcreteFactory(){}
		ConcreteFactory(){ std::cout<<"ConcreteFactory..."<<std::endl; }
		Product* CreateProduct(){ return new ConcreteProduct(); }
	protected:
	private:
};



int main(void){
	Factory* fac = new ConcreteFactory();
	Product* p = fac->CreateProduct();
	delete p;
	delete fac;
	return 0;
}
