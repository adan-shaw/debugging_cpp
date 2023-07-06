//编译
//		g++ -g3 ./vfunc简介.cpp -o x 



//虚函数简介:
/*
	虚函数的作用是:
		如果子类想要修改父类'public / protected'中的某个函数, 以实现自己的特别功能时, 这时候就会用到虚函数, 
		相当于对父类的某个函数进行重载, 虚函数表会新增一个新的函数, 来装载这个新成员;


	虚函数重载:
		当程序员编写了一个虚函数, 就意味着有两个同名函数: 
			父类原版函数+子类新建的虚函数, 这时候必然触发'函数重载';
		最终, 编译器在执行编译时, 会自动识别:
			父类实体会使用-父类原版函数
			子类实体会使用-子类新建的虚函数(local 原则)


	virtual虚函数中的使用限制:
		* 构造函数不能是虚函数
		* 普通函数不能是虚函数(虚函数必须是父类中的'public/protected'成员函数)
		* private 中不能定义虚函数
		* 全局函数不能是虚函数
		* 静态成员函数不能是虚函数
		* 内联函数不能是虚函数(如果内联函数被virtual修饰, 会忽略inline, 使它变成纯虚函数)
*/



#include <iostream>



// 定义一个函数指针类型
typedef void(*Func)(void);



class father_t1{
	public:
		void f(void){std::cout<<"普通函数: father_t1::f()"<<std::endl;}
		virtual void g(void){std::cout<<"虚函数: father_t1::g()"<<std::endl;}
		virtual void h(void){std::cout<<"虚函数: father_t1::h()"<<std::endl;}
};

class son_t1 : public father_t1{
	public:
		virtual void f(void){std::cout<<"虚函数: son_t1::f()"<<std::endl;}
		virtual void g(void){std::cout<<"虚函数: son_t1::g()"<<std::endl;}
		virtual void h2(void){std::cout<<"虚函数: son_t1::h2()"<<std::endl;}
};

void t1(void){
	father_t1 *pfather;
	son_t1 son;
	Func pF = NULL;
	std::cout << (long*)&son << std::endl;

	pF = (Func)*( (long*)*(long*)(&son)+0 );
	pF();

	pF = (Func)*( (long*)*(long*)(&son)+1 );
	pF();

	pF = (Func)*( (long*)*(long*)(&son)+2 );
	pF();

	pF = (Func)*( (long*)*(long*)(&son)+3 );
	pF();

	//pF = (Func)*( (long*)*(long*)(&son)+4 );
	//pF();

	//用子类指针调用'子类实体'中的g()
	son.g();

	//用父类指针调用'子类实体'中的g() {发现子类继承时, 已经修改虚函数了, 改成son->g();}
	pfather = (father_t1*)&son;
	pfather->g();

	//再读虚表, 就会Segmentation fault 内存溢出
	return;
}





class father_t2{
	public:
		virtual void f(void){std::cout<<"虚函数: father_t2::f()"<<std::endl;}
		virtual void g(void){std::cout<<"虚函数: father_t2::g()"<<std::endl;}
		virtual void h(void){std::cout<<"虚函数: father_t2::h()"<<std::endl;}
};

class father2_t2{
	public:
		virtual void f(void){std::cout<<"虚函数: father2_t2::f()"<<std::endl;}
		virtual void g(void){std::cout<<"虚函数: father2_t2::g()"<<std::endl;}
		virtual void h(void){std::cout<<"虚函数: father2_t2::h()"<<std::endl;}
};

class father3_t2{
	public:
		virtual void f(void){std::cout<<"虚函数: father3_t2::f()"<<std::endl;}
		virtual void g(void){std::cout<<"虚函数: father3_t2::g()"<<std::endl;}
		virtual void h(void){std::cout<<"虚函数: father3_t2::h()"<<std::endl;}
};

class son_t2 : public father_t2, father2_t2, father3_t2{
	public:
		virtual void f(void){std::cout<<"虚函数: son_t2::f()"<<std::endl;}
		virtual void g2(void){std::cout<<"虚函数: son_t2::g2()"<<std::endl;}
		virtual void h2(void){std::cout<<"虚函数: son_t2::h2()"<<std::endl;}
};

void t2(void){
	son_t2 son;
	Func pF = NULL;
	long **pvtab = (long**)&son;

	//son 继承father_t2
	pF = (Func)pvtab[0][0];//f() 被子类改写
	pF();

	pF = (Func)pvtab[0][1];
	pF();

	pF = (Func)pvtab[0][2];
	pF();

	//子类自身的g2(), h2()
	pF = (Func)pvtab[0][3];
	pF();

	pF = (Func)pvtab[0][4];
	pF();

	//son 继承father2_t2
	pF = (Func)pvtab[1][0];//f() 被子类改写
	pF();

	pF = (Func)pvtab[1][1];
	pF();

	pF = (Func)pvtab[1][2];
	pF();

	//son 继承father3_t2
	pF = (Func)pvtab[2][0];//f() 被子类改写
	pF();

	pF = (Func)pvtab[2][1];
	pF();

	pF = (Func)pvtab[2][2];
	pF();

	return;
}



int main(void){
	t1();
	t2();
	return 0;
}

















