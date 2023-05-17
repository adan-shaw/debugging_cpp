//编译:
//		gcc -g3 ./many_val多参数函数.c -o x 



#include <stdio.h>
#include <stdarg.h>
#include <alloca.h>



//1.多参数返回(即便是多个参数, 返回值的类型也要一致)
int ret_many_val(void){
	return 1,2,3,4,5;
}



//2.输入多个参数, 计数sum总和
//	(如果不告知参数数量和准确的类型长度, 位移肯定出错!!造成指针访问越界)
int sum(int argc, ...){
	int i, val = 0;
	va_list ap;

	va_start(ap, argc);				//argc 参数后面
	for(i = 0; i < argc; i++)
		val += va_arg(ap, int);	//根据参数类型, 位移int 位
	va_end(ap);								//释放va_list 容器

	return val;
}



//3.输入多个参数, 字符串版本(证明都是指针存放而已, 也可以提取, 但需要用二维string指针)
void ret_many_val_string(int argc, ...){
	int i;
	va_list ap;
	char** p = alloca(argc*sizeof(char*));
	char** pstr = p;						//记住二维指针p 的起始位置

	va_start(ap, argc);
	for(i = 0; i < argc; i++)
		*p++ = va_arg(ap, char*);	//二维指针赋值
	p = pstr;										//二维指针赋值完毕后, 复位

	for(i = 0; i < argc; i++)
		printf("%s\n",*pstr++);		//打印一次, 复位一次
	pstr = p;										//指针复位

	for(i = 0; i < argc; i++)
		printf("%s\n",*pstr++);
	pstr = p;


	//printf("%s%s%s%s\n",ap);//失败, 不能直接这样传递
	va_end(ap);
	return;
}



int main(void){
	//1.多参数返回测试(测试失败,只有一个int e = ret_many_val() = 5;成功,其余为乱码,失败)
	//	证明: 可以传入多个参数, 不能返回多个参数. 
	//				想返回多个值, 你可以用new() 结构体/类指针返回, 换一种思考方式.
	int a,b,c,d,e;
	int *pp = ret_many_val();
	printf("ret_many_val() = %d\n",pp++);
	printf("ret_many_val() = %d\n",pp++);
	printf("ret_many_val() = %d\n",pp++);
	printf("ret_many_val() = %d\n",pp++);
	printf("ret_many_val() = %d\n",pp++);
	a,b,c,d,e = ret_many_val();
	printf("ret_many_val() = %d-%d-%d-%d-%d\n",a,b,c,d,e);



	//2.多参数输入demo 测试
	printf("get sum of 4, 2, 3, 4, 5 = %ld\n", sum(4,2,3,4,5));
	printf("get sum of 3, 5, 10, 15 = %ld\n", sum(3,5,10,15));

	//3.输入多个参数, 字符串版本
	ret_many_val_string(4,"love","you","idiot idiot idiot", "*********************");

	return 0;
}





/*
	1.什么是'多参数函数'?
		多参数函数, 可输入'多个参数'(参数不限)的函数, 常见的有:
			int printf(const char* format, ...);
		使用example:
			printf("%d%d%d%d\n",a,b,c,d);





	2.'多参数函数'的实现方式: (多参数'函数宏')
	2.1: 头文件:
		<stdarg.h>


	2.2: 函数宏(宏没有错误返回!!一般来说, 都是绝对正确的!):
		va_start	令函数得以访问可多参数(函数宏)
		va_arg		访问下一个函数可多参数(函数宏)
		va_copy		(C99)创造函数可多参数的副本(函数宏)
		va_end		结束函数可多参数的行程(函数宏)


	2.3:类型va_list 容器(typedef)
		va_list保存了va_start, va_arg, va_end及va_copy的所需信息,
		可直接在'多参数函数’中访问va_list 容器





	3.固定参数函数与多参数函数对比

	'固定参数函数':
		一般只返回一个值, 函数的参数个数是'固定的'

	'多参数函数’弊端很多:
		首先, 多参数传入, 可能还有不少用途.
		但是, 多参数返回, 使用者会很混乱.
		你应该如何区分这些返回值? 出错的时候, 怎么办?

		最大的麻烦是:
			你并不知道值的类型, 如果值的类型是一致的, 那还好办.
			如果值的类型不是一致的, 问题比较严峻, 很麻烦.
			一般情况下, 不值得设计得那么复杂.
			(测试失败, 只有一个e 成功, 其余为乱码, 失败)

			况且, c/c++ 一般都推荐, 使用bool 做返回值,
			要不最好就是不返回值!!
			真的有必要返回值, 也是通过引用的方式, 传入引用值返回.
			这样可以比较高'可控性地'掌握函数的值返回.





	4.C 语言的一个隐晦规则:
		纯c 语言function() 如果没有定义返回类型, 默认返回int
		(但是这样的函数, 编译器会有警告报出, 不推荐这样做)





	5.由于多值返回,(测试失败, 只有一个e 成功, 其余为乱码, 失败)
		所以, 其实多值返回, 还是需要用:
		* 返回一个数组, 链表, 等链式结构存储(下策)
		* 传入一个引用, 用引用传入, 用引用返回(上策)

		所以, 多值返回, 建议用引用.

*/
