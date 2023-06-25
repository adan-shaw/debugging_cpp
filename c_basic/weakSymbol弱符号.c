//编译:
//		gcc -g3 ./weakSymbol弱符号.c -o x



//弱符号的用处: c 语言解决同名函数的冲突问题(c++ 基本直接用虚函数, 所以c++ 中基本很少用)
/*
	符号类型		对象
	强					函数名, 赋初值的全局变量
	弱					未初始化的全局变量



	当代码中同时存在多个强或弱的全局变量时, 要遵守如下规则:
		* 强符号只能定义一次, 否则编译错误;
		* 强弱符号同时存在, 以强符号为准;
		* 没有强符号, 则从多个弱符号中任选一个, 用--fno-common编译选项可以在这种情况下打出warning;



	弱符号的声明有两种方式:
		__attribute__((weak))  - 前缀修饰法(内嵌, 不能单独存在)
		#pragma weak           - 编译宏强调(单独一行, 告诉编译器执行预处理)



	ps:
		static 变量不能变为weakSymbol弱符号;
		不影响const/extern 等前缀词的使用;
*/



//强符号函数&变量(默认是强符合, 不能同名)
void func(void);
int var;
//const char *p="hello";

//用修饰法, 创建弱符号函数
void __attribute__((weak)) func(void);
int __attribute__((weak)) var;
//const char __attribute__((weak)) *p="world";



//强符号函数&变量(默认是强符合, 不能同名)
void func2(void);
int var2;
const char *p2="hello";

//用编译宏强调法, 创建弱符号函数
#pragma weak func2
#pragma weak var2
#pragma weak p2



int main(void){
	return 0;
}
