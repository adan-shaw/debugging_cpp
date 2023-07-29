//#inlcude <stdbool.h>
//
// '常用的嵌入式-位操作宏'
//
#define GET_LOW_BYTE0(x) (((x) >>  0) & 0x000000ff)     /* 获取第0个字节 */
#define GET_LOW_BYTE1(x) (((x) >>  8) & 0x000000ff)     /* 获取第1个字节 */
#define GET_LOW_BYTE2(x) (((x) >> 16) & 0x000000ff)     /* 获取第2个字节 */
#define GET_LOW_BYTE3(x) (((x) >> 24) & 0x000000ff)     /* 获取第3个字节 */

#define CLEAR_LOW_BYTE0(x) ((x) &= 0xffffff00)          /* 清零第0个字节 */
#define CLEAR_LOW_BYTE1(x) ((x) &= 0xffff00ff)          /* 清零第1个字节 */
#define CLEAR_LOW_BYTE2(x) ((x) &= 0xff00ffff)          /* 清零第2个字节 */
#define CLEAR_LOW_BYTE3(x) ((x) &= 0x00ffffff)          /* 清零第3个字节 */

#define SET_LOW_BYTE0(x) ((x) |= 0x000000ff)            /* 第0个字节全部bit置1 */
#define SET_LOW_BYTE1(x) ((x) |= 0x0000ff00)            /* 第1个字节全部bit置1 */
#define SET_LOW_BYTE2(x) ((x) |= 0x00ff0000)            /* 第2个字节全部bit置1 */
#define SET_LOW_BYTE3(x) ((x) |= 0xff000000)            /* 第3个字节全部bit置1 */



#define GET_BIT(x, bit) (((x) & (1 << (bit))) >> (bit)) /* 获取第bit位 */
#define CLEAR_BIT(x, bit) ((x) &= ~(1 << (bit)))        /* 清零第bit位 */
#define SET_BIT(x, bit) ((x) |= (1 << (bit)))           /* 置位第bit位 */



/* 判断x 的第n位bit是否为1, 是就返回true(一般不这样用, 这个宏会触发2次if, 只做示范, 不推荐使用) */
//#define IF_BIT(x, bit) (((x) & (1 << (bit))) ? true : false)
#define IF_BIT(x, bit) (((x) & (1 << (bit))) ? 1 : 0)
/*
一般用法(直接if):
	if(x & (1 << bit)){
		do something
	}
	else{
		do something
	}
*/



/* 获取x 的'第[n:m]位bit的值': (从右边开始,第n位开始数起(从1开始数起),连续取m位数,返回'第[n:m]位的值') */
#define GET_BIT_M_TO_N(x, m, n) ((unsigned int)((x) << (31-(n))) >> ((31-(n)) + (m)))
