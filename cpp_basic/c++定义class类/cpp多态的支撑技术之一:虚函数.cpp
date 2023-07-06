//编译:
//		g++ -g3 ./cpp多态的支撑技术之一\:虚函数.cpp -o x



//1.虚函数表简介:
/*
	每个含有虚函数的类都有一个虚函数表(Virtual Table), 简称为V-Table;
	C++的编译器会保证'虚函数表的指针', 存在于对象实例内存区域中的'最前面的位置'
	(这是为了保证取'虚函数表'时拥有最高的读取性能)

	我们可以通过函数指针, 遍历V-Table中的所有函数, 并调用相应的函数;
	(但每个虚函数的'返回类型, 参数数量, 类型'等信息都不相同, 
	 访问时, 需要定义一个匹配的函数指针, 这就意味着: 
	   遍历V-Table 是十分艰巨的设计, 绝对不建议你这样做;
	 访问V-Table, 实际编程中很少用, 知道这件事就好, 这里主要用来讲述虚函数的原理, 需要用到V-Table)


	你要知道的原理:
		* 每一个类都有虚函数列表
		* 虚表可以继承; 
			如果子类没有重写虚函数, 子类将会与父类共享同一个虚函数列表, 这样可以节省很多内存;
			(无论有多少个派生子类, 只要没有重写虚函数, 则会与父类共享同一个虚函数列表, 你想想节省了多少内存)
		* 如果子类重写了某个虚函数, 那么虚表中的地址就会改变, 新增的虚函数, 会加插到虚表中;
			(子类独有的虚函数, 会放在的最后面)

	虚函数的一个重要作用: 有继承关系的父类, 子类, 共享一个虚表, 节省内存!!

	ps:
		c 语言中的静态变量, 本身就是唯一的, 本身就是节约内存的, 
		因此, 静态变量数据没必要进入虚表V-Table, 静态变量数据也不能定义为'虚表V-Table中的数据';
*/



//2.访问虚函数表
/*
	访问虚函数表:
		class father;
		long **pl = (long**)&father;

	用函数指针访问虚函数:
		class father;
		long **pl = (long**)&father;

		typedef void(*Func)(void);
		Func pF = (Func)pl[0][1];
		pF();

	实际上你很难知道'虚表V-Table'中, 哪个[x][y] 坐标, 是哪个函数? 
	而且, 你还要定义一个匹配的函数指针(函数指针需要批评参数类型, 返回参数类型, 函数名), 才能正确访问到该虚函数;
	因此, 使用函数指针, 指向'虚表V-Table', 再通过函数指针去访问函数的方式, 很少用!!(仅用作测试)
*/



//3.虚函数覆盖重写分析:
/*
	* 父类的虚函数f(), 子类的虚函数f1(), 单一继承, 无覆盖; --> 结果: 
			虚函数等于普通函数, 无覆盖;
			互不相干, 父类指针调用父类函数, 子类指针调用'子类自身的函数'
			(不测试了, 懒得写代码)


	* 父类的普通函数f(), 子类的虚函数f(), 单一继承, 同名产生覆盖; --> 结果:
			普通函数f() 不写进V-Table虚表, 没有覆盖问题;
			父类, 子类各自'一份函数实体内存';
			互不相干, 父类指针调用父类函数, 子类指针调用'子类自身的函数'
			[ 请参考: 测试1->f() ]


	* 父类的虚函数g(), 子类的虚函数g(), 单一继承, 产生覆盖; --> 结果:
			V-Table虚表生成'一份新的函数实体内存';
			'子类实体'调用父类函数, 发现父类虚函数已经被篡改, 被子类虚函数取代;
			这样做既节省一点点内存, 又减少歧义的产生;
			[ 请参考: 测试1->g() ]


	* 父类的虚函数f(), 子类的虚函数f(), 多继承, 无覆盖; --> 结果:
			互不相干, 哪个父类的指针, 就调用哪个父类的函数, 
			子类指针, 调用'子类自身的函数';


	* 父类的虚函数f(), 子类的虚函数f(), 多继承, 产生覆盖; --> 结果:
			V-Table虚表生成'一份新的函数实体内存';
			'子类实体'调用父类函数, 发现父类虚函数已经被篡改, 被子类虚函数取代;
			多继承时, 只是每一个父类都会被篡改, 并没有什么特别的;
*/



