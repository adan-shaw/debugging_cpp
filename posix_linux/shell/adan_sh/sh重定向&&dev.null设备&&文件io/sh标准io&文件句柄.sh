#!/bin/sh


#
# 1.shell 的标准io流
#
:<<!
	shell 的文件句柄, 就是一个数字.

	标准输入(standard input)  0  /proc/self/fd/0
	标准输出(standard output) 1  /proc/self/fd/1
	错误输出(error output)    2  /proc/self/fd/2


	shell 并没有文件句柄一说, 只有读取文件, 写入文件的操作.
	shell 是以.exe 为单位的数据io 读写, 不可能会有文件句柄的.
	所以:
		[ 3 - 65536 ] 都不是'shell进程的文件句柄'
		不要对其重定向操作.
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



