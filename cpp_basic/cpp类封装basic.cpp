//编译:
//		g++ -g3 -o x ./cpp类封装basic.cpp



//1.封装
/*
	c/c++ 封装要义:
			c 语言也有一点封装特性, 比如说全局变量, 局部变量, static 变量封装,
			c++ 在c 的基础上, 增加了一个类封装. 
			将大部分的封装问题压缩在类里面, 实现更好的<面向对象> 抽象编程思维.

	封装的用途:
		1.代码传递, 你将自己的代码传给别人使用的时候, 
			封装好的类就是最好的使用说明, 
			有经验的程序员不需要看你的详细实现也可以快速使用.

		2.使程序结构更简单, 更容易堆叠, 简单才能走得更远.

		3.防止自己忘记, 回归重塑的时候, 方便自己查阅.
			这个忘记自己写的逻辑的事情经常有发生, 如果没有封装, 这种情况会更糟糕.
			public, private, protected 区分也有助于回忆代码结构,
			增强代码结构划分, 提高阅读能力.

		ps: 随着时间的推移, 封装的作用在后期项目维护中越来越重要,
				这些都是前人的设计经验, c 代码会渐渐变得难以管理.

	类封装原则:
		1.函数返回:
			自定义函数尽量返回void, if 都省掉了, 也减少进栈出栈,
			有业务逻辑, 有if, 尽量在每个自定义函数内, 自己解决, 
			不要经常性返回一个检查值, 会影响性能;
			(当然你可以用assert() 来代替if, 提高性能和可测试性)

			cpp 有bool 值类型, c 语言只有int, 所以有:
				cpp->if(bool) = true = 1, false = 0;
				c	->if(int) !=  true = 0, false = 1;

	public, private, protected:
		善用这三个关键字, 以最小化原则, 
		尽量多的隐藏class 的自身运行变量,
		尽量少的向外暴露共享变量(最好只暴露操作API),
		这就是类封装的简要方法
*/



//2.继承
/*
	继承关系, 主要由public, private, protected 来决定;
	public:
		任意类, 都可以互相访问('类定义期间'的自我访问or '生成类实体后'互相访问)

	private:
		私有数据, 只能在'类定义期间'的自我访问('生成类实体后'互相访问, 需要在public 做跳板操作API)

	protected:
		保护数据, 类似private私有数据, 只能在'类定义期间'的自我访问, 
		或者派生类(即子类), 在'类定义期间'的自我访问
		('生成类实体后'互相访问, 需要在public 做跳板操作API)


	'类定义期间', 可以访问public, private, protected;
	'生成类实体后', 只能访问public, 访问private, protected 都需要做跳板;
	('生成类实体后', 自己访问自己的private, protected, 也是不行的!!)

	protected相当于: 可以继承的private


	继承级别:
		class father;
		class son : public father;		//公有级继承
		class son : protected father;	//保护级继承
		class son : private father;		//私有级继承
		详细继承关系, 请看: ./inherited类继承/cpp类继承tab.png


	//单一继承, 多继承的区别:
		单一继承:
			class father;
			class son : public father;

		多继承(特别容易产生覆盖混乱的问题):
			class father;
			class father2;
			class father3;
			class son : public father, father2, father3;


	通过继承创建的新类称为: "子类","派生类".
	被继承的类称为: "基类","父类","超类".

	在某些 OOP 语言中, 一个子类可以继承多个基类;
	但一般情况下, 一个子类只能有一个基类;
	要实现多重继承, 可以通过多级继承来实现.
*/



//3.多态
/*
	1.静多态(静多态通过'模板'和'函数重载'来实现)
		模板:
			编译时, 编译器发现你调用<模板函数>时, 
			自动生成将模板中的T替换为int类型(假设你实体化T = int), 从而实现模板功能,
			也间接实现了静多态;
			[在-E 预编译, 预处理过程中'识别变量类型']

		函数重载:
			如果两个函数名相同, 其参数的'类型/个数/顺序'不相同, 则成为重载函数;(编译器会自动根据程序逻辑,选择适合的函数,作为build烧写对象)
			重载函数与模板, 并没有本质上的区别,
			都是由编译器判断选择哪个函数, 编译器自动识别,
			从而实现了静多态;
			[在-E 预编译, 预处理过程中'选择重载函数']

		静多态是: 
			编码时, 代码格式固定; 
			编译时, 由编译器自动识别;



	2.动多态(动多态通过'继承', '虚函数(virtual)'来实现的)
		动多态是: 
			编码时, 由程序员自己写代码实现'继承'&'虚函数';
			编译时, 编译器照常编译(其实只要用了虚函数, 就必然牵涉到'函数重载');

		(大部分情况是: 静多态&动多态 一般会同时出现)

		继承:
			一般发生在子类, 父类之间,
			public, private, protected 数据, 自己分清楚, 不难;

		虚函数:
			虚函数的作用是:
				子类想要修改父类'public / protected'中的某个函数时, 以实现自己的特别功能, 这时候就会用到虚函数;
				当程序员编写了一个虚函数, 就意味着有两个同名函数: 父类原版函数+子类新建的虚函数, 这时候必然触发'函数重载';
				最终, 编译器在执行'函数重载'的时候, 
				父类实体会使用-父类原版函数
				子类实体会使用-子类新建的虚函数(local 原则)

			virtual在函数中的使用限制:
				构造函数不能是虚函数, 否则会出现编译错误. 
				普通函数不能是虚函数, 也就是说虚函数必须是父类的'public / protected'成员函数;
				全局函数不能是虚函数, 否则会导致编译错误;
				静态成员函数不能是虚函数, 否则会导致编译错误;
						(static成员函数是唯一的, n个类实体共享, 不可能成为虚函数, 与全局函数同理)
				内联函数不能是虚函数, 如果内联函数被virtual修饰, 计算机会忽略inline使它变成存粹的虚函数;
						(虚函数是子类特有, 不支持inline内嵌)
*/



//4.泛型(即模板类, 参数类型模板化)
/*
	泛型不属于c++ 面向对象的特性, 但泛型支撑起c++ 面向对象, 抽象描述的关键任务:
		类型自识别(一般是交给编译器识别[在-E 预编译, 预处理过程中识别], 
						 编译器识别后, 写机器码的时候, 也是固定类型的, 
						 只不过在编译期间, 自动识别类型, 更方便程序员做抽象描述, 更方便程序员做快速设计)

	泛型的好处:
		一个模板, 应付所有类型, 节约代码量, 也提高抽象编程的描述能力
*/



//5.虚函数表:
/*
	每个含有虚函数的类都有一个虚函数表(Virtual Table), 简称为V-Table;
	C++的编译器会保证'虚函数表的指针'存在于对象实例内存区域中的'最前面的位置'(这是为了保证取'虚函数表'时拥有最高的读取性能)

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



//6.虚函数覆盖重写分析:
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

	// 私有'变量/函数'不允许'运行时外部访问'(只允许在类定义期间, 内部访问)
	//x0.num = 1;

	return;
}



//虚函数测试太复杂, 请查看: ./virtual_func虚函数/virtual_demo.cpp



int main(void){
	//测试类继承
	test_class_inherited();

	//虚函数测试(略)
	return 0;
}
