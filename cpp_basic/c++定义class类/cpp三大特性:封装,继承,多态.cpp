//编译:
//		g++ -g3 ./cpp三大特性\:封装,继承,多态.cpp -o x 



//1.封装
/*
	c/c++ 封装要义:
		c 语言也有一点封装特性, 比如说全局变量, 局部变量, static 变量封装, 函数, 结构体, 等等;
		c++ 在c 的基础上, 增加了一个类封装, 
		一个类就能包罗万象, 将大部分的封装问题压缩在类里面, 实现更好的<面向对象>式抽象编程;



	封装的用途:
		1.代码传递, 你将自己的代码传给别人使用的时候, 
			封装好的类就是最好的使用说明, 有经验的程序员不需要看你的详细实现过程, 只需简单阅读一下类定义接口, 就可以快速使用;

		2.使程序结构更简单, 更容易堆叠, 简单才能走得更远;

		3.防止自己忘记, 回归重塑的时候, 方便自己查阅;
			忘记自己写的逻辑的事情经常发生, 如果没有封装, 这种情况会更糟糕;
			public, private, protected 区分也有助于回忆代码结构, 增强代码结构划分, 提高阅读能力;

		ps:
			随着时间的推移, 封装的作用在后期项目维护中越来越重要, 这些都是前人的设计经验, c 代码会渐渐变得难以管理;
			这也是面向对象编程的优势, 面向对象编程可以实现敏捷开发, 非常适合处理业务逻辑;



	类封装一般约定:
		* 函数返回值:
			自定义函数尽量返回void, bool都省掉了, 减少if 分支, 也减少返回值参数的进栈出栈,
			有业务逻辑, 尽量在每个自定义函数内实现, 不要经常性返回一个检查值, 
			这样不但会影响程序性能, 还会产生很多小函数, 非常难维护;
			(可以用assert() 来代替if, 提高程序的可靠性, 方便debug, 禁用后又能提高性能, 减少if 分支)

		* 函数形参:
			尽量明确形参类型, 是常量, 还是变量,
			是否需要赋默认值,
			* 只传递变量的值, 最好形参定义为const type, 类型不能变, 值可变;
			* 传递变量的地址, 引用or 指针, 都不加const;
				(用这种粗俗的方式, 进行约定, 实现函数形参的默认约定)

		* cpp 有bool 值类型, c 语言只有int, 所以有:
			cpp->if(bool) = true = 1, false = 0;
			  c->if(int) != true = 0, false = 1;

		* public, private, protected:
			用最小化原则, 进性类成员的隐藏;
			尽量多的隐藏class 的自身运行变量;
			尽量少的向外暴露共享变量(最好只暴露操作API, 且API 不应过度拆散, 最好集成一条较大的API, 这样最好维护);

		* 函数实现过程:
			尽量遵守线程安全函数的标准, 编写线程安全函数;
			在实现业务的基础上, 尽量少产生预测分支;
*/





//2.继承
/*
	继承关系, 主要由public, private, protected 来决定;
	public:
		会继承, 外部可访问这部分成员;
	protected:
		会继承, 外部不可访问这部分成员;
	private:
		不会继承, 外部不可访问这部分成员;

	public, private, protected 访问规则:
		请看: class访问规则demo.cpp



	继承级别(以单一继承为例):
		class father;
		class son1 : public father;			//public 公有级继承, 父类中的public, protected 直接移到子类中, 属性不变;
		class son2 : protected father;	//protected 保护级继承, 父类中的public, protected 都变成了子类中成为protected;
		class son3 : private father;		//private 私有级继承, 父类中的public, protected 都变成了子类中成为private;
		详细继承关系, 请看: ./inherited类继承/cpp类继承tab.png


	单一继承, 多继承的区别:
		单一继承:
			class father;
			class son : public father;

		多继承(特别容易产生覆盖混乱的问题, 自己设计时, 应当避免, 无法避免时要细心处理):
			class father;
			class father2;
			class father3;
			class son : public father, father2, father3;


	通过继承创建的新类称为: "子类","派生类"
	被继承的类称为:        "基类","父类","超类"

	在某些 OOP 语言中, 一个子类可以继承多个基类;
	但一般情况下, 一个子类只能有一个基类;
	要实现多重继承, 可以通过多级继承来实现(少用, 尽量就别搞这么复杂了);
*/





//3.多态(大部分情况是: 静多态&动多态 会同时出现的)
/*
	封装是基本, 
	继承是补充,
	多态是现象(解析出现这种情况的原因);



	1.静多态(静多态通过'模板'和'函数重载'来实现, 由编译器自动识别)
		模板:
			编译时, 编译器发现你调用<模板函数>时, 
			会自动生成将模板中的T替换为int类型(假设你实体化T = int), 从而实现模板功能, 也间接实现了静多态;
			[在-E 预编译, 预处理过程中'识别变量类型']

		函数重载:
			如果两个函数名相同, 其参数的'类型/个数/顺序'不相同, 则成为重载函数;
			(编译器会自动根据程序逻辑,选择适合的函数,作为build烧写对象)
			重载函数与模板, 并没有本质上的区别,
			都是由编译器判断选择哪个函数, 编译器自动识别, 从而实现了静多态;
			[在-E 预编译, 预处理过程中'选择重载函数']



	2.动多态(动多态通过'继承', '虚函数(virtual)'来实现的; 编译时, 编译器照常编译, 并不会做特殊处理)
		继承:
			一般发生在子类, 父类之间, 不同的继承方式, 有不同的子类产生, 是动多态产生原因的一种;
			public公有级继承, private私有级继承, protected保护级继承, 
			继承后, 父类的类成员, 在子类中, 都有不一样的public, private, protected 属性;

			最终, 继承在动多态中的作用, 主要是表现在:
				产生不同的子类public, private, protected 属性;


		虚函数:
			虚函数的作用是:
				如果子类想要修改/重写父类'public / protected'中的某个函数, 以实现自己的特别功能时,
				这时候就会用到虚函数, 相当于对父类的某个函数进行重载, 虚函数表会新增一个新的函数, 来装载这个新成员;

			虚函数重载:
				当程序员编写了一个虚函数, 就意味着有两个同名函数: 父类原版函数+子类新建的虚函数, 
				这时候必然触发'函数重载';
				最终, 编译器在执行编译时, 会自动识别:
					父类实体会使用-父类原版函数
					子类实体会使用-子类新建的虚函数(local 原则)

			最终, 虚函数在动多态中的作用, 主要是表现在:
				允许子类修改/重写父类'public / protected'中的某个函数
*/



#include <cstdio>
#include <cstring>

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



int main(void){
	test_class_inherited();
	return 0;
}
