#!/bin/sh

:<<!
nohup 日志输出期间, log文件的读写拥有权问题:
	一旦有echo 会回显输出, 就会抢占文件所有权, 进行写操作.

	log文件默认权限是rw:r:r

	如果日志文件被删, 被改名, 被其它人操作弄走之后, 
	nohup 指向的exe程序, 会继续工作.
	但是日志记录从此丢失.
!



#
# nohup 的常见用法
#
# 1.nohup 前台运行(默认参数)
# nohup {comm}, 标准输出1,2 自动重定向到./nohup.out文件中, 前台运行.
nohup {comm}



# 2.nohup 前台运行(自定义参数)
# nohup {comm}, 标准输出1,2 自动重定向到./myout.log, 前台运行.
nohup {comm} > myout.log 



# nohup 后台运行(自定义参数)
# nohup {comm}, 标准输出1,2 自动重定向到./myout.log, 后台运行.
nohup {comm} > myout.log & 



# 显示所有前台后台程序
jobs








#
# nohup 简介:
#
:<<!
	什么是nohup 命令??
		nohup, no hang up 的缩写, 即不挂断的意思.
		就是用户登出之后, 仍然不会挂断程序.


	nohup命令, 有什么用??
		默认情况下, 一旦用户登出, 就会自动关闭会话.
		一旦会话自动关闭, 就会关闭会话下, 所有的进程组.
		nohup 就可以保护, 你想执行的程序, 在用户登出之后, 不会被自动关闭.

		只是不会被自动关闭而已, 
		但如果你使用Ctrl + C or Ctrl + Z等主动键入命令的方式,去关闭nohup程序, 
		nohup 程序, 同样会被关闭.
		nohup 程序, 也会被程序报错而终止(如果出现bug or程序崩溃的情况,也会被终止).

		所以, 最常见的用法是:
			shell 自动化检查脚本, 运维脚本, 放在'后台运行', 
			那么登出之后, 该脚本仍然可以帮你检查系统的各项参数.
			例:
				nohup xxx.sh &


	nohup命令的前台后台:
		nohup 一般在jobs control 的后台工作.
		一般运行一些运维脚本, 甚至是木马, 后台等等.
		操作的时候, 需要使用jobs control 的'前后台切换'命令.

		如果nohup 在前台, 则与普通命令, 没啥差别.
		std_out输出同样是输出到tty,
		任何std_in同样是输入, 键入Ctrl + C 同样会停止程序.


	nohup省缺参数:
		缺省情况下, nohup 执行的命令, 会在前台运行,
		std_out,std_err都会被重定向到一个名为nohup.out的文件中.



	nohup与daemon 的较量:
	问:
		nohup 可以取替daemon shell 吗?
		比如说redis 这种数据库, 其实你不需要tarball redis 这么麻烦.
		你可以使用nohup 命令, 进行管理.
		虽然不太可靠, 但是可以省去很多复杂的daemon shell 编写??

		第一:
			nohup 如果要实现自动化启动, 需要改写/etc/profile
			不可以修改~/.bashrc, 因为这个是要登录之后, 才会被装载的rc文件.
			开机启动之后, 你不一定需要登录root用户.
			如果改写/etc/profile, 可能不能控制启动级别, 启动顺序, 优先级等等调度方案.
			会很容易出现bug, 也不是不行.
			跟在/etc/profile里面, 
			直接命令启动redis, postgresql也行啊, 只不过容易出bug而已

		第二:
			nohup start,stop,status,restart等管理命令, 很不到家.
			重新调出来, 使用jobs control 前后台切换, 
			把nohup 后台运行的进程, 重新调出来查看, 容易出现崩溃的问题.

	所以:
		nohup 绝对取替不了daemon shell

!











