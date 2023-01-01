#!/bin/sh

# 谨记:
# 	无论shell if 判断什么!! 一定要带"" 双引号, 减少很多bug !!
# 	-z ""     单个双引号的场合
# 	"" != ""  两个双引号的场合
# 	一定要多打双引号!!







#
# if 的两种常见用法: (能保留stdout-1, 千万别用stderr-2. 保留1 作为判断依据!!)
#
# 方法1: ``, $() 直接反引, 直接对标准io输出进行if判断, 这种方法比较好!!
user_name="adan"
if [ "`cat /etc/passwd | grep "$user_name"`" != "" ];then
	echo "/etc/passwd 中包含$user_name 字符串"
fi

# if [] 时, 尽量保留stdout-1 作为判断依据, 不要用stderr-2, 
# 否则可能if 结果总是为""空, 因为管道传输, 总是依赖stdout-1
if [ -n "`id $user_name 2>/dev/null | grep "uid="`" ];then
	echo "id $user_name 存在"
fi



# 方法2: 用变量接住单寸字符串, 再进行判断
tmp=`echo "fuck you" | grep "you"`
if [ "$tmp" != "" ];then
	echo "方法一: 包含(echo使能)"
else
	echo "方法一: 不包含(echo使能)"
fi



# 方法3: 检查用户是否存在?
user_name="adan"
if id -u $user_name >/dev/null 2>&1; then
	echo "user-$user_name exists(用户已存在)"
else
	echo "user-$user_name exists(用户不存在)"
fi





#
# (基础版)文件比较:
#
:<<!
[-e "filepath"  ] 如果filepath存在, 则为真.
[-d "filename"  ] 如果filename存在且type为目录, 则为真.
[-f "filename"  ] 如果filename存在且type为常规文件, 则为真.
[-L "filename"  ] 如果filename存在且type为符号链接, 则为真.
[-r "filename"  ] 如果filename存在且可读, 则为真.
[-w "filename"  ] 如果filename存在且可写, 则为真.
[-x "filename"  ] 如果filename存在且可执行, 则为真.

["filename1" -nt "filename2"]  如果filename1比filename2新, 则为真.
                               例: [ ./file -nt /home/adan/file ]
["filename1" -ot "filename2"]  如果filename1比filename2旧, 则为真.
                               例: [ ./file - ot /home/adan/file ]
!
# 一定要加双引号!! 否则出错！！
if [ -e "/home/adan/filepath" ]; then echo "文件路径存在"; fi
if [ -d "/home/adan/folder" ]; then echo "文件夹存在"; fi
if [ -f "/home/adan/file" ]; then echo "常规文件存在"; fi
if [ -L "/home/adan/link" ]; then echo "符号链接存在"; fi
if [ -r "/home/adan/file" ]; then echo "文件存在且可读"; fi
if [ -w "/home/adan/file" ]; then echo "文件存在且可写"; fi
if [ -x "/home/adan/file" ]; then echo "文件存在且可执行"; fi

if [ "./file" -nt "/home/adan/file" ]; then
	echo "当前./file文件 比/home/adan/file目标文件新, 可执行更新操作."
fi

if [ "./file" -ot "/home/adan/file" ]; then
	echo "当前./file文件 比/home/adan/file目标文件旧, 不需要执行更新操作."
fi

# 文件'不存在':
if [ ! -e "/home/adan/filepath" ]; then
	echo "cant find this filepath文件不存在"
fi







#
# (基础版)字符串比较:
#
str1="oh fuck"
str2="oh fuck"
str3="are you kidding ?"
str4=""

# [-z "str"]: 如果str长度为零, 则为真(要加双引号, 否则出错!!)
if [ -z "$str4" ]; then
	echo "str4 is empty"
fi
# [-n "str"]: 如果str长度非零, 则为真(要加双引号, 否则出错!!)
if [ -n "$str1" ]; then
	echo "str1 is not empty"
fi

# ["$str1" = "$str2"]: 判断是否相等
if [ "$str1" = "$str2" ]; then
	echo "str1 = str2"
fi
# ["$str1" != "$str2"]: 判断是否不相等
if [ "$str1" != "$str3" ]; then
	echo "str1 != str3"
fi

:<<!
	注意1:
		'每个变量'都要加双引号!! 不能裸奔

	注意2:
		等于逻辑, 尽量不要用'==', 而使用'=';
		'=='双等号很有问题, shell里面的逻辑等于, 只有一个'='等于号.
		错误测试:
			if [ "xxx" == "xxx" ]; then
				echo "$str1 == $str2"
			else
				# 出错
				echo "str1 != str2"
			fi
!







#
# (基础版)数字比较:
#
:<<!
[num1 -eq num2]  等于,      例:[ 3 -eq $num ]
[num1 -ne num2]  不等于,    例:[ 3 -ne $num ]
[num1 -lt num2]  小于,      例:[ 3 -lt $num ]
[num1 -le num2]  小于或等于, 例:[ 3 -le $num ]
[num1 -gt num2]  大于,      例:[ 3 -gt $v ]
[num1 -ge num2]  大于或等于, 例:[ 3 -ge $num ]
!
num="3"

# 一定要加双引号, 否则出错!!
if [ "3" -eq "$num" ]; then echo "3 -eq $num"; fi
if [ "3" -ne "$num" ]; then echo "3 -ne $num"; fi
if [ "3" -lt "$num" ]; then echo "3 -lt $num"; fi
if [ "3" -le "$num" ]; then echo "3 -le $num"; fi
if [ "3" -gt "$num" ]; then echo "3 -gt $num"; fi

if [ "3" -ge "$num" ]; then
	echo "3 -ge $num"
fi

# '数字运算式'样例:
tmp=$(($num % 2))
if [ "$tmp" -eq "0" ]; then
	echo "num是偶数:$num"
else
	echo "num是奇数:$num"
fi







#
# 与或非
#
# [ ! ""] 非not
if ! "false";then
	echo "hello 非not"
fi

# ["" -a ""] 与and
if [ "true" -a "true" ];then
	echo "hello 与and"
fi

# ["" -o ""] 或or
if [ "true" -o "false" ];then
	echo "hello 或or 1"
fi
if [ "false" -o "true" ];then
	echo "hello 或or 2"
fi





exit 0;








#
# 基本宏true/false 的值判断
#

# 显示true/false 在shell 中, 实际代表的数值.(实际是一个字符串true/false)
echo "1.true/false 在shell中的实际数值测试:"
tmp=true
echo "ture=$tmp"
tmp=false
echo "ture=$tmp"

# 再次确认
echo "confirm again, 再次确认shell的布尔值就是字符串true/false"
if [ true = "true" ];then
	echo "if [ true = "true" ]; ==> if-true"
fi
if [ false = "false" ];then
	echo "if [ false = "false" ]; ==> if-true"
fi

# 再再次确认
echo "confirm again and again"
if "true";then
 echo "if "true"; ==> if-true"
fi
if ! "false";then
 echo "if ! "false"; ==> if-true"
fi

echo -e "\n\n\n"

