# , , , , ,,, , , , ,,, ,,, , , ,  , , , ,, ,  , , ,, , , , , , ,, , ,, ,, ,
# text rules simple version for public:
# !_1.'行-字符长度'测试(how many char in one line):
# 1111111111111111111111111111111111111111111111111111111111111111111111111
# 邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵
# *************************************************************************
# !_2.标题命名规则(headline named rules):
# 		!_1.xxx:	'level 1' headline
# 		!_2.xxx:	'level 1' headline
# 		!_3.xxx:	'level 1' headline
# 			@_2.xxx:	'level 2' headline
# 				#_3.xxx:	'level 3' headline
# !_3.统一使用english US(英文)的标点符号, 作为编码统一标点字符.
# !_4.文件更改日志(file changed log):
# 		...
# , , , , ,,, , , , ,,, ,,, , , ,  , , , ,, ,  , , ,, , , , , , ,, , ,, ,, ,

#!/bin/sh

:<<!
	include 包含一个shell function library 函数库,
	并不会创建新的shell子进程,
	而是让'shell编译器'直接装载多一段代码.
!

# source 关键字(bash shell 特有!!)
#source ./sh_func_lib.sh

# posix shell 方式
. ./sh_func_lib.sh





#
# 测试1: 普通shell 函数, 通过8bit 数字字符返回.
#
# 输入数字实参"123", 执行函数
echo_ret=$(shell_func1 "123")

# 获取最近一次函数执行结果
# (shell是单线程的, 不用考虑多线程问题, 这个肯定正确的)
func_ret=$?

echo -e "shell_func1() 函数返回:\n$func_ret\n"
echo -e "shell_func1() 'echo回显'返回:\n$echo_ret\n"

# 测试失败!! 不能传入非数字的字符串!! 
#######################################################################
# 测试: 向'shell_func1'传入"damn"字符串.
# 证明: shell 普通函数, 返回值8 bit, 不能是数字以外的字符串, 超出[-128,255]的数字也不行.
#shell_func1 "damn" # 失败!!
#shell_func1 "123456" # 失败!!
#func_ret=$?
#echo -e "shell_func1() 函数返回:\n$func_ret\n"
#echo -e "shell_func1() 'echo回显'返回:\n$echo_ret\n"
#######################################################################





#
# 测试2:通过'echo回显'返回字符串
#
echo_ret=$(echo2STDOUT "fuck you" &)
func_ret=$?
echo -e "echo2STDOUT() 'echo回显'返回:\n$func_ret\n"
echo -e "echo2STDOUT() 'echo回显'返回:\n$echo_ret\n"





exit 0



