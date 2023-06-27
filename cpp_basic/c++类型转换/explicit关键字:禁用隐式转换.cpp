//编译:
//		g++ -g3 ./explicit关键字\:禁用隐式转换.cpp -o x



//explicit关键字的唯一作用就是: 禁用隐式转换
/*
	隐式类型转换的风险, 一般存在于自定义类型转换间, 尤其需要注意自定义类的构造函数.
	因此, 使用explicit关键字, 可以通知编译器, 禁用隐式转换!!
*/



class MyString{
public:
	explicit MyString(int n) {};				//本意: 预先分配n个字节给字符串
	explicit MyString(const char* p) {};//用C风格的字符串p作为初始化值
	//...
};

int main(void){
	//MyString s0 = "China";						//FAILED: 隐式转换被禁用
	MyString s1 = MyString("China");		//OK
	MyString s2(10);										//OK 分配10个字节的空字符串
	MyString s3 = MyString(10);					//OK 分配10个字节的空字符串

	//MyString s4 = 10;									//FAILED: 隐式转换被禁用
	//MyString s5 = 'A';								//FAILED: 隐式转换被禁用

	return 0;
}
