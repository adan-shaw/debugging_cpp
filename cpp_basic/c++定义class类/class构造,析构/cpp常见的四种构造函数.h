//c++ 常见的4 种构造函数
/*
	* 隐式构造函数
	* 显式构造函数
	* 拷贝构造函数(浅拷贝/深拷贝)
	* 赋值拷贝构造函数(运算符重载)

ps:
	- 析构函数(不重要, 可以不用理会)
*/

//c++ 构造函数要注意的要点
/*
	* 构造函数不能修改默认返回值
		构造函数不能修改默认返回值, 默认返回值为: 该class 类的实体指针!! [class&]

	* 不能前置改变构造函数的返回类型, 构造只能返回[class&], 并且是自动填写的!!
		'构造函数'不能是static 函数, 也不能是const 函数!! inline 函数更不可能;
*/

//c++ 构造函数的体外话
/*
	- 构造函数'初始化值'变量
	- 构造函数之间的互相调用
*/



#include <cstdio>
#include <cstring>



#define strlen_max (128)



//测试1: '显式构造函数'与'隐式构造函数'的区别
class Test1{
public:
	Test1(const int n){								//(隐式声明)构造函数
		num=n;
		fprintf(stderr,"Test1-(隐式声明)构造函数: num=%d\n",num);
	}
private:
	int num;
};

class Test2{
public:
	explicit Test2(const int n){			//explicit(显式声明)构造函数
		num=n;
		fprintf(stderr,"Test2-explicit(显式声明)构造函数: num=%d\n",num);
	}
private:
	int num;
};

class Test3{
public:
	Test3(const int n = 10){					//(隐式声明)构造函数+默认赋值
		num=n;
		fprintf(stderr,"Test3-(隐式声明)构造函数+默认赋值: num=%d\n",num);
	}
private:
	int num;
};





//测试2:'拷贝构造函数':
/*
	'拷贝构造函数'主要用在:
		创建新的new类实体时, 拷贝'某个实体类'中某些的值, 达到复制效果;
		具体拷贝哪些值, 需要程序员指明, 否则只像调用默认构造函数一样, 得到一个没有初始化的类实体;
*/
class Test5{
public:
	Test5(const int n = 555){					//(隐式声明)构造函数+默认赋值
		num=n;
		fprintf(stderr,"Test5-(隐式声明)构造函数+默认赋值: num=%d,num_cp=%d\n",num,num_cp);
	}

	Test5(const Test5 &m){						//浅拷贝构造函数
		num_cp=m.num_cp;
		fprintf(stderr,"Test5-浅拷贝构造函数: num=%d,num_cp=%d\n",num,num_cp);
	}

	//修改num_cp 变量
	void write_num_cp(const int num_cp){
		this->num_cp = num_cp;
	}
private:
	int num;													//这个值不拷贝
	int num_cp;												//指定拷贝这个值
};

class Test6{
public:
	Test6(const int n = 10){					//(隐式声明)构造函数+默认赋值
		num=n;
		text = new char[strlen_max];
		fprintf(stderr,"Test6-(隐式声明)构造函数+默认赋值: num=%d,num_cp=%d\n",num,num_cp);
	}

	Test6(const Test6 &m){						//深拷贝构造函数
		num_cp=m.num_cp;
		//新建一个字符串缓冲区, 用作'深拷贝':
		//	(指针拷贝, 不能直接'='等号修改指针值, 否则就不是完全拷贝, 这样会造成两个类, 共享一个指针实体, 是致命的逻辑错误)
		text = new char[strlen_max];
		strncpy(text,m.text,strlen_max);
		fprintf(stderr,"Test6-深拷贝构造函数: num=%d,num_cp=%d,text=%s\n",num,num_cp,text);
	}

	void write_num_cp(const int num_cp){	//修改num_cp 变量
		this->num_cp = num_cp;
	}

	void write_text(const char* ptext){		//修改text 变量
		strncpy(text,ptext,strlen_max);
	}
private:
	int num;													//这个值不拷贝
	int num_cp;												//指定拷贝这个值
	char* text;												//'深拷贝'这个指针实体(仅以字符串buf拷贝作为示范,其他深拷贝,自己对号入座)
};





