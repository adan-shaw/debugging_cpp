#include<stdio.h>
#include<errno.h>



//弊端: 
/*
	只能传入一个string, 传入超出1 个参数, 会报错崩溃; 如果有必要传入多个参数, 建议改用printf() 函数;
	(或者先自行组装字符串, 再一次性把string传入这个FreeRTOS_debug_printf() 函数)
*/



//
// print_debug()/print_debugEx() 宏的定义声明:
//
//激活'通用的报错宏'
#define __DEBUG__ "on"
//off [正确的取消方法, 这才是空宏定义, 空宏定义不是: #define __DEBUG__ NULL ]
//#define __DEBUG__

//'通用的报错宏':
#ifdef __DEBUG__
	//配置'通用的报错宏'的打印API名
	#define DEBUG_PRINT_API fprintf

	//报错宏: 只传入一个string
	#define print_debug(api_name) \
		DEBUG_PRINT_API(stderr, "[%s, %s]%s->%s()->LINE-%d-, errno=[%d], %s\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name)

	//报错宏: 只传入一个string+一个Number(一般输入API名+返回值tmp)
	#define print_debugEx(api_name, api_int_ret) \
		DEBUG_PRINT_API(stderr, "[%s, %s]%s->%s()->LINE-%d-, errno=[%d], %s, %d\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name, api_int_ret)
#endif



