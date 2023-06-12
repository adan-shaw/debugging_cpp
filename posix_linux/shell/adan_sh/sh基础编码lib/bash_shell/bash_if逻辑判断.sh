#!/bin/bash
#
# <bash shell> if 拓展: 使用正则表达式.
#

# 注意: 正则表达式变量, 不要再用""双引号'括起来'了!! 
#       反而字符串变量, 可以再用""双引号'括起来'.

# 筛选A-Z, 检索前2 个字符, 后续字符随意
regex="[A-Z]{2}"

# 筛选[0-9]的数字, 选6个, 后续的字符随意.(需要配合: read -n 6 才能选出全部是数字)
regex2="[0-9]{6}"

# 筛选电话号码(需要配合: read -n 11 才能选出全部是数字)
regex3="[0-9]{11}"

# 筛选连用: 只允许英文字母和数字
regex4="[0-9,A-Z,a-z]"

# 复杂正则表达式: 只允许{英文字母,数字,"_"}
regex5="^\w+$"


read -p "筛选A-Z, 检索前2 个字符, 后续字符随意: " tmp
if [[ "$tmp" =~ $regex ]]; then
	echo "ok: $tmp"
else	
	echo "1 Invalid entry"
fi


read -p "筛选[0-9]的数字, 选6个, 后续的字符随意: " -n 6 -t 15 tmp
if [[ "$tmp" =~ $regex2 ]]; then
	echo "ok: $tmp"
else	
	echo "2 Invalid entry"
fi


read -p "筛选[0-9]的数字, 选11个, 后续的字符随意: " -n 11 -t 30 tmp
if [[ "$tmp" =~ $regex3 ]]; then
	echo "ok: $tmp"
else	
	echo "3 Invalid entry"
fi


read -p "筛选连用: 只允许英文字母和数字: " tmp
if [[ "$tmp" =~ $regex4 ]]; then
	echo "ok: $tmp"
else	
	echo "4 Invalid entry"
fi


read -p "复杂正则表达式: 只允许{英文字母,数字,"_"}: " tmp
if [[ "$tmp" =~ $regex5 ]]; then
	echo "ok: $tmp"
else	
	echo "5 Invalid entry"
fi





#
# <bash shell> if 机制简介:
#
if false; then
echo "
	(基础版)-[]和test: {# !/bin/sh 标准posix 中特有}
		两者是一样的, 在命令行里 test 'expr表达式' 和 [ 'expr表达式' ] 的效果相同. 
		test的三个基本作用是判断文件、判断字符串、判断整数. 
		支持使用"与,或,非"将表达式连接起来. 

		test中可用的比较运算符只有==和!=, 
		两者都是用于字符串比较的, 不可用于整数比较, 整数比较只能使用-eq, -gt这种形式. 

		无论是字符串比较还是整数比较都千万不要使用大于号小于号. 
		当然, 如果你实在想用也是可以的, 
		对于字符串比较可以使用尖括号的转义形式, 
		如果比较"ab"和"bc": [ ab \< bc ], 结果为真, 也就是返回状态为0.


	(拓展版)-[[ ]]: {# !/bin/bash 'bash shell' 中特有}
		这是内置在shell中的一个命令, 它就比刚才说的test强大的多了. 
		支持字符串的模式匹配(使用=~操作符时甚至支持shell的正则表达式).

		逻辑组合可以不使用test的-a,-o而使用 && || 等C语言常用的"与或非"符号.

		字符串比较时可以把'右边的表达式'作为一个模式(这是右边的字符串不加双引号的情况下.
		如果右边的字符串加了双引号, 则认为是一个文本字符串.), 
		而不仅仅是一个字符串, 比如[[ hello == hell? ]], 结果为真. 

		注意: 
			使用[]和[[]]的时候不要吝啬空格, 
			每一项两边都要有空格, 因为例如:
			[[ 1 == 2 ]]的结果为"假", 但[[ 1==2 ]]的结果为"真"!


	赋值符号:
	'=' 就是赋值运算.
	':='就是当冒号前面的变量不存在或值为空时, 就把等号后的值赋值给变量.
	'+='这个应该不用解释吧, 和C中一样, 变量等于本身和另一个变量的和.
" > /dev/null
fi








# (()) [] [[]] 三种括号比较:
# (()) :# 比[] 还低级, 比posix 限制还多, 不知道有什么用.
# []   :# bash [[ ]] 的降级 = posix; 与posix 都通用!!
# [[ ]]:# bash 高级正则表达式

read -p "筛选A-Z, 检索前2 个字符, 后续字符随意: " tmp
#if (( "$tmp" == "love" )); then
if [ "$tmp" = "love" ]; then
	echo "ok: $tmp"
else	
	echo "1 Invalid entry"
fi


if [[ "$tmp" = "love" && "$tmp" != "" ]]; then
	echo "ok: $tmp"
else	
	echo "1 Invalid entry"
fi


# (( )) 比[] 更低一级, 只能用 == or != 两种, 不能用-z -n 等关键字
if (( -z "$tmp" ));then
	echo "(()) cant use -z -n -gt 等对比参数, [] 特殊使用, 但(()) 可以使用== > &&"
fi
if (( $tmp == "love" && $tmp != "!love" ));then
	echo $tmp
fi






