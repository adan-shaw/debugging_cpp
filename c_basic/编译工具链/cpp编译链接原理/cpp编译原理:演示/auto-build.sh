#!/bin/sh


# 预处理: 宏字符串, 各类特殊字符串... 编译器替换字符串级别.
g++ -E cpp编译原理.cpp -o cpp编译原理.i

# 编译: 得出汇编码.
g++ -S cpp编译原理.i -o cpp编译原理.s

# 汇编to机械码s: 将汇编码, 翻译出机械码.
g++ -c cpp编译原理.s -o cpp编译原理.o
chmod 755 ./cpp编译原理.o
./cpp编译原理.o

# 链接: 二进制共享链接库, 链接, 联合, 得出完整的.exe可执行程序.
g++ cpp编译原理.o -lm -lpthread -o cpp编译原理.exe
chmod 755 ./cpp编译原理.exe
./cpp编译原理.exe




# (测试失败!!)
:<<!
	不要math.h pthread.h, 本示例仅做简单测试, 不考虑ld 链接器, 链接共享链接库的情况;

	判断在没有任何'外部链接库'的情况下, c 语言内建库生成的.o 文件, 是否可以直接执行;
		g++ -c ./cpp编译原理.cpp -o ./cpp编译原理without不要math-pthread-failed.o
		chmod 755 ./cpp编译原理without不要math-pthread-failed.o
		./cpp编译原理without不要math-pthread-failed.o
!
