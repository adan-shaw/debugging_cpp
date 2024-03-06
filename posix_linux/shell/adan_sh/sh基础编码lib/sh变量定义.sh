#!/bin/sh



#
# 0.tmp=$(), tmp=`` 将标准IO执行结果, 装载进变量tmp中:
#
:<<!
	$(), `` 这种反引命令, 会像管道一样, 标准输出1,2 都装载.
	但是不建议使用!!
	tmp=$(), tmp=``, 是很不靠谱的.
	tmp变量只能装载一句字符串!!
	而且就算装载进去, 也会删除所有空格和换行.
	所以判断命令执行结果, 最好还是用管道, 或者重定向到文件, 再做判断.
!





#
# 1.日期变量
#
# 字符串里面, 可以包含命令. 这就是shell.
echo -e "原date命令结果:\n$(date)"
echo -e "原date命令结果:\n"$(date)

year=$(date +%Y)
month=$(date +%m)
day=$(date +%d)
hour=$(date +%H)
minute=$(date +%M)
second=$(date +%S)
echo "当前日期是: $day-$month-$year"
echo "当前时间是: $hour:$minute:$second"

# 以时间数字,生成文件名后缀
# (年月日,时分秒全加上去)
file_name="前缀`date "+%Y%m%d%H%M%S"`"
# (年月日)
file_name="前缀`date "+%Y%m%d"`"
echo "$file_name"





#
# 2.定义'字符串变量'
#
# 只要有"双引号" or '单引号'创建的变量, 都是字符串变量!!
str1="love you"
str2="8086-"$(date "+%Y%m%d%H%M%S")
str3=',,!@#$%'

# '字符串变量'的引用
echo $str1 $(date)
echo $str1 $str2 $str3
echo "love you $str2 $str3"
echo 'love you again $str2 $str3'

# 字符串变量合并(在'无shell系统命令'语句, 纯字符串的情况, 可以这么用)
str4=$str1$str2$str3
echo $str4





#
# 3.定义'数字变量'
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





#
# 4.shell内部变量:
#
# 0, $0,$1,$2...(shell 传入的执行参数):
echo "shell程序本身的名字: $0"
echo "shell程序传入的第一个参数(如果有): $1"
echo "shell程序本身的第二个参数(如果有): $2"

# 显示输入的参数和参数个数(argv and argc)
# * 函数传入参数的个数
argc=$#
# * 遍历每一个输入参数
argv=$@
# * 全部输入参数
argv2=$*


# 1, '#'说明: $#变量是'命令行参数'或'位置参数'的数量
echo "本shell 程序输入的运行参数个数是: $#"

# 2, '-'说明: $-变量是传递给shell脚本的执行标志
echo "传递给本shell 程序的执行标志是: $-"

# 3, '?'说明: $? 变量是最近一次执行的命令或shell脚本的出口状态(退出状态,查看是否正确返回的)
echo "最近一次执行的命令或shell脚本的出口状态是: $?"

# 4, '$'说明: $$ 变量是shell脚本里面的进程ID.
#		妙用: Shell脚本经常使用 $$ 变量组织临时文件名,确保文件名的唯一性.
echo "本shell 程序的PID是: $$"

# 6, LINENO: 调测用, 用于显示脚本中当前执行的命令的行号. 
echo "当前脚本行号: $LINENO"

# 7, OLDPWD: cd - 功能一样.
cd $OLDPWD

# 8, PPID:当前进程的父进程的PID
echo "当前进程的父进程的PID: $PPID"

# 9, PWD: 当前工作目录. 
echo "当前工作目录: $PWD"

# 10, RANDOM: 获取一个0~32767的随机数. 
echo "获取一个0~32767的随机数: $RANDOM"
echo "获取一个0~32767的随机数: $RANDOM"
echo "获取一个0~32767的随机数: $RANDOM"

# 11, SECONDS:脚本已经运行的时间(以秒为单位)
echo "脚本已经运行的时间: $SECONDS"


# 12, REPLY: 如果read命令没有指定变量接收数据. 
#					 则可以把REPLY变量用作read命令的默认变量, 接收read命令读入的参数. 
#					 posix shell 不允许read 不带参数执行!! 但bash shell 可以!!
#read
#echo "read 读取的数据: $REPLY"


# 13, $FUNCNAME: 这个功能更强大, 它是一个数组变量, 其中包含了整个shell所有的函数的名字.
#							 单个shell的所有函数, 都会被$FUNCNAME 数组收集!!
# 因此:
# 	变量${FUNCNAME[0]} 代表'当前正在执行'的函数名,(son) 
# 	变量${FUNCNAME[1]} 则代表'调用函数'(father)
# 如果没有shell 函数, 则为"".
# [证明: 本功能是shell 的函数管理模块]
#echo "[当前正在执行]的函数名: ${FUNCNAME[0]}"
#echo "[调用本函数的上一个函数]的函数名: ${FUNCNAME[1]}"





#
# 5.shell环境变量:
#
#shell 环境变量-配置文件:
# vi ~/.bash_profile 
# vi ~/.bashrc

# 1, 查看所有环境变量
env

# 2, 查看PATH环境变量
tmp=$(env | grep PATH)
echo -e "\n含有PATH关键字的环境变量有:\n$tmp"

# 3, export 导入新的环境变量(重启会自动失效)
export ADAN="love you"
echo "查看新设置的环境变量ADAN: $ADAN"

# 4, 查看几个常用的环境变量:
echo "PATH环境变量: $PATH"

echo "HOME环境变量(当前用户的home路径) : $HOME"

echo "HOSTNAME环境变量(主机名): $HOSTNAME"

echo "LOGNAME环境变量(当前用户的登录名): $LOGNAME"

echo "SHELL环境变量(shell程序的类型[sh or bash or ... ]): $SHELL"

echo "LANG环境变量(显示的语言字符编制[utf-8 gb2312 gbk之类的]): $LANG"





#
# 6.不怎么有效的内建变量/环境变量
#
echo "$PS1"
echo "$PS2"
echo "$PS3"
echo "$PS4"
#TMOUT: 互交超时值(计算用户输入时间):
echo "你上一次输入的数据的时间, 用了: $TMOUT"









