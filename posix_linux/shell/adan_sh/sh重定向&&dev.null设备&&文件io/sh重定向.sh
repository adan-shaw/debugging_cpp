#!/bin/sh


#
# 0.重定向期间, 如果重定向到file 中时, file 的读写权限抢占问题
#
:<<!
	shell 重定向到file 时, 会触发文件锁, 对文件产生片刻独占.
	但是并不是永久独占, 因为被重定向的file, 你仍然可以利用gedit 打开这个文件.
	说明无论是 > 还是 >>, 都没有独占文件.

	所以, 一旦shell 有echo 标准输出1, or 标准输出2的时候, 
	就会尝试抢占file 的读写权限.
	如果抢占失败, 可能会跳过, 不理会失败.
	可能会导致shell 执行崩溃.

	shell 重定向时, 自动生成的文件, 是rw:r:r 权限的.
	只有属主可写, 安全.
	属主组成员, 也不可写!!


	即便如此, shell 不适合做运算, 也不适合做自动运维任务.
!





#
# 1.shell 的标准io流
#
:<<!
	shell 的文件句柄, 就是一个数字.

	标准输入(standard input)  0  /proc/self/fd/0
	标准输出(standard output) 1  /proc/self/fd/1
	错误输出(error output)    2  /proc/self/fd/2

!





#
# 2.shell 的标准io流-重定向
#
:<<!
	重定向到'标准io流', 需要加'&'号, 如:
		2>&1


	重定向到'非标准io流' or 文件, 不需要加'&'号, 如:
		1>tmp.file
		2>tmp.file
		1>/dev/null


	重定向符号, 有'>'格式化写 or '>>'追加写
!
# 标准err > 标准输出, 标准输出 > tmp.file
file_path="/etc/passwd"
if [ -f $file_path ]; then
	cat $file_path 2>&1 1>tmp.file
fi





#
# 3.文件逆向重定向(读入)到shell 变量
#

# 3.1 do while() < file_path 逐行读取到'line变量'
file_path="shell重定向.sh"
if [ -f $file_path ]; then
	while read line
	do
		#echo $line
		echo $line | grep "#"
	done  < $file_path
fi


# debug
#clear
# 3.2 整个文件到'data变量'(空格, 换行会被自动全部消除)
file_path="shell重定向.sh"
if [ -f $file_path ]; then
	data=`cat $file_path`
	echo $data
fi
# ps:
# 	不能用来消除注释, 发布release软件, 因为同行注释, 难以消除. 
# 	而且你也没有时刻用';'分隔语句间隔, 所以这种方法基本没啥用.


# ps2:
# 	使用cat 命令, 把文件数据导入到标准io: 0,1,2, 是没有问题的.
# 	如: cat /etc/passwd 2>&1 1>tmp.file
# 			它的空格, 换行, 会被全部保留, 没有全部自动消除

# 	但是, 如果你将文件数据‘装载/导入’进shell 变量中, 这就会有问题!!
# 	shell 变量只能装置一个字符串, 不能过多换行.
# 	所以, 空格, 换行会被自动全部消除!!
# 	所以, 不能将'文件数据' or 命令执行结果, ‘装载/导入’进shell 变量中!!


# ps3:
# 	管道其实也可以保留'空格, 换行', 只是管道一般默认最大只能读取64kb 的文件而已.
# 	管道也可以处理: .exe执行结果的标准io输出




