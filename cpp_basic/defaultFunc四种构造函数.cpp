//编译:
//		g++ -g3 ./defaultFunc四种构造函数.cpp -o x



//摘要:
/*
	隐式构造函数
	显式构造函数
	拷贝构造函数(浅拷贝/深拷贝)
	赋值拷贝构造函数(运算符重载)
	析构函数


	构造函数'初始化值'变量
	构造函数之间的互相调用


	构造函数不能修改默认返回值, 默认返回值为: 类实体指针!! [class&]
	'构造函数'不能是static 函数, 也不能是const 函数!! inline 函数更不可能
*/



#include <cstdio>
#include <cstring>



#define strlen_max 128



//测试1: '显式构造函数'与'隐式构造函数'的区别
class Test1{
	public:
		//(隐式声明)构造函数
		Test1(const int n){
			num=n;
			fprintf(stderr,"Test1-(隐式声明)构造函数: num=%d\n",num);
		}
	private:
		int num;
};

class Test2{
	public:
		//explicit(显式声明)构造函数
		explicit Test2(const int n){
			num=n;
			fprintf(stderr,"Test2-explicit(显式声明)构造函数: num=%d\n",num);
		}
	private:
		int num;
};

class Test3{
	public:
		//(隐式声明)构造函数+默认赋值
		Test3(const int n = 10){
			num=n;
			fprintf(stderr,"Test3-(隐式声明)构造函数+默认赋值: num=%d\n",num);
		}
	private:
		int num;
};

//测试1
void test_123(void){
	fprintf(stderr,"'显式构造函数'与'隐式构造函数'的区别[测试1]:\n");

	//Test1 t1;
	Test1 t1 = 12;
	//Test1 t1(999);

	//Test2 t2;
	//Test2 t2=12;
	Test2 t2(12);

	//没有默认赋值, 就必须赋值初始化类, 才能通过编译
	Test3 t3;
	Test3 t33 = 9999888;
	//Test3 t33(1110);
	return ;
}





//测试2:'拷贝构造函数':
/*
	主要用在:
		创建新的new类实体时, 拷贝'某个实体类'中某些的值, 达到复制效果;
		具体拷贝哪些值, 需要程序员指明, 
		否则只像调用默认构造函数一样, 得到一个没有初始化的类实体;
*/
class Test5{
	public:
		//(隐式声明)构造函数+默认赋值
		Test5(const int n = 555){
			num=n;
			fprintf(stderr,"Test5-(隐式声明)构造函数+默认赋值: num=%d,num_cp=%d\n",num,num_cp);
		}

		//浅拷贝构造函数
		Test5(const Test5 &m){
			num_cp=m.num_cp;
			fprintf(stderr,"Test5-浅拷贝构造函数: num=%d,num_cp=%d\n",num,num_cp);
		}

		//修改num_cp 变量
		void write_num_cp(const int num_cp){
			this->num_cp = num_cp;
		}
	private:
		int num;		//这个值不拷贝
		int num_cp;	//指定拷贝这个值
};

class Test6{
	public:
		//(隐式声明)构造函数+默认赋值
		Test6(const int n = 10){
			num=n;
			text = new char[strlen_max];
			fprintf(stderr,"Test6-(隐式声明)构造函数+默认赋值: num=%d,num_cp=%d\n",num,num_cp);
		}

		//深拷贝构造函数
		Test6(const Test6 &m){
			num_cp=m.num_cp;
			//新建一个字符串缓冲区, 用作'深拷贝':
			//	(指针拷贝, 不能直接'='等号修改指针值, 
			//	 否则就不是完全拷贝, 这样会造成两个类, 共享一个指针实体, 是致命的逻辑错误)
			text = new char[strlen_max];
			strncpy(text,m.text,strlen_max);
			fprintf(stderr,"Test6-深拷贝构造函数: num=%d,num_cp=%d,text=%s\n",num,num_cp,text);
		}

		//修改num_cp 变量
		void write_num_cp(const int num_cp){
			this->num_cp = num_cp;
		}

		//修改text 变量
		void write_text(const char* ptext){
			strncpy(text,ptext,strlen_max);
		}
	private:
		int num;		//这个值不拷贝
		int num_cp;	//指定拷贝这个值
		char* text;	//'深拷贝'这个指针实体(仅以字符串buf拷贝作为示范,其他深拷贝,自己对号入座)
};

//测试2
void test_56(void){
	char test_text[] = "dont fucking kidding me !!";
	fprintf(stderr,"\n\n\n'拷贝构造函数'[测试2]:\n");

	//浅拷贝(没有指针实体拷贝)
	Test5 t51;
	t51.write_num_cp(999);//改写num_cp 拷贝目标值
	Test5 t52(t51);				//用'拷贝构造函数'的方式创建新的类t52.[没有被拷贝的值num = 随机数]
												//ps: 由于gnu stl内存重用的原则,t52类总是重用上一次测试剩下的t33,
												//		所以值总是随t33的变化而变化,并无实用意义,只是一个随机值而已;
	Test5 t53 = t51;			//同上, [没有被拷贝的值num = 随机数]

	//深拷贝(指针实体拷贝)
	Test6 t61;
	t61.write_num_cp(6666);
	t61.write_text("fuck you");
	t61.write_text(test_text);
	Test6 t62(t61);
	Test6 t63 = t61;
	return;
}





