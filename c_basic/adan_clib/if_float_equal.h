#include<stdbool.h>

//c++ 默认附带cmath 头文件, 会与math.h 冲突, 直接屏蔽即可
#ifndef __cplusplus
	#include<math.h>
#endif


// Epslion(希腊符号中, 代表一个极小数的意思), 这里取1*10的-8次幂)
#define Epslion (1e-8)

//printf 格式化输出的正则表达式:
/*
	(printf 函数, 最高是%lf, 并不能像正则表达式一样, 使用%llf; 但可以用%lld)
	%d:   用于int
	%ld:  用于long
	%lld: 用于longlong
	%f:   float
	%lf:  double
	//%llf: long double
*/



//比较两个float/double/ldouble, 返回max
#define if_double_max(x, y)  ( x>y ? x : y )
#define if_float_max(x, y)   ( x>y ? x : y )
#define if_ldouble_max(x, y) ( x>y ? x : y )

//比较两个float/double, 返回min
#define if_double_min(x, y)  ( x>y ? y : x )
#define if_float_min(x, y)   ( x>y ? y : x )
#define if_ldouble_min(x, y) ( x>y ? y : x )



//比较两个double, 相等返回true, 不相等返回false
/*
	c++ double/float 浮点, 
	比较大小很简单, 直接使用'>'大于号, '<'小于号即可;

	比较是否相等, 很难, 需要专门设计一下:
		少于1*10的-8次幂, 即0.00000001, 小数点后8个0, 即判定为相等;
		c/c++ 判断浮点数相等, 只能取近似值;
		具体取小数点后多少位, 看你的程序敏感度, 高精度的程序, 还可以继续向后取值.

	1e-8 = 小数点后8个0
	1e-16 = 小数点后16个0
	1e-32 = 小数点后32个0
*/
#define if_double_equal(x, y)  ( fabs(x-y)<(1e-8)  ? true : false )
#define if_float_equal(x, y)   ( fabsf(x-y)<(1e-8) ? true : false )
#define if_ldouble_equal(x, y) ( fabsl(x-y)<(1e-8) ? true : false )




