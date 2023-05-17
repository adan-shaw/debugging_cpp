#!/bin/sh


#
# 0.case{} 中使用通配符'*'
#
# 利用case in 语句-匹配字符串. 标准posix 可用!!
# posix 也可以使用少量特殊字符, 通配符号
str="1 2 3 4 5 6 7"
str_son="1 7"
case $str in
	*"$searchString"*) echo "方法四: 包含" ;;
	*) echo "方法四: 不包含" ;;
esac





#
# 1.询问用户是否确定?
#
# posix shell 不支持read 有过多参数!! 
# 不支持正则表达式筛选输入数据, 只能做简单互交.

read -r -p "are you sure? [Y/N] " tmp
case "$tmp" in
	[yY][eE][sS]|[yY]) echo -e "Yes";                                     ;;
			[nN][oO]|[nN]) echo -e "No"; exit 1;                              ;;
									*) echo -e "\033[31m invalid input!! \033[0m"; exit 1; ;;
esac


# 符合正则表达式:(简式)
read -p "Please Enter: only one number or char here: " stdin
case "$stdin" in
   [0-9]) echo -e "你输入了数字\n"; ;;
[a-zA-Z]) echo -e "你输入了字母\n"; ;;
       *) echo -e "\033[31m invalid input!! \033[0m"; exit 1; ;;
esac



exit 0;




#
# 2.case 多路选择test
#
# 结论:
# 	标准posix不可以在case{}中使用正则表达式, 
# 	不能用'*'通配符号, 也不能用'^'之类的正则表达式符号

read -p "请输入一个字符串: " stdin
case "$stdin" in
	"1")
		# 允许'重复值', 数字1, 即字符"1".
		# 出现'重复值'时, 在上面的优先批评.
		echo "switch \"1\""
		;;
	1)
		echo "switch 1"
		;;
	222)
		echo "switch 222"
		;;
	"sss")
		echo "匹配sss 字符串"
		;;
	"*adan*")
		echo "posix 不允许在case{}中使用正则表达式,所以匹配*adan*字符串失败."
		echo "posix 也不可以使用if [ =~ ] 进行字符串匹配操作."
		echo "if [ =~ ] 也是正则表达式."
		;;
	"love you")
		echo "空格也是字符, 不会视为'字符串分割符'"
		;;
	*)
		echo "unknow input, program going on.(输入的字符串, 没有在case{}中匹配成功)"
		;;
esac



# debug point
#exit 0
