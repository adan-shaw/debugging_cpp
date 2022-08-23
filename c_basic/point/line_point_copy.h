#include <string.h>

//连续内存的存放概念:
/*
	二维数组变量的实际存放方式, 是压扁为一维数组连续存放的;
	三维数组变量的实际存放方式, 是压扁为一维数组连续存放的;
	N维数组变量的实际存放方式, 是压扁为一维数组连续存放的;
*/

//指针的基础概念:
/*
	* 所有指针都不知道自己的终止位置在哪里的, void*, char*, int* 都一样, 超过终止位就指针越界;

	* void* 指针不允许位移, 也就是不允许void* p++;

	* 由于不同指针的位移量不一样, 
		char* 位移1byte, int* 位移4 byte, 
		行指针位移一行[具体一行有多少个byte, 行byte_max=sizof(type)*line_max]
*/

//本模块利用宏, 对二维数组, 三维数组初始化, 拷贝进行优化:
/*
	* row_max 必须大于0, 必须为二维数组单行line_max;
	* row_count 必须大于0[一般是unsigned int 类型], 必须为二维数组的列数;
*/


//对二维数组进行优化[面积=row_max*row_count]
#define memsetEx2(p,c,row_max,row_count) ( memset(p,c,row_max*row_count) )

#define memcpyEx2(pdest,psrc,row_max,row_count) ( memcpy(pdest,psrc,row_max*row_count) )


//对三维数组进行优化[体积=row_max*row_count*height]
#define memsetEx3(p,c,row_max,row_count,height) ( memset(p,c,row_max*row_count*height) )

#define memcpyEx3(pdest,psrc,row_max,row_count,height) ( memcpy(pdest,psrc,row_max*row_count*height) )
