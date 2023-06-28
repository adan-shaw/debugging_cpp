//编译:
//		g++ -g3 ./cpp运算符重载.cpp -o x



//cpp运算符重载的实际意义:
/*
	组建简明的代码逻辑, 符合普通的人类逻辑思维, 方便拓展, 维护, 迭代, 也节省代码量;
	cpp 本身的含义就是:
		oop 面向对象编程, 使用更便捷的编码规则, 实现业务逻辑;

	cpp运算符重载可以看成是一种'自定义快捷工具符号'的方式, 提高了cpp 的语言灵活性, 可靠性(简单即最可靠, 业务太复杂了);

	脚本语言的实现, python, java, lua 等, 会大量使用cpp运算符重载, 实现脚本语言的一些特殊功能, 
	实际上底层支撑实现, 可能都是cpp运算符重载;
*/



//'赋值拷贝构造函数'(即重载运算符号, 本功能实现: 拷贝构造函数重载为'='号运算符)
/*
例如: 
	重载'='号运算符, 相当与'拷贝构造函数' Test8 t81(t82)
	Test8 t81=t82;
	
	当然, 你还可以对其他运算符号, 进行特殊改造, 例如:
	Test8 t81=t82+t83;

	ps:
		如果是字符串容器的赋值拷贝, 那么需要先侦查字符串长度.
*/



#include <cstdio>



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

/*
	Test8 mm = m + m2;
	这一句, 实际上是'两个'赋值拷贝构造函数的调用:
		第一个调用, 是:
			m + m2;
			原理和'='号一样, m 由this 传入, m2 由(const Test8 &m2)传入;
		第二个调用, 是:
			mm=(m + m2);
*/



int main(void){
	//Test8 t80();								//默认构造函数已经被改写, 必须传入实参, 否则编译器报错;
	Test8 t82(2);									//error: no match for 'operator+' (operand types are 'Test8()' and 'Test8')
	Test8 t81(100);								//这种报错实际是: 编译器找不到匹配的构造函数, 就把最后一个构造函数的下一个函数, 当成报错函数, 进行报错(非精确定位);
	Test8 t83 = t81 + t82;				//所以, 这种报错十分难搞(非常无厘头)
	Test8 t84 = t81 - t82;
	Test8 t85 = t81 * t82;
	Test8 t86 = t81 / t82;

	Test8 t87(t81);
	t87 = t87 + t81;
	t87 = t87 + t82;

	Test8 t88(2);
	t88 += t81;
	t88 += t82;

	return 0;
}
