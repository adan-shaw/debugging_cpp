// 前置声明:
/*
	不要千方百计想太多叼炸天的宏, 现实情况是:
		* 宏在节约内存上, 打不过const 常量
			(const 常量只有一个内存存储空间, 宏只是做字符串替换, 用一次就诞生出一个常量, 很不划算)

		* 宏在便捷性上, 打不过inline 内联函数/local 本地函数.
			(inline 内联函数也可以把API 函数的代码静态编译, 跟宏一样, 而且更客观, 阅读性强)

	所以, 宏的用法, 经常用到的也就那几种;

	有返回值, 有赋值的宏, 用'()';
	无返回值, 执行语句的宏, 用'{}';
	'() + {}', 可以联合使用, 但需要小心一点操作;
*/



//
// 常见的define宏定义(注意多点使用()括号, 不要被剥削了运算符号的优先级!!)
//

//多句宏(仅提供示范例子, 这个例子本身并没有实用性)
#define FOOLING_AROUND(x,y) { x=x+y; y=x-y; x=x-y; }


//常量宏(注意, 这里的数字并不是double, 只是字符串替换而已!! (谨记)
#define PI (3.1415926)


//(整形/浮点)大小比较宏(float浮点只是'==等于'比较难判断而已,大小比较是可行的)
#define MAX_NUM(a,b) ( (a)>(b) ? (a) : (b) )
#define MIN_NUM(a,b) ( (a)<(b) ? (a) : (b) )



//运算公式宏(示范), 一定要多用括号!!
//#define M(y) ( (y)*(y)+3*(y) )



//分配堆内存宏(成功返回指针, 失败返回NULL):
#define MALLOC(type, n) ((type *)malloc(n * sizeof(type)))
/*
	例:
		int *ptr = MALLOC(int, 5);
	展开后:
		ptr = (int *) malloc ( (5) * sizeof(int) );
*/



//跨平台能力强的变量类型重定义
typedef unsigned char byte;    /* Unsigned 8 bit value type */
typedef unsigned char boolean; /* Boolean value type */

typedef unsigned int uint32;   /* Unsigned 32 bit value */
typedef unsigned short uint16; /* Unsigned 16 bit value */
typedef unsigned char uint8;   /* Unsigned 8 bit value */
typedef signed int int32;      /* Signed 32 bit value */
typedef signed short int16;    /* Signed 16 bit value */
typedef signed char int8;      /* Signed 8 bit value */