//测试3:'构造函数'中调用'其他构造函数', 并初始化值
class Test7{
public:
	Test7() : Test7(1111) {};					//构造函数之间可以互相调用, 但不能调用'非构造函数'

	Test7(const int n)
	:num(9999),
	 num2(n){													//(隐式声明)构造函数+初始化赋值-'冒号竖式'
		fprintf(stderr,"Test7-(隐式声明)构造函数+初始化赋值-'冒号竖式': num=%d,num2=%d\n",num,num2);
	}
private:
	int num;
	int num2;
};

void test_7(void){
	fprintf(stderr,"\n\n\n'构造函数'中调用'其他构造函数', 并初始化值.[测试3]:\n");
	Test7 t71;
	return;
}





//测试4:'赋值拷贝构造函数'(即重载运算符号, 实现拷贝构造函数)
/*
例如:
	//重载'='号运算符, 相当与'拷贝构造函数':
	Test8 t81(t82)
	Test8 t81=t82;

	当然, 你还可以对其他运算符号, 进行特殊改造, 例如:
		Test8 t81=t82+t83;

	ps:
		如果是字符串容器的赋值拷贝, 那么需要先侦查字符串长度.
*/
class Test8{
public:
	Test8(const int n){								//(隐式声明)构造函数
		num=n;
		fprintf(stderr,"Test8-(隐式声明)构造函数: num=%d\n",num);
	}

	Test8(const Test8 &m){						//浅拷贝构造函数
		num=m.num;
		fprintf(stderr,"Test8-浅拷贝构造函数: num=%d\n",num);
	}

	Test8& operator+(const Test8 &m2){//'+'号运算符重载
		this->num = this->num + m2.num;
		fprintf(stderr,"Test8-运算符重载operator+, num=%d\n",num);
		return *this;
	}

	Test8& operator-(const Test8 &m2){//'-'号运算符重载
		this->num = this->num - m2.num;
		fprintf(stderr,"Test8-运算符重载operator-, num=%d\n",num);
		return *this;
	}

	Test8& operator*(const Test8 &m2){//'*'号运算符重载
		this->num = this->num * m2.num;
		fprintf(stderr,"Test8-运算符重载operator*, num=%d\n",num);
		return *this;
	}

	Test8& operator/(const Test8 &m2){//'/'号运算符重载
		this->num = this->num / m2.num;
		fprintf(stderr,"Test8-运算符重载operator/, num=%d\n",num);
		return *this;
	}

	Test8& operator=(const Test8 &m2){//'='号运算符重载
		this->num = m2.num;
		fprintf(stderr,"Test8-运算符重载operator=, num=%d\n",num);
		return *this;
	}

	Test8& operator+=(const Test8 &m2){//'+='号运算符重载
		this->num = this->num + m2.num;
		fprintf(stderr,"Test8-运算符重载operator+=, num=%d\n",num);
		return *this;
	}

	Test8& operator-=(const Test8 &m2){//'-='号运算符重载
		this->num = this->num - m2.num;
		fprintf(stderr,"Test8-运算符重载operator-=, num=%d\n",num);
		return *this;
	}

	Test8& operator*=(const Test8 &m2){//'*='号运算符重载
		this->num = this->num * m2.num;
		fprintf(stderr,"Test8-运算符重载operator*=, num=%d\n",num);
		return *this;
	}

	Test8& operator/=(const Test8 &m2){//'/='号运算符重载
		this->num = this->num / m2.num;
		fprintf(stderr,"Test8-运算符重载operator/=, num=%d\n",num);
		return *this;
	}

private:
	int num;
};





//测试5:'析构函数'
class Test9{
public:
	Test9(void){}
	~Test9(void){//析构函数
		fprintf(stderr,"Test9-析构函数: ~Test9\n");
	}
private:
	int num;
};

void test_9(void){
	fprintf(stderr,"\n\n\n'析构函数'[测试5]:\n");
	Test9 t9;
	return;
}