//测试3:'构造函数'中调用'其他构造函数', 并初始化值
class Test7{
public:
	//构造函数之间可以互相调用, 但不能调用'非构造函数'
	Test7() : Test7(1111) {};

	//(隐式声明)构造函数+初始化赋值-'冒号竖式'
	Test7(const int n)
	:num(9999),
	 num2(n){
		fprintf(stderr,"Test7-(隐式声明)构造函数+初始化赋值-'冒号竖式': num=%d,num2=%d\n",num,num2);
	}
private:
	int num;
	int num2;
};

//测试3
void test_7(void){
	fprintf(stderr,"\n\n\n'构造函数'中调用'其他构造函数', 并初始化值.[测试3]:\n");
	Test7 t71;
	return;
}





//测试4:'赋值拷贝构造函数'(即重载运算符号, 实现拷贝构造函数)
/*
例如: 
	//重载'='号运算符, 相当与'拷贝构造函数' Test8 t81(t82)
	Test8 t81=t82;
	
	当然, 你还可以对其他运算符号, 进行特殊改造, 例如:
	Test8 t81=t82+t83;

	ps:
		如果是字符串容器的赋值拷贝, 那么需要先侦查字符串长度.
*/
class Test8{
	public:
		//(隐式声明)构造函数
		Test8(const int n){
			num=n;
			fprintf(stderr,"Test8-(隐式声明)构造函数: num=%d\n",num);
		}

		//浅拷贝构造函数
		Test8(const Test8 &m){
			num=m.num;
			fprintf(stderr,"Test8-浅拷贝构造函数: num=%d\n",num);
		}

		//'+'号运算符重载
		Test8& operator+(const Test8 &m2){
			this->num = this->num + m2.num;
			fprintf(stderr,"Test8-运算符重载operator+, num=%d\n",num);
			return *this;
		}

		//'-'号运算符重载
		Test8& operator-(const Test8 &m2){
			this->num = this->num - m2.num;
			fprintf(stderr,"Test8-运算符重载operator-, num=%d\n",num);
			return *this;
		}

		//'*'号运算符重载
		Test8& operator*(const Test8 &m2){
			this->num = this->num * m2.num;
			fprintf(stderr,"Test8-运算符重载operator*, num=%d\n",num);
			return *this;
		}

		//'/'号运算符重载
		Test8& operator/(const Test8 &m2){
			this->num = this->num / m2.num;
			fprintf(stderr,"Test8-运算符重载operator/, num=%d\n",num);
			return *this;
		}

		//'='号运算符重载
		Test8& operator=(const Test8 &m2){
			this->num = m2.num;
			fprintf(stderr,"Test8-运算符重载operator=, num=%d\n",num);
			return *this;
		}

		//'+='号运算符重载
		Test8& operator+=(const Test8 &m2){
			this->num = this->num + m2.num;
			fprintf(stderr,"Test8-运算符重载operator+=, num=%d\n",num);
			return *this;
		}

		//'-='号运算符重载
		Test8& operator-=(const Test8 &m2){
			this->num = this->num - m2.num;
			fprintf(stderr,"Test8-运算符重载operator-=, num=%d\n",num);
			return *this;
		}

		//'*='号运算符重载
		Test8& operator*=(const Test8 &m2){
			this->num = this->num * m2.num;
			fprintf(stderr,"Test8-运算符重载operator*=, num=%d\n",num);
			return *this;
		}

		//'/='号运算符重载
		Test8& operator/=(const Test8 &m2){
			this->num = this->num / m2.num;
			fprintf(stderr,"Test8-运算符重载operator/=, num=%d\n",num);
			return *this;
		}

	private:
		int num;
};

//Test8 mm = m + m2; 实际上是'两个'赋值拷贝构造函数 的调用了!!
//第一个调用, 是m + m2; 原理和'=' 号一样, m 由this 传入, m2 由(const Test8 &m2) 传入.
//第二个调用, 是mm=(m + m2) 的结果.
//还没想好, 慎用

//测试4
void test_8(void){
	fprintf(stderr,"\n\n\n'赋值拷贝构造函数'(即重载运算符号, 实现拷贝构造函数)[测试4]:\n");
	Test8 t81(100);
	Test8 t82(2);
	Test8 t83 = t81 + t82;
	Test8 t84 = t81 - t82;
	Test8 t85 = t81 * t82;
	Test8 t86 = t81 / t82;

	//比较不好的'赋值拷贝构造函数'的实现
	Test8 t87(1);//没有默认构造函数:Test8(), 不能直接使用: Test8 t87; 必须要: Test8 t87(1) 赋值
	t87 = t87 + t81;
	t87 = t87 + t82;

	//比较好的'赋值拷贝构造函数'的实现
	Test8 t88(2);
	t88 += t81;
	t88 += t82;
	return ;
}






//测试5:'析构函数'
class Test9{
	public:
		Test9(void){}
		//析构函数
		~Test9(void){
			fprintf(stderr,"Test9-析构函数: ~Test9\n");
		}
	private:
		int num;
};

//测试5
void test_9(void){
	fprintf(stderr,"\n\n\n'析构函数'[测试5]:\n");
	Test9 t9;
	return;
}





int main(void){
	//测试1
	test_123();

	//测试2
	test_56();

	//测试3
	test_7();

	//测试4
	test_8();

	//测试5
	test_9();
	return 0;
}
