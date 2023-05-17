//编译:
//	gcc -g3 -nostartfiles ./def_trace_err.c -e test_def_trace_err -o x



//
// DEBUG()/DEBUG_ex() 宏的定义声明:
//
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>



//激活'通用的报错宏'
#define __DEBUG__ "on"
//off [正确的取消方法, 这才是空宏定义, 空宏定义不是: #define __DEBUG__ NULL ]
//#define __DEBUG__

//'通用的报错宏':
#ifdef __DEBUG__
	//配置'通用的报错宏'的打印API名
	#define DEBUG_PRINT_API fprintf

	//报错宏: 只传入一个string
	#define DEBUG(api_name) \
		DEBUG_PRINT_API(stderr, "\n[[ %s, %s ]]\n{{ %s, %s(), LINE-%d- }}:\n\terrno=[%d], %s\n\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name)

	//报错宏: 只传入一个string+一个Number(一般输入API名+返回值tmp)
	#define DEBUG_EX(api_name,api_int_ret) \
		DEBUG_PRINT_API(stderr, "\n[[ %s, %s ]]\n{{ %s, %s(), LINE-%d- }}:\n\terrno=[%d], %s, %d\n\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name, api_int_ret)
#endif



//
// DEBUG()/DEBUG_ex() 宏的测试代码:
//
void test_def_trace_err(void) {
	char str[]="Hello World";
	DEBUG("love you");
	DEBUG_EX("love you again", 111);
	exit(0);
}





//
//1.Unix 为什么需要检查API返回值
//
/*

1.1 为什么需要检查API返回值:
	一般API调用之后, 都会返回某类状态值, 用以表明调用成功与否.
	要了解调用是否成功, 就要对状态值进行检查(若调用失败, 则采取相应行动)
	至少, 程序应该显示错误消息, 警示'编码者'这里有异常.


	通常, 返回值为-1表示出错, 返回值为0表示成功.

	​少数API(比如, getpriority())在调用成功后, 也会返回-1 .
	要判断此类API是否发生错误应在调用前, 将errno先置为0,
	并在调用后对其进行检查(只对errno 检查, 这类函数很少!!).





1.2 什么情况下, 不需要检查API返回值:
	大部分API 都需要检查API返回值的, 但有部分API 的确不需要检查返回值.
	如time(NULL); strncpy();
	这类函数基本不会有任何出错的可能性, 也就不需要检查API返回值.

	不检查返回值, 少敲儿个字, 听起来的确诱人, 但实际却得不偿失.
	大部分资源申请型API, 都需要检查API返回值!!如:
		内存资源申请malloc(),new()
		文件资源申请open() fopen()
		socket资源申请socket()
		进程资源申请fork()
		第三方链接库API调用, 等等.

	实际上, 少用几个if, 性能也不会快到哪里去.
	认定API"不可能失败", 不对返回值进行检查, 这样的程序是很不严谨的.

	部分API可以用断言来判断是否可以正确调用.

*/







//
//2.errno 的本质 && 打印错误errno 的方式:
//
/*

2.1 errno 是'进程内部的全局变量'的本质:
	errno 是'进程内部的全局变量', 
	每个进程都只有一个errno.

	fork() 的子进程, 子进程也有自己的errno.
	子进程无论调用API 成功与否, 都会写时拷贝一份errno 出来.
	因此, 子进程崩溃, 也不会影响到父进程.


	如果你在程序中, 不及时打印出errno 错误信息的话,
	errno的值, 可能会被同一个进程内的'下一个API 覆盖', 
	这样你就不能知道: 
		到底是哪个API 报错的了;
		或者你看到errno 值正常, 
		但实际上, 上一个API 出错的errno值被下一个API覆盖了.
		你就看不到出错的API的errno 值了.


	此外, SUSv3 允许在函数调用成功时, 将errno设置为非零值.
	(当然, 几乎没有函数会这么做).





2.2 打印错误errno 的方式:
	要<错误打印>的方法有很多, 但都是基于errno 做出来的.
	正常来说, 错误信息都是打印到syslog 标准的log 系统中的.

*1.errno
		只是一个全局int(最底层,也会被冲掉的,新的errno覆盖旧的errno)


*2.strerr()
		stdin,stdout,stderr是每一个进程自带的,进程开始就会自动创建


*3.perror()
		是对errno 的封装翻译, 在自带字符串后面附上errno 翻译
		一般只是用作打印字符串到屏幕上面,还是打印errno到file比较好


*4.strerror()
		执行errno 翻译, 返回错误字符串





2.3 极限宏和变量(C11):[忽略!!]
errno_t (C11) 对int 的 typedef, 定义于头文件 <errno.h>
	用于自描述性函数返回errno 值(typedef)

rsize_t (C11) 对size_t 相同类型的typedef, 定义于头文件 <stddef.h>
	用于自描述性函数在运行时检查它们的参数范围(typedef)

RSIZE_MAX (C11) 范围检查函数可接受的最大大小, 定义于头文件 <stdint.h>
	展开成常量或可能在运行时改变的变量(例如, 当前分配的内存大小改变时)(宏变量)


*/







//
//3.c++常用的打印debug宏(定位到哪个文件,哪一行,哪个API,报什么errno错误):
//
/*

	如何编写一个通用的报错宏?? 其实还是很简单的:
		首先要定位到:
			哪个文件
			哪一个自定义函数
			哪一行
			API报什么errno错误

	这些都是由编译器去自动翻译的!!
***__FUNCSIG__  编译器自动插入字符串: 函数名的详细信息
                (ELF 标准信息, 打印入栈方式, 返回值类型,参数类型等)
	例如:
		printf("FUNCINFO:%s\n",__FUNCSIG__);
		输出: 
			FUNCINFO:int __cdecl hello_world(char *)

***__FUNCDNAME__  编译器自动插入字符串: 仅函数名


gcc 真正有效的宏:
__FUNCTION__   编译器自动插入字符串: 仅函数名
__LINE__  编译器自动插入字符串: 当前代码行数
__FILE__  编译器自动插入字符串: 源文件名

	因此, 你可以用这些'编译器自动插入字符串'宏, 组建一个自定义'通用的报错宏'.


*/














