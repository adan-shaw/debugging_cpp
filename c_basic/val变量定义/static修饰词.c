//编译:
//		gcc -g3 ./static修饰词.c -o x



/*
前言:
	阅读本文, 务必先阅读: 
		text_code-data-bss.h
		_statement.h
	理解.code 区, .data 区, .bss 区, heap 区, pthread_stack区 等概念;
	理解全局变量, 静态变量, 局部变量, 指针等概念;
	理解声明和定义的概念;



static 关键字的本意:
	将'变量/函数'存放到.data or .bss 区中的指令;
	类中成员变量, 可以使用static 关键字;
	结构体中成员变量不能加入static 关键字;



static 关键字的作用:
	1.隐藏全局'变量/函数':
			正常情况下, 全局'变量/函数'是所有.cpp/.c 文件都可以访问的,
			加了static 之后, 全局变量就只能在当前文件中被访问;


	2.让局部变量保存上一次执行的结果:
			将局部变量存放在.data or .bss, 形成寄存关系, 
			这样每次调用函数的时候, 局部变量都不会被重置, 
			static 局部变量只在第一次调用的时候初始化.


	3.类中的static 变量or 函数:
			无论这个类的定义了多少个实体, static 成员在程序中也只有一份拷贝,
			由该类的所有对象共享访问这个static 成员,
			(它的值可以更新, 应该避免多个类实体篡改产生的资源冲突[多线程编码中, static 关键字通常是非线程安全的用法])

		静态数据成员和普通数据成员一样, 遵从public,protected,private访问规则;



static 不能用来修饰struct, class, 不存在:
	static struct {};
	static class {};

	class 类中成员变量, 可以使用static 关键字;
	struct 结构体中, 成员变量不能加入static 关键字;
	联合体, 枚举体中, 成员变量同样不能加入static 关键字;



static 关键字, 是多线程-线程安全函数杀手:
	用了static 的函数, 一般都不属于线程安全函数;
	除非这个变量是只读变量, 只读变量应当用const, 而不是static;
	所以, 多线程-线程安全函数, 会避免使用static 关键字;

*/



#include <stdio.h>
 
/*
证明:
	static 只能把描述function API 的函数指针, 移到.bss/.data 区, 实现让其他.cpp 文件不可见的功能, 
	默认这个function API 的函数指针是全局变量, 是其他.cpp 文件可见的;

	static 修饰之后, 并不是整个函数挪到 .bss/.data 区;
	只是将描述function API 的函数指针, 移到.bss/.data 区;
	(默认也是在全局变量区, 也是在.bss/.data 区, 只不过转为static 就会变成隐藏属性);

证据:
	如果整个函数移到static 区, 则局部变量tmp 也会变成static 变量, 
	此时tmp 应该会累加, 而不是每次执行前清0;

其他:
	static 关键字, 不能用在function 函数形参, 返回值上面;
	因为函数形参, 返回值, 是要入栈的, 定义为static 变量, 无意义;
*/

static int print(int a){
	int tmp = 0;
	tmp =+ a;
	printf("%d\n", tmp);
	return tmp;
}

int main(void){
	printf("%d\n", print(9));
	printf("%d\n", print(9));
	return 0;
}



