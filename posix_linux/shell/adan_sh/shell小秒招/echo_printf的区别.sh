#!/bin/bash
#set -x # 开启debug 模式(不打印备注)


# 本例说明: 
# 在shell 程序中, echo与printf的区别(printf 天生就具备正则表达式).



# 1.显示字符
echo "显示单字符串"
echo "显示" "多" "串" "字符串"

val="<变量字符串>"
echo $val "显示变量字符串"
echo "显示变量字符串2 $val"

echo -e "开启转义字符模式 \nlove you idiot \n\nbye"
printf "love you idiot\nshell 'printf' 自带类似c语言的换行模式,也能打印多串字符"



# 2.重写文件or 文件尾追加数据 (输出重定向, 还有个输入重定向<< <)
echo "先格式化文件, 再写入这串字符串" > test.txt
echo "先格式化文件, 再写入这串字符串" > test.txt # 第二次写入, 还是只有一句

echo "追加写入字符串" >> test.txt
echo "追加写入字符串" >> test.txt
echo "追加写入字符串" >> test.txt



# 3.显示字符串长度
test_string="0123456789" # (字符串从0 下标开始)
len=${#test_string}
echo "$len"

# 截取 3-8 的字符串返回 (从下标为3 的字符开始, 截取6个字符)
cut_string=${test_string:3:6}
echo "$cut_string"



