#!/bin/bash



# bash shell定义数组变量:
:<<!
	shell 数组, 是bash shell特有;
	posix shell 根本没有数组, 根本不能:
		${arr_num[*]}
		${#arr_str[@]}
		等等复杂操作都不能做.
	因此, posix shell 很难做复杂数组运算的!!

	bash shell数组按照下标的类型可分为两种: 
		1.索引(indexed)数组: 
			下标为0、1、2等'非负整数'. 
		2.关联(associative)数组: 
			下标为'自定义的字符串'. 
!

#
# 1.'索引'数组定义:
#
declare -a arr_str
declare -a arr_num


# '索引'数组初赋值:
arr_str=("miley" "niki" "jim" "fiona")
echo -e "arr_str 初赋值:\n ${arr_str[*]}"
#arr_str=([7]="xXx" [8]="V")
#echo -e "arr_str 初赋值:\n ${arr_str[*]}"

#arr_num=(1,2,3,4,5,6,7,8) # 错误!! 只能用' '空格间隔元素, 不能用','逗号!!
arr_num=(1 2 3 4 5 6 7 8)
echo -e "arr_num 初赋值:\n ${arr_num[*]}"
#arr_num=([9]=10 [10]=11)
#echo -e "arr_num 初赋值:\n ${arr_num[*]}"





#
# 2.'索引'数组: 元素索引
#
# '索引'数组变量,全数组打印:
echo "全值打印:${arr_str[*]}"
echo "全值打印:${arr_num[*]}"


# '索引'数组变量,遍历组打印:
echo "遍历打印:${arr_str[@]}"
echo "遍历打印:${arr_num[@]}"


# '索引'数组变量,显示下标:
echo "遍历打印下标:${!arr_str[@]}"
echo "遍历打印下标:${!arr_num[@]}"


# '@'符号和'*'符号的索引区别:
echo -e "${arr_num[*]} = \"1 2 3 4 5 6 7 8\" = 一条字符串为整体传入echo"
echo "等价于: tmp=\"1 2 3 4 5 6 7 8\";echo tmp;"

echo -e "${arr_num[@]} = \"1 2 3 4 5 6 7 8\" = 八条字符串单独传入echo"
echo "等价于: echo 1 2 3 4 5 6 7 8;"





#
# 3.'索引'数组: 元素增删改
#
# '索引'数组尾部追加元素:
arr_str[${#arr_str[@]}]="adam"
arr_str+=("michel")


# '索引'数组下标'修改/新增/查询'值:
# (3种方法, 数组从下标0开始, 自己注意数组序号, 空位会自动默认为: "" 空字符串;
#  其实'字符串数组','数字数组', 本质上都是'字符串数组-索引数组', 因为shell里面, 没有数字.
#  只有当shell 数组不能通过下标查找时[关联数组],才是特别的.
arr_str[4]="adan shaw"
arr_str[5]="tarlor swift"
arr_str[6]="elon musk"
echo ${arr_str[@]}

arr_num[8]="9"
echo "arr_num[8]=${arr_num[8]}"


# '索引'数组-切片引用(即显示[3,5] 之间的元素):
echo "arr_str[3,5] = ${arr_str[@]:3:5}"
echo "arr_num[3,5] = ${arr_num[@]:3:5}"


# '索引'数组删除元素(只能根据下标来删除):
unset arr_str[0]
unset arr_str[1]
echo ${arr_str[@]}
unset arr_num[0]
unset arr_num[1]
echo ${arr_num[@]}





#
# 4.'索引'数组: 元素统计
#
# '索引'数组, 计算数组的元素个数(只能遍历累加了),用下标遍历性能更优!! 字符串少, 代价少:
# 压缩式:
count=0;for tmp in "${!arr_str[@]}"; do let count=count+1; done
echo "arr_str[] 元素个数: $count";

# 展开式:
count=0;
for tmp in "${!arr_num[@]}";
do
	count=$((count+1));
done
echo "arr_num[] 元素个数: $count";





#
# 5.'索引'数组删除
#
# 删除整个'索引'数组:
unset arr_str
unset arr_num





#
# 6.'关联'数组定义(太复杂, 略):
#
declare -A arr_map

arr_map=([name]=zwl [age]=28 [sex]=male)

# 显示所有value(即数组中的值)
echo ${arr_map[@]}
# 显示所有key(即数组中的下标)
echo ${!arr_map[@]}







#
# 1.for 循环
#
# >1 'for 循环'一般是'有限循环', 因为for i in <set>, 这个<set>一般是有限集合.
# >2 这个<set> 也允许出现重复字符串, shell一切皆是字符串, 允许重复!!
# >3 这个<set> 的字符串之间, 会自动根据' '空格来划分字符串(空格是'字符串分隔符号')
# >4 允许多个<set> 直接并排到一个for i in <set> <set1> <set2> 中
str="i'm a asshole !!"
for i in "f***you" "f***er 1 1" $str $str
do
	echo $i
done


# bash shell 特有的<set>: 将<set> 虚拟化为: 数字-数组
if false; then
echo "
	# 数字字符串-数组(数组初始化方式: ()括号 + 空格)
	num_set=(9 99 999 9999 90909)

	# {,} 会被'全部'当成一串字符串
	max={"1","2","3","4"}

	# 用逗号','分割, 会被'全部'当成一个字符串
	maxx="1","2"

	# 数组-字符雷同测试:(允许重复字符串)
	for i in 1 2 3 1 1 1 5 $maxx $max ${num_set[*]}
	do
		echo $i
	done
	# 注意: shell 数据的详细操作方式, 还请参照: shell变量定义(数组).sh
" > /dev/null
fi
