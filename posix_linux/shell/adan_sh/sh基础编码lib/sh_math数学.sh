#!/bin/sh


#
# 1.定义'数字变量'
#
num1='12345'
num2="23435"

:<<!
shell没有'数字变量','数字变量'实际是字符串,
但你仍然可以让'数字-字符串'进行计算;
shell 中不能存在小数点, 没有浮点数!! 只能做整形运算.
除法需谨慎!! 负数可以有!!

注意:
	bash shell 允许'数字变量'中, 有'其它非数字字符', 但计算结果也是会出错的;
	num3="-a123A"
	tmp=$((num1+num3));
	# 计算结果不正常, 会出错的.
	echo "num1=$num1 + num3=$num3 = $tmp"
	posix shell 根本语法层都通过不了, 执行前语法检测时, 直接中断报错, 
	'数字变量'中, 不能有'其它非数字字符'.
	num4="a123."
!

#
# posix shell 标准写法!!(注意: 数字不允许有空值""[即:不允许有空字符串])
#
tmp=$((num1+num2))
echo "num1=$num1 + num2=$num2 = $tmp"

tmp=$((num1-num2))
echo "num1=$num1 - num2=$num2 = $tmp"

tmp=$((num1*num2))
echo "num1=$num1 * num2=$num2 = $tmp"

tmp=$((num1/num2))
echo "num1=$num1 / num2=$num2 = $tmp"

echo -e "\n\n\n"





#
# 2.复杂运算
#
sum="0"

sum=$((sum+66))
echo "$sum"
sum=$((sum-55))
echo "$sum"
sum=$((sum*100))
echo "$sum"
sum=$((sum/100))
echo "$sum"


# 11/7 = 1, 小数位被自动忽略
sum=$((sum/7))
echo "$sum"


# 3%2 = 1(求余数正确, 可用)
sum=3
sum=$((sum%2))
echo "$sum"


# (求余数正确, 可用2)
sum=8
tmp=$(($sum%2))
if [ "$tmp" -eq "0" ]; then
	echo "num是偶数:$sum"
else
	echo "num是奇数:$sum"
fi

echo -e "\n\n\n"









