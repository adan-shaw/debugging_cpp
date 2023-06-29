//编译:
//		g++ -g3 ./enum_class枚举类.cpp -o x



//C++枚举简介
/*
	C++枚举包含两种:
		限定作用域枚举类型
		不限定作用域枚举类型(一般比较少用)

	C++编译器不会将枚举类变量默认转换为整数类型,
	但我们可以使用强制类型转换, 将枚举类型变量显式地转换为整数类型;

	枚举类默认值域: [0,n] (从0 开始计数, 是一个unsigned int/short)
*/

#include <stdio.h>

//限定作用域的枚举类(其实就是方便给某些记号数据命名而已)
enum class Color{
	Red,
	Blue,
	Black
};

//强制修改起始位
enum class Color2{
	Red=100,
	Blue,
	Black
};

int main(void){
	Color color1 = Color::Blue;													//正确
	//Color color2 = 2;																	//错误, 类型不匹配(编译报错)
	Color color3 = static_cast<Color>(10);							//通过编译, 但没意义, 值范围超出了枚举类成员值;
	//unsigned int color_val1 = color1;									//错误, 枚举类变量无法直接隐式转换为整数(编译报错)
	unsigned int c2 = static_cast<unsigned int>(color1);//正确, 使用C++强制类型强转
	unsigned short c3 = static_cast<unsigned short>(color1);
	printf("c2=%d\n",c2);
	printf("Color: Red=%d,Blue=%d,Black=%d\n",Color::Red,Color::Blue,Color::Black);
	printf("Color2: Red=%d,Blue=%d,Black=%d\n",Color2::Red,Color2::Blue,Color2::Black);
	return 0;
}
