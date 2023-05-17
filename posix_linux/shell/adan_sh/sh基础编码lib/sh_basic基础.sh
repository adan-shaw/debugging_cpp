#!/bin/sh

#
# 0.shell 简介
#
:<<!
	* shell 跟system 的亲源性 *
		shell = system command 编程, shell 可以直接跑系统大部分的命令;

	* shell 如何切换root 权限 *
		当然, su root 切换用户, 然后要二次输入密码的shell 很难写.
		但你可以使用sudo -u 来指定某个用户, 去执行某个命令;(换一个思路, 照样可以做事情)
		所以, shell 基本可以做任何事情.

	* shell 的脚本特性 *
		shell 是一种一边执行, 一边翻译的语言, 不需要预先编译, 只会预先检查语法是否正确.
		但是shell 脚本一旦装载, 就不能更改其'执行内容'.
		shell 是整个文件读入内存, 再逐句执行的.
		所以, 修改shell后, 只能终止之前的shell程序, 重头再执行.
!

ls
ping "-help"
# 打印字符串的新方式(格式化打印字符串), 比echo 打印稍微复杂一点.
printf '\n%s' "hello you asshole"
printf '\n%s' "love off me asshole"





#
# 1.posix shell 与bash shell 的区别:
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
			但是debian 默认是dash, 一种bash shell 阉割版, 接近posix shell.
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





#
# 2.shell 的备注:
#
# 单行备注:
######禁止在执行语句的后面, 打单行备注!! 虽然shell这样做, 但容易出错.####
######ini 配置文件肯定出错, ini 配置文件不让在语句后面打备注的.#########


# 多行备注:
# posix 标准shell 的多行注释(万能):
if false; then
	echo "
		shell 是一种一边执行, 一边翻译的语言, 
		不需要预先编译, 只会预先检查语法是否正确.

		这意味着, 你可以像这样做'多行注释', 丝毫不影响shell 程序.
		也不会增加shell 程序的消耗.
		shell 程序只有在读到这个if语句为true的时候, 
		才会调用echo 程序, 然后将字符串弹入echo 程序.
		然后监控echo 程序的执行结果.
		但是这是一个永远读不到的语句!!

		不过这里多了一个读取if消耗.
		但其实第一种方法: 单行注释, 也有读取消耗的.
		shell 是一直逐句逐句读取的, 
		所以肯定会继续向下读取, 就算是注释语句, 也要读.
		也就是: 
			单行注释语句也要if 一下, 其实原理都一样.
	" > /dev/null
fi
# 空式(方便copy)
if false; then
	echo "
		
	" > /dev/null
fi


# shell 多行注释
# 兼容posix and bash shell, 但只能用来注释文本.
# 如果用来注释到特殊字符, 如$(), # 号等. 会有问题的.
# 所以, 在使用过程中, 如果有语法错误, 
# 可以切换到其他方法进行注释.
:<<!
注意:
	多行备注不能完全屏蔽部分关键字, 如:
		$ () # 这两个字符不能合并, 
		否则shell 可以运行, 但仍然会在这一句报错.

	posix 其实也可以用这种多行注释, 但是可能关键字限制更严重.
	实在不行, 你就用if false; then 来注释, 万能.

!





#
# 3.shell 简写式 与 ';'行结束 符号:
#
:<<!
	shell 简写式, 每一句都要打';'分号结束, 
	否则会出现很多未知错误!! 语法错误!!
	而且很难定位错误, 直接就是显示:
		最后一行缺失fi or done or esac之类的.
		十分诡异, 麻烦!!
	ps:
		shell 定位错误, 本来就比较困难, 
		尽量写得标准点, 清楚简明一点.

	';'分号, 是各种shell 的通用'行结束符号'
!
echo "hello"; echo "hello again";





#
# 4.查看当前系统支持哪些shell
#
cat /etc/shells

# 检查当前系统是否支持bash shell
#tmp=$(cat /etc/shells | grep "bash") # 匹配有歧义,/rbash也可以通过./bash最保险
tmp=$(cat /etc/shells | grep "/bash")
if [ -z "$tmp" ];then
	echo "this system is not support bash shell"
else
	echo "this system support bash shell"
fi




#
# 5.开启shell 调试模式
#
# 开启调试模式
set -x
:<<!
		shell 是先装载字符串, 再执行的; 

		shell 在'执行前'会先检查一遍语法, 
		只要shell 中有任何一个'语法错误', shell 不会运行, 
		但会告诉你什么语句错了.


		在set -x, debug 模式下, shell 会告诉你是哪一行出错, 跟lua 差不多.
		你也可以用'显式命令'debug shell: 
			sh -x ./test.sh


		常用的shell debug 模式:
			# 开启debug 模式(不打印备注)
			set -x
			# 开启debug 非常详细模式(打印备注, 从这一行开始, 备注也会被打印出来)
			set -vx
			# 关闭debug 模式(默认模式)
			set +x

		你可以在任意地方开启and关闭调试模式, 以方便进行区域调试.
		这样不用显示所有的调试信息, 免得追查困难
!
# 关闭调试模式
set +x





#
# 6.shell 脚本出现编码格式差异时:
#
:<<!
	如果出现: /bin/bash^M: bad interpreter
	一般情况下, 不是UTF-8 的字符编码格式问题, 
	而是换行符号\n or \r\n 的问题.

	unix 下的换行符, 与windows 下的换行符'\r\n' 是不一样的. 
	所以, 你在windows 上面写的shell 脚本, 迁移到linux 的时候, 
	可能需要转一下字符编码格式

	# vim 查询字符的编码格式
	:set ff?
	# 设置fileformat=unix
	:set ff=unix
	:set ff=dos
!




