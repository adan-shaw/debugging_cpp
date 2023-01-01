#!/bin/sh



#
# 1.shell内部变量:
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
# 2.shell环境变量:
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
export ADAN="fuck you"
echo "查看新设置的环境变量ADAN: $ADAN"

# 4, 查看几个常用的环境变量:
echo "PATH环境变量: $PATH"

echo "HOME环境变量(当前用户的home路径) : $HOME"

echo "HOSTNAME环境变量(主机名): $HOSTNAME"

echo "LOGNAME环境变量(当前用户的登录名): $LOGNAME"

echo "SHELL环境变量(shell程序的类型[sh or bash or ... ]): $SHELL"

echo "LANG环境变量(显示的语言字符编制[utf-8 gb2312 gbk之类的]): $LANG"








#
# 3.不怎么有效的内建变量/环境变量
#
echo "$PS1"
echo "$PS2"
echo "$PS3"
echo "$PS4"
#TMOUT: 互交超时值(计算用户输入时间):
echo "你上一次输入的数据的时间, 用了: $TMOUT"




