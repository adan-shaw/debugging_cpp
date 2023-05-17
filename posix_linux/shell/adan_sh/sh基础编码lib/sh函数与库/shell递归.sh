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

#
# 1.shell 中的'函数递归'调用思维:
#
# 最简式:(会卡死, 陷入死循环)
#recursion() {
#  recursion
#}



#
# 2.shell 中的递归'控制式'
#

# 2.1 递归执行自己100 次tmp=$(($1+1))
recursion() {
	# 累加控制
	tmp=$(($1+1))
	# 溢出判断
	if [ $tmp -lt 100 ];then
		# 不溢出, 打印回显
		echo "love you $tmp"
		# 执行递归, 递归recursion()函数
		recursion $tmp
	fi
	return $tmp
}
recursion "0"


# 2.全局变量控制版本

# 全局变量count
gCount="0"

recursion2() {
	gCount=$(($gCount+1))
	if [ $gCount -lt 100 ]
	then
		echo "love you2 $gCount"
		recursion2
	fi
	return $gCount
}
recursion2

