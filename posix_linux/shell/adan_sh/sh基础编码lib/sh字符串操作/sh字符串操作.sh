#!/bin/sh



#
# 0.查找一个字符串是否存在'目标子字符串'(一定要多打双引号!!)
#

# 方法一:利用grep查找, posix 可用！！
str="long stringx cc vv bb"
str_son="string"
if [ -n "$(echo "$str" | grep "${str_son}")" ];then
	echo "方法一:包含"
else
	echo "方法一:不包含"
fi



# 方法二:利用case in 语句-匹配字符串. 标准posix 可用!!
# 			 posix 也可以使用少量特殊字符, 通配符号
str="1 2 3 4 5 6 7"
str_son="1 7"
case $str in
	*"$searchString"*) echo "方法四:包含" ;;
	*) echo "方法四:不包含" ;;
esac




# shell 3目运算符
echo "***/data/cache****" | grep -q "/data/cache" && echo "true" || echo "false"
echo "***/data***/cache****" | grep -q "/data/cache" && echo "true" || echo "false"





exit 0

#
#***1.定义‘字符串变量’
#
# 只要有"双引号" or '单引号'创建的变量, 都是字符串变量!! 
# 但'单引号'有些特殊!! 详情看2:单引号与双引号的区别
# $() = ``(单斜点), 用来执行命令, 获取echo 回显字符串结果.
str1="love you"
str2="8086"
str3='!@#$%'

# ‘字符串变量’的引用
echo $str1
echo $str1 $str2 $str3
echo "love you $str2 $str3"
echo 'love you again $str2 $str3'

# 字符串变量合并(在'无shell系统命令'语句, 纯字符串的情况, 可以这么用)
str4=$str1$str2$str3
echo $str4





#
# 2.单引号与双引号的区别:
#
str_tmp="love you"
tmp='$str_tmp "" '' '' \\ // '
echo $tmp
# 双引号, 可以引用变量!! 单引号中没有任何变量!!
# 单引号可以写入任何字符串!! 无任何特殊字符!!
# 双引号有特殊字符!! 特殊字符需要用 \" 转意字符表示
tmp="$str_tmp"
# 想要组装'执行命令的字符串', 得这样组装!!
tmp=$("ls \"/home/adan/$str_tmp\"");





#
# 3.字符串合并与分拆
#
# 合并
A="adan"
B="shaw"
AB="$A$B"
echo $AB


# 分拆(切片)
Str="Learn Bash Commands from linuxidc.com"
# 参数扩展采用${VAR_NAME:S:L}的形式. 在此:S表示起始位置,L表示长度.
subStr=${Str:0:20}
# 结果:Learn Bash Commands
echo $subStr


Str="Learn Bash Commands from linuxidc.com"
# cut 工具分割切片字符串:{ cut -d ' ' -f 1-4 } 表示以空格分割字符串, 取前1-4个
subStr=$(echo $Str | cut -d ' ' -f 1-4)
echo $subStr


#cut 指定取出第n个字符:[cut -c 1].
Str="Learn Bash Commands from linuxidc.com"
# 先取出前20个字符
subStr=${Str:0:20}
echo $subStr
# 然后用循环体, 将这20 个字符全部切碎, 粒度为1
count="1"
sig="0"
while :
do
	# 字符串数组取值:
	arr_str[$sig]=$(echo $subStr | cut -c $count)
	let count+=1
	let sig+=1
	#echo ${arr_str[*]}
	echo ${arr_str[@]}
	if [ $count -gt "20" ];then
		break;
	fi
done



# 原型
#a='111|222|333'
#OIFS=$IFS; IFS="|"; set -- $a; aa=$1;bb=$2;cc=$3; IFS=$OIFS
#echo $aa $bb $cc 
#
# IP 字符串分拆, 改版:
default_ip="192.168.5.1"
OIFS=$IFS;
IFS=".";
set -- $default_ip;
aa=$1;
bb=$2;
cc=$3;
dd=$4;
IFS=$OIFS 
echo $aa $bb $cc $dd




#
# 4.字符串替换与删除
#
tmp="abc1234234123123123223323.23abc"
echo $tmp
# 替换一次(仅执行一次替换)
tmp=${tmp/23/bb}
echo $tmp
# 双斜杠-替换所有匹配到的字符串(全部替换)
tmp=${tmp//23/bb}
echo $tmp


# 字符串删除
# 删除一次
tmp=${tmp/abc/}
tmp=${tmp/.bbabc/}
echo $tmp
# 删除所有匹配到的字符串
tmp=${tmp//bb/}
echo $tmp
# 用变量实现删除字符
tmp2="4"
tmp=${tmp//$tmp2/}
echo $tmp





#
# 5.遍历打印当前文件夹, 统计改名的文件数量
#
:<<!
	这种遍历方法不安全, 遇到带空格的文件名, 或者某些特殊字符的文件名, 如换行等.
	会出现'字符串变量'取值偏差问题
	由于shell 字符串变量, 会对空格敏感, 
	所以遇到空格就会将一个文件名分裂成2个字符串变量, 甚至是3 个, 4个, n 个
	所以这不是一种好的遍历文件夹的方法.
	正确的方法:
		参考:shell实践成品/ls批量改名和删除/批量改名/shell.find批量-遍历改名.sh
!
#
# 遍历方法1:
dir="/home/adan"
count="0"
for tmp in $dir/*;
do
	echo $tmp
	count=$((count+1))
done
echo -e "\nthis folder's 'sum of file'=$count"


# 遍历方法2:
file_set=$( ls )
count="0"
for tmp in ${file_set[@]};
do
	echo $tmp
	count=$((count+1))
done
echo -e "\nthis folder's 'sum of file'=$count"







