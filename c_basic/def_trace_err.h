#include<stdio.h>
#include<errno.h>



//
// DEBUG()/DEBUG_ex() 宏的定义声明:
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
	#define DEBUG(api_name) \
		DEBUG_PRINT_API(stderr, "\n[[ %s, %s ]]\n{{ %s, %s(), LINE-%d- }}:\n\terrno=[%d], %s\n\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name)

	//报错宏: 只传入一个string+一个Number(一般输入API名+返回值tmp)
	#define DEBUG_EX(api_name,api_int_ret) \
		DEBUG_PRINT_API(stderr, "\n[[ %s, %s ]]\n{{ %s, %s(), LINE-%d- }}:\n\terrno=[%d], %s, %d\n\n",\
		__DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__, errno, api_name, api_int_ret)
#endif





