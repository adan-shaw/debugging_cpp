#!/bin/sh


#
# 1.for 循环, 只能做有限循环.
#

# 常见用法
for i in "str1" "s ss" "lslsls"
do
	echo $i
done


# * 全能测试for 循环 *
# >1 'for 循环'一般是'有限循环', 因为for i in <set>, 这个<set>一般是有限集合.
# >2 这个<set> 也允许出现重复字符串, shell一切皆是字符串, 允许重复!!
# >3 这个<set> 的字符串之间, 会自动根据' '空格来划分字符串(空格是'字符串分隔符号')
# >4 允许多个<set> 直接并排到一个for i in <set> <set1> <set2> 中
# >5 字符串迭代完毕, for 自动结束
str="i'm a asshole !!"
for i in "f***you" "f***er 1 1" $str $str
do
	echo $i
done


# 接近c 语言的for() 循环的用法
count=16
for((i=0; i<$count; i++));
do
	echo "fuck you"
done


# 循环'重定向-写入体':
write_count=4
for((i=0; i<$write_count; i++));
do
	echo "fuck"
done >&1;

# debug point
#exit 0







#
# 2.while 循环
#
# 2.1 'while 循环'可以做无限循环, 一定要自己做循环退出控制, 否则就会成为死循环)
tmp="0"
while :
do
	if [ "$tmp" -gt 7 ];then
		break
	else
		tmp=$(($tmp+1))
		echo "$tmp"
	fi
done


# 2.2 do while() < file_path 逐行读取到'line变量'
file_path="shell重定向.sh"
if [ -f "$file_path" ]; then
	while read line
	do
		#echo $line
		echo $line | grep "#"
	done  < $file_path
fi
# debug point
#exit 0







#
# 3.shell递归函数('函数递归'调用思维):
#

# 3.1 递归执行自己100 次
# 递归自身recursion() 一百次, 然后退出.
# 用变量tmp=传入值$1; 去除if [ $tmp -lt 100 ] 真的会无限循环.
recursion() {
	tmp=$(($1+1))
	if [ "$tmp" -lt 100 ];then
		echo "fuck you $tmp"
		recursion $tmp
	fi
	return $tmp
}
# 调用recursion
recursion "0"

# debug point
#exit 0





# 3.2 全局变量控制版本
# (不是一个真正的递归, 而是一个全局变量控制下的循环体):

# 全局变量count
global_count="0"

recursion2() {
	global_count=$(($global_count+1))
	if [ "$global_count" -lt 100 ]
	then
		echo "fuck you2 $global_count"
		recursion2
	fi
	return $global_count
}
# 调用recursion2
recursion2


