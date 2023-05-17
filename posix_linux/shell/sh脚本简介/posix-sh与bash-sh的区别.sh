#!/bin/sh

#
# posix shell 与bash shell 的区别:
#

if false; then
	echo "
	1) * posix shell, bash shell简介 *
		posix shell是通用unix系统shell标准.
		bash shell是一个gnu shell, 兼容posix shell, 并且带有自己的拓展功能.
		可以把posix shell 看成是bash shell 的一个子集.

		但posix shell 的好处是:
			跨unix平台流通性.
			主要是各大平台的默认shell 都不一样, 虽然各大平台都支持安装bash shell,
			但debian默认的是dash, 一种bash shell 阉割版, 接近posix shell.
			所以, shell 的种类太多了, posix shell 好处很大.

		bash shell 的好处是:
			拓展功能多, 功能强一点.



	2) *安装, 切换posix shell or bash shell *
		posix shell, 不需要安装, 所有unix 系统都支持posix shell.
		bash shell, 安装请参考: dash阉割版切换为bash全能版
		dash shell, debian 默认的shell
		切换:
			xfce4-terminal可视化tty, 
			打开的shell基本都是dash/bash shell, 
			想使用posix shell, 直接执行: /bin/sh or sh
			再运行shell 脚本或命令.



	3) * shell编码时, 使用posix / bash shell *
		编码声明不管用, 只有在安装切换好shell 的时候, 
		编码声明的作用才比较明显.

		bash shell 头声明:
			#!/bin/bash
		dash shell 头声明:
			#!/bin/dash
		posix shell 头声明:
			#!/bin/sh

		支持在执行过程中, 突然切换头声明.
		例如:
			#!/bin/sh
			...
			# 编写posix shell 标准的代码
			...
			# posix shell end
	
			#!/bin/bash
			...
			编写bash shell 标准的代码
			...
			# bash shell end

		其他表示方式:
			#!/bin/bash --posix
			实际等于: #!/bin/sh
			本质上, 就是命令bash shell 按照posix shell 的标准, 执行下面的代码.

		使用经验:
			单靠shell 文本行头, 指定的shell 标准, 
			例如指定了posix shell 标准, 不一定就是使用posix 标准去运行shell.

			bash shell 开启posix 标准, 其实也会放水, 
			有时候posix shell 不允许的语句, 
			bash shell的posix 标准, 也会放行.(十分奇怪)

			所以, 想要真正写出posix shell 标准的程序, 最安全的办法是:
			直接执行/bin/sh, 然后运行程序进行测试, 这样才能真正体验到posix shell !!



	4) * posix shell与bash shell的主要编码差异 *
		* if[] case{} for() while() 等逻辑控制体中, 能否使用正则表达式.
			bash shell 可以使用正则表达式, posix shell 不能.

		* 是否可以使用'数字-数组'功能. posix shell 的'字符串集'不能下标索引.
			bash shell 可以使用'数字-数组'功能, posix shell 不能.

		* 兼容性: 
				posix shell 可以兼容跨平台, 但不兼容其它shell 的拓展写法.
				bash shell等拓展shell, 兼容posix shell的所有写法, 但跨平台能力一般.

		* math计算能力:
			各类shell的'数字字符串'运算区别不大, 只是写法不一样.
			建议直接以posix shell为标准即可.


	" > /dev/null
fi
