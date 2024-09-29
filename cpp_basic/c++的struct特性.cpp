//编译:
//		g++ -g3 ./c++的struct特性.cpp -o x



/*
c/c++ 的struct 的主要区别:
	c++ 中的struct, 基本上就是用class 来重写的, 与class不同的是, 结构体包含的函数默认为public, 而不是private;
	所以在c++ 中, 你可以直接把struct 看成是一个没有私有函数的class;



c++ 改造struct 的原因:
	在C语言中, 结构体不能包含函数;
	在C++语言中, 面向对象的程序设计中, 对象具有状态(属性)和行为, 状态保存在成员变量中, 行为通过成员方法(函数)来实现; 

	在C语言中的结构体, 只能描述一个对象的状态, 不能描述一个对象的行为;
	在C++中的结构体, 考虑到C语言到C++语言过渡的连续性, 对结构体进行了扩展, C++的结构体可以包含函数;
	(这样, C++的结构体也具有类的功能, 与class不同的是, 结构体包含的函数默认为public, 而不是private)



总结:
	在c++ 中, 看到struct 写成了class, 并没有什么大惊小怪的, 你就把c++ 中的struct 当成class 来看待就行, 平常心;
	c++ 程序员, 也经常把class 当成struct 来用, 毫不掩饰那种生硬的OOP 思维, 理解一下就行(ps: 无聊至极);

	优点:
		* 方便内嵌函数做统计:
			struct 中封装的数据, 突然具备函数处理能力, 还能轻松做一些简单统计行为, 其实也非常方便;
			还能直观地告诉你, 我是struct, 我只做一些简单的统计工作, 适合被其它class 内嵌, how prefect? (bullshit)
			100% 符合OOP 思维,  how prefect? (bullshit)

		* 构造函数也非常方便地进行赋值行为, 省掉很多赋值语句;


	另外:
		由于c++ 这样大规模使用OOP 特性, 你就不需要考虑太多struct 体积大小的问题了,
		在c++ 语言中, 不需要考虑太多性能问题, 他们都这样写了, 有何性能可言, 直接根据OOP 思维实现业务逻辑就行;
		因为即便是:
			* 你十分苛刻的遵守语法规则写代码, 别人不遵守, 那实际上这么做是没有什么实际意义的, 不过你可以坚持你自己的做法;
*/



#include <cstdio>
#include <cassert>
#include <string>



struct student{
	std::string name;//反正都用c++ 了, 不要吝啬性能, 直接套个c++ 类作为数据容器(大肠包小肠, 非常搞笑, 完成业务功能就行)
	int age;
	bool sex;

	//默认构造函数(必须赋值, 防止丢数据, 一旦进入默认构造函数, 则报错)
	student(){
		perror("unassigned struct student{}");
		assert(0);
	}

	//赋值构造(只实现一种赋值函数, 省得麻烦, 搞太多也很麻烦)
	student(std::string a, int b, bool c) : name(a), age(b), sex(c) { 
		;
	}

	//print 汇报自己的基础info
	void say()
	{
		printf("I am %s, age = %d, sex = %d\n", name.c_str(), age, sex);
	}
};



int main(void){
	struct student adan("adan", 18, true);
	struct student shaw("shaw", 28, true);
	adan.say();
	shaw.say();
	struct student nobody();//只要有'()', 都不会触发默认构造函数
													//证明这种写法其实很弱鸡, 极度容易犯错, 你不犯错, 队员犯错-_- (写成这样子, 就有一个struct 没有被初始化了)
	struct student nobody2;	//默认构造函数, 触发断言(0)
	return 0;
}



