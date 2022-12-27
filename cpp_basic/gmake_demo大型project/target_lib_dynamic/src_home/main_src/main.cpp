//< 1. 在.cpp 文件里面禁用断言 >
//必须定义在#include <assert.h> 前面,
//<assert.h> 里面有判断, 如果NDEBUG 已经定义, 那就passed 不进行定义
//所以你可以抢先定义NDEBUG 禁用<assert.h> 里面的所有定义
//以达到禁用assert() 宏的目的
//
//<assert.h> 当然不会什么都不做就直接passed, 如果你抢先定义了NDEBUG..
//那么<assert.h> 就会将assert() 变成空函数:
/*
#ifdef NDEBUG
#define assert(x) ((void)0)
#else
.
#endif // #ifdef NDEBUG
*/

//由此可见, 你如果抢先定义了NDEBUG, 就会执行#define assert(x) ((void)0)
//assert() 就变成了一个空函数, 会被直接跳过, 什么都不编译.



//< 2. 在makefile 里面添加NDEBUG 禁用断言, 详情自己看makefile demo >



//< 3. 断言的用处 >
//调试的时候开启, 发行的时候禁用.

//(3.1: 设备操作种类:) 
//	所有的file 操作, socket 操作, 都应该用if 来判断,
//	因为这些都是io 设备, 有很多未知因素

//(错误用法!! 3.2: 相对可以用断言的操作:)
//	大部分内存io 操作, 大部分系统信息获取操作如: time(),
//	还有函数传入指针的时候, 你也可以断言一下指针是否失传
//	ps:
//	3.2 绝对是错误的, 真实开发中讲求效率,
//	这样婆妈, 编码质量不高, 程序跑不快

//(废话!! 3.3: 断言都不需要用到的操作:)
//	部分如rand() 已经在man 表明返回值一定在[0,RAND_MAX],
//	还有void 函数, 你不需要对这些函数断言.
///	注意: 你也不能太依赖断言, 如strncpy(), 该截断的时候还是需要截断的.


//#define NDEBUG
#include <assert.h>
#include <stdio.h>

//测试-I 链接包含选项是否有效
#include "../src1/no_assert2.h"

extern void test(void);

int main(void){
  assert(0);
  fprintf(stderr, "XXXX\n");
  test();
  return 0;
}