#include <stdio.h>
#include <string.h>

//父类father
class father{
public:
	//father(): father(99,88) {}
	father(const int n = 10, const int m = 20){
		num = n;
		num2 = m;
		num3 = n*m;
		fprintf(stderr,"father: num=%d,num2=%d,num3=%d\n",num,num2,num3);
	}
	void print_text(void){
		fprintf(stderr,"class father print_text()\n");
	}
	int num3;

private:
	int num;

protected:
	int num2;
};


//子类son, public公有继承父类father
class son : public father{
public:
	//子类必须调用: father(2,2) 构造函数, 初始化继承类部分
	son(const int n = 100, const int m = 200):father(1,1){
		x1 = n;
		x11 = m;
		//公有继承int num3
		fprintf(stderr,"son: x1=%d,x11=%d,num3=%d\n",x1,x11,num3);
		print_text();
	}
private:
	int x1;

protected:
	int x11;
};


//子类son2, protected保护继承父类father
class son2 : protected father{
public:
	//子类必须调用 : father(2,2) 构造函数, 初始化继承类部分
	son2(const int n = 101, const int m = 201):father(2,2){
		x2 = n;
		x22 = m;
		//公有继承int num3, 保护继承int num2
		fprintf(stderr,"son2: x2=%d,x22=%d,num2=%d,num3=%d\n",x2,x22,num2,num3);
		print_text();
	}
private:
	int x2;

protected:
	int x22;
};


//子类son3, private私有继承父类father
class son3 : private son2{
public:
	//子类必须调用 : father(2,2) 构造函数, 初始化继承类部分
	son3(const int n = 1000, const int m = 2000){
		x3 = n;
		x33 = m;
		//公有继承int num3, 保护继承int num2, 保护继承int x22
		fprintf(stderr,"son3: x3=%d,x33=%d,num2=%d,num3=%d,x22=%d\n",x3,x33,num2,num3,x22);
		print_text();
	}
private:
	int x3;

protected:
	int x33;
};



//测试类继承
void test_class_inherited(void){
	father* p0;
	son* p1;
	son2* p2;
	son3* p3;

	fprintf(stderr,"封装, 继承[测试1]\n");
	father x0(-1,-1);
	fprintf(stderr,"\n\n");
	son x1(1,1);
	fprintf(stderr,"\n\n");
	son2 x2(2,2);
	fprintf(stderr,"\n\n");
	son3 x3(3,3);
	fprintf(stderr,"\n\n");


	fprintf(stderr,"封装, 继承之间, 指针乱串(父类指针, 子类指针)[测试2]\n");
	fprintf(stderr,"父类指针, 指向子类, 同样调用出public 变量\n");
	p0 = &x1;
	p0->print_text();
	fprintf(stderr,"public int value num3 = %d\n",p0->num3);

	fprintf(stderr,"子类指针, 指向父类, 同样调用出public 变量\n");
	p1 = (son*)&x0;
	p1->print_text();
	fprintf(stderr,"public int value num3 = %d\n",p1->num3);


	//跨代访问失败
	/*
		p0 = (son3*)&x3;//父类指针, 指向孙类, 同样调用出public 变量
		p0->print_text();
		fprintf(stderr,"public int value num3 = %d\n",p0->num3);

		p3 = (son3*)&x0;//父类指针, 指向孙类, 同样调用出public 变量
		p3->print_text();
		fprintf(stderr,"public int value num3 = %d\n",p3->num3);
	*/


	//私有'变量/函数'不允许'运行时外部访问'(只允许在类定义期间, 内部访问)
	//x0.num = 1;

	return;
}



int main(void){
	test_class_inherited();
	return 0;
}
