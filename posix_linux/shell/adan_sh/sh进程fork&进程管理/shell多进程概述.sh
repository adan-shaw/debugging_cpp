#!/bin/sh


:<<!
#
# 0.shell 编码原则
#
shell 的原子粒度:
	内核编码, 原子粒度是: syscall
	系统编码, 原子粒度是: OS API
	shell编码, 原子力度是: '.exe'可执行二进制程序.

	每一句shell 命令, 它的本质粒度, 就是直接装载二进制程序, 执行, 然后释放.
	shell 就是不停装载, 不停释放.
	所以, 不要跟shell 谈效率, 谈速度.
	只要功能实现了, 就行了, 不用考虑细节, 考虑细节, 请你写c.

	shell 的原子粒度, 就是'.exe'可执行二进制程序,
	它的命令参数, 实际就是'.exe'可执行二进制程序-的argv,argc 参数.
	这是已经约定好的!!


	shell 程序的本质:
		shell 程序的本质, 就是一个装载, 运行, 释放的程序, 简称: shell解析器.



	shell 程序的逻辑判断, 结果判断, 各种程序逻辑的实现:
		shell程序的各种程序逻辑, 基本依靠’字符串回显echo‘来实现.
		一个程序, 是否执行顺利, 全靠'字符串回显echo结果'来判断.
		这就是shell.

		shell 的函数返回, 基本只能判断, 是否装载正常, 运行正常, 退出正常.
		一旦出错, 还是得看出错std_err'字符串回显echo'
		所以, 做shell 编码, 基本看'字符串回显echo'来实现程序功能.





#
# 1.shell 脚本顺序执行的原则:
#
	shell是从上至下, 一边读, 一边解析, 一边装载,运行,释放; 一句又一句地循环下去.
	因此, 即便遇到错误语句, shell 只会报错, 然后shell 还能继续.
	除非遇到特殊字符错误,或字符编码格式错误,导致整个shell文本不可识别,才会出错终止.
	因此shell 基本不会因为错误而停止.








#
# 2.shell 的正/反命令执行模式:
#
2.1 直接执行-模式:
	把'.exe二进制程序', 直接装载,运行,释放;(可附带argv argc 运行参数)

	执行期间, '.exe二进制程序'接管标准io:0,1,2;
	'shell解析器'阻塞, 不会继续向下读取语句;
	一般不询问'执行echo回显结果'. 例如:
	chmod 755 ./x

	一般不会触发fork();
	*不应该用exec*: 直接执行exec, 则当前进程就地结束, 一切都替换为'新的exec进程'
	*不能用&*: 语法错误,不能单独使用'&'符号



2.2 反引执行-模式[ ``反引号 or $() ]
	'shell解析器'先执行``/$() 里面的命令.即:
		'shell解析器'先装载, 运行, 释放``/$() 里面的'.exe二进制程序';
		执行过程中, std_out'反引回显echo结果'默认打印到'shell局部变量'中;
				如果不用变量装载'反引回显echo结果', 
				则std_out'反引回显echo结果'会直接打印到'shell解析器'中,
				让'shell解析器'直接按照'反引回显echo结果'的字符串去执行命令,十分危险.
		一般都会用'shell局部变量', 装载'反引回显echo结果'.

		执行过程中, tty 会话'标准io:0,1,2'不会被接管;
		'shell解析器'阻塞, 不会继续向下读取语句;
		一般不会触发fork();

		*不应该用exec*: tmp=$(exec ls);当前进程不会结束,
									 '新的exec进程'结束后,当前父进程才能继续执行(脱了裤子放屁)
		*不应该用&*: 与'&后台执行模式'联用, 没意义(脱了裤子放屁)

	ps:
		``  是SysV 反引模式, 普及率很高.
		$() 是posix反引模式, 普及率也不低.



2.3 后台进程执行-模式[ & ]
	'shell解析器'执行fork(), 创建子进程, 到后台执行"xxxx &"命令.
		ps: 
			& 号前面的一整句'命令', 里面描述的'.exe二进制程序', 
			都会被单独fork()子进程装载, 放到后台执行.
			标准io:0,1,2, 由子进程和父进程共享, tty 控制台也共享;

	'shell解析器'不会阻塞, 继续向下读取语句;
	一般不询问'执行echo回显结果';
	wait命令, 唯一生效的模式.(等待单个, 或所有子进程结束返回,'shell解析器'才继续执行.)

	*不应该用exec*
	*不应该与'tmp=$()'联用, 没意义*



2.4 exec创建新进程执行-模式:
		'shell解析器'创建一个新的'shell解析器', 
		并为它再创建一个新的进程, 创造一个全新, 等同的执行环境, 去执行"exec xxxx"的命令.

		但新创建的进程, 与原来的进程, 仍然同属父子进程关系, 
		父进程终结, 子进程同样会被init进程, 当成孤儿进程收养.

		如果'当前父进程'直接exec, 则'当前父进程'被替换为'exec进程'.
		如直接执行: 
			exec ls;
		则ls 结束后, 会自动exit(0), 即便是c 语言也有exit() 函数.
		ls 终结之后, 'shell解析器'不会继续下向读取新语句, 而是直接跟着ls终止退出.
		因为exec 直接运行, 会直接取替'原来的调用进程'.

		'shell解析器'不会阻塞, 继续向下读取语句;
		一般不询问'执行echo回显结果';

		*不应该与'tmp=$()'联用, 没意义*







#
# 3.shell 脚本多进程通信:
#
shell 脚本多线程通信, 手段比较少:
	通信控制, 基本只用信号.

	数据传输, 有管道pipe, 命名管道fifo, tmpfs文件(比较可靠).
		tmpfs文件的数据通信方法:
			$ (子进程语句) > $ tmpfs;
			将结果全部打印到tmpfs内存分区上(保存到tmpfs上的文件)
			然后再分析'tmpfs上的文件'的echo/printf回显结果.


	# shell 函数库的方法
	其它操作, 基本都是靠'shell函数参数传递' or 
	std_out标准io:0,1,2 的字符打印传递.


	下面罗列shell常用到的, 跟进程相关的宏变量:
	父进程:
		子进程的pid=$!
		自身pid=$$

	子进程:
		父进程pid=$PPID
		自身pid=$$











!
