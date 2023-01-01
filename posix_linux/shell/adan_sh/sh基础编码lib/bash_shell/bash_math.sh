#!/bin/bash



#
# bash shell 写法(其实posix shell 也可以用, 但是某些情况下容易出错.):
#

#
# 3.bash 定义‘数字变量’
#
# 让'数字-字符串'进行计算(方法1: 单个变量+1,+n 用这种方法)
echo "num1=$num1"
let num1=$num1+5
echo "num1+5=$num1"

echo "num1=$num1"
let num1=$num1-50
echo "num1-50=$num1"

echo "num1=$num1"
let num1=$num1*5
echo "num1*5=$num1"

echo "num1=$num1"
let num1=$num1/25
echo "num1/25=$num1"


# 让'数字-字符串'进行计算(方法2: 两个变量相加用这种方法)
tmp=$[num1+num2]
echo "num1=$num1 + num2=$num2 = $tmp"

tmp=$[num1-num2]
echo "num1=$num1 - num2=$num2 = $tmp"

tmp=$[num1*num2]
echo "num1=$num1 * num2=$num2 = $tmp"

tmp=$[num1/num2]
echo "num1=$num1 / num2=$num2 = $tmp"





#
# 4.bash 复杂运算
#
sum="0"

let sum+=66
echo $sum
#let sum-=55 # 无效, 失败的写法(统一用加法, 没有减法!! 想要减法, 自己加一个负数)
let sum+=-55
echo $sum
let sum*=100
echo $sum
let sum/=100
echo $sum


# 11/7 = 1, 小数位被自动忽略
let sum/=7
echo $sum


# 3%2 = 1(求余数正确, 可用)
sum=3
let sum=sum%2
echo $sum


# (求余数正确, 可用2)
sum=8
let tmp=sum%2
if [ $tmp -eq "0" ]; then
	echo "num是偶数:$sum"
else
	echo "num是奇数:$sum"
fi




