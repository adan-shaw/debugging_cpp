//编译
//		g++ -g3 ./virtual_demo.cpp -o x 



//单一继承, 多继承的区别:
/*
	单一继承:
		class father;
		class son : public father;

	多继承(特别容易产生覆盖混乱的问题):
		class father;
		class father2;
		class father3;
		class son : public father, father2, father3;
*/



//获取一个类的虚函数表:
/*
	每个含有虚函数的类都有一个虚函数表(Virtual Table), 简称为V-Table;
	c++的编译器会保证'虚函数表的指针'存在于对象实例内存区域中的'最前面的位置'(这是为了保证取'虚函数表'时拥有最高的读取性能)

	我们可以通过函数指针, 遍历V-Table中的所有函数, 并调用相应的函数;
	(但每个虚函数的'返回类型, 参数数量, 类型'等信息都不相同, 
	 访问时, 需要定义一个匹配的函数指针, 
	 这就意味着: 遍历V-Table 是十分艰巨的设计, 绝对不建议你这样做, 
	 访问V-Table, 实际编程中很少用, 知道这件事就好, 这里主要用来讲述虚函数的原理, 需要用到V-Table)

	你要知道的原理:
		* 每一个类都有虚函数列表
		* 虚表可以继承; 
			如果子类没有重写虚函数, 子类将会与父类共享同一个虚函数列表, 这样可以节省很多内存;
			(无论有多少个派生子类, 只要没有重写虚函数, 则会与父类共享同一个虚函数列表, 你想想节省了多少内存)
			如果重写了某个虚函数, 那么虚表中的地址就会改变, 新增的虚函数, 会加插到虚表中;
		* 子类独有的虚函数放在后面(后面插入子类最新修改的虚函数)

	因此, 虚函数也有另外一个作用:
		节省内存!!
	将子父类共性函数, 设置为虚函数, 可以节约内存!!
	(当然不同的类, 就没有可能节省内存了, 也没有必要谈论'虚函数的问题';
	 但只要是父类, 子类, 就一定有共性相关的部分, 节省内存也很不错!!
	 这点, c++ 做得比c 语言好很多;)

	ps: 静态变量, 本身就是唯一的, 本身就是节约内存的, 没必要进入虚表V-Table;


	访问虚函数表:
		class father;
		long **pl = (long**)&father;

	用函数指针访问虚函数:
		class father;
		long **pl = (long**)&father;

		typedef void(*Func)(void);
		Func pF = (Func)pl[0][1];
		pF();
	实际你很难知道, 虚表V-Table中, 哪个[x][y] 坐标, 是哪个函数? 而你又要定义什么样的函数指针去访问?
	所以, 这种访问方式, 很少用!!(仅用作测试)
*/



//虚函数覆盖重写分析:
/*
	* 父类的虚函数f(), 子类的虚函数f1(), 单一继承, 无覆盖; --> 结果: 
				虚函数等于普通函数, 无覆盖;
				互不相干, 父类指针调用父类函数, 子类指针调用'子类自身的函数'(不测试了, 懒得写代码)


	* 父类的普通函数f(), 子类的虚函数f(), 单一继承, 同名产生覆盖; --> 结果:
				普通函数f() 不写进V-Table虚表, 没有覆盖问题;
				父类, 子类各自'一份函数实体内存';
				互不相干, 父类指针调用父类函数, 子类指针调用'子类自身的函数'[ 请参考: 测试1->f() ]


	* 父类的虚函数g(), 子类的虚函数g(), 单一继承, 产生覆盖; --> 结果:
				V-Table虚表生成'一份新的函数实体内存';
				'子类实体'调用父类函数, 发现父类虚函数已经被篡改, 被子类虚函数取代;
				这样做既节省一点点内存, 又减少歧义的产生;[ 请参考: 测试1->g() ]


	* 父类的虚函数f(), 子类的虚函数f(), 多继承, 无覆盖; --> 结果:
				互不相干, 哪个父类的指针, 就调用哪个父类的函数, 
				子类指针, 调用'子类自身的函数';


	* 父类的虚函数f(), 子类的虚函数f(), 多继承, 产生覆盖; --> 结果:
				V-Table虚表生成'一份新的函数实体内存';
				'子类实体'调用父类函数, 发现父类虚函数已经被篡改, 被子类虚函数取代;
				多继承时, 只是每一个父类都会被篡改, 并没有什么特别的;
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

//测试1
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

//测试2
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
	std::cout << "测试1:" << std::endl;
	t1();

	std::cout << "测试2:" << std::endl;
	t2();

	return 0;
}












