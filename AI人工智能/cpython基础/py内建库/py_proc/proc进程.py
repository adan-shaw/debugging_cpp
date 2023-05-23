#coding=utf-8



# 1.进程管理之: 创建
'''
os.fork()                               fork一个子进程, 在子进程中返回0, 在父进程中返回子进程ID;

os.system("bash command")               运行shell命令, 结果直接输出到stdout
os.popen("bash command")                运行shell命令, 结果保存到python对象中, 可使用readlines() 读出执行结果

os.execl(path, arg0, arg1, ...)         程序段替换系列(具体替换哪个程序段, pid 是否更改等细节条件, 请看c++ 编程中的细节, 这里忽略)
os.execle(path, arg0, arg1, ..., env)
os.execlp(file, arg0, arg1, ...)
os.execlpe(file, arg0, arg1, ..., env)
os.execv(path, args)
os.execve(path, args, env)
os.execvp(file, args)
os.execvpe(file, args, env)
'''



# 2.进程管理之: 退出

'''
os.abort()             [中断进程]向'调用该函数的进程'发送一个SIGABRT信号, 同时, 在Unix系统上默认的行为是产生一个core文件;

os._exit(n)            强制退出进程, 并且返回退出状态n (在退出的时候不会执行清理工作, 直接退出, 一般只用在fork之后的子进程中调用)

sys.exit()             退出程序(比较优雅,调用它能引发SystemExit异常,然后我们可以捕获这个异常做些清理工作)

可用的退出状态(并不适用所有的Unix平台都可用):
	os.EX_OK           - 正常退出
	os.EX_USAGE        - 命令执行不正确, 如命令参数错误
	os.EX_DATAERR      - 输入数据有误
	os.EX_NOINPUT      - 输入文件不存在或者不可读
	os.EX_NOUSER       - 指定的用户不存在
	os.EX_NOHOST       - 指定的主机id不存在
	os.EX_UNAVAILABLE  - 请求的服务不可用
	os.EX_SOFTWARE     - 内部软件错误
	os.EX_OSERR        - 操作系统错误
	os.EX_OSFILE       - 系统文件不存在
	os.EX_CANTCREAT    - 无法创建指定的输出文件
	os.EX_IOERR        - 在进行I/O操作时出错
	os.EX_PROTOCOL     - 协议切换操作非法, 或者协议切换不可用
	os.EX_NOPERM       - 没有权限执行该操作
	os.EX_CONFIG       - 配置错误
'''



# 3.进程管理之: 信号发送
'''
os.kill(pid, sig)         发送一个信号sig给进程id为pid的进程

os.wait()                 等待任意一个子进程结束, 返回一个tuple, 包括子进程的进程ID和退出状态信息: 
                          		一个16位的数字: 
                          			低8位是杀死该子进程的信号编号, 而高8位是退出状态(如果信号编号是0);
                          			(其中低8位的最高位如果被置位, 则表示产生了一个core文件)

os.wait3(options)         可指定参数options:
                          	默认值=0          [阻塞模式]死等一个子进程结束
                          	os.WNOHANG=1    - [非阻塞模式]如果没有子进程退出, 则不阻塞wait3()/waitpid()调用
                          	os.WCONTINUED=8 - 如果子进程从stop状态变为继续执行, 则返回进程自前一次报告以来的信息
                          	os.WUNTRACED=2  - 如果子进程被停止过而且其状态信息还没有报告过, 则报告子进程的信息
                          由于有异步操作: 本函数失败时返回-1

os.waitpid(pid, options)  可指定pid, 可指定参数options, 返回值wait(), wait3(), waitpid() 都是一样的;
                          	pid是一个正数: waitpid() 等待指定pid的进程的退出信息;
                          	pid为0:       等待'当前进程组'中的任何子进程的退出信息;
                          	pid为-1,      等待'当前进程'的任何子进程的退出信息;
                          	pid小于-1,    获取'进程组id为pid的绝对值'的任何子进程的退出信息;

分析子进程'结束状态'返回值, 简介:
	system(), wait(), waitpid() 才有返回的'结束状态'信息;
	fork(), 基本靠wait 系列函数获取'结束状态'信息;
	os.exit(), 主进程都退出了, 所有程序结束了, 无分析'结束状态'信息的必要了;(有问题可分析coredump)

分析子进程'结束状态'返回值, API:
	os.WCOREDUMP(status)      如果一个core文件被创建, 则返回True(否则返回False)
	os.WIFCONTINUED(status)   如果一个进程被停止过, 并且继续执行, 则返回True
	os.WIFSTOPPED(status)     如果子进程被停止过, 则返回True 
	os.WIFSIGNALED(status)    如果进程由于信号而退出, 则返回True
	os.WIFEXITED(status)      如果进程是以exit()方式退出的, 则返回True
	os.WEXITSTATUS(status)    如果WIFEXITED(status)返回True, 则返回一个整数, 该整数是exit()调用的参数(否则返回值是未定义的)
	os.WSTOPSIG(status)       返回导致进程停止的信号
	os.WTERMSIG(status)       返回导致进程退出的信号
'''



# 4.进程管理之: 杂项
'''
os.times()          返回一个'由5个浮点数'组成的tuple, 记录了进程的一些关键时间变量, 时间单位为秒(s);
                    时间变量包括: 
                      user time,           进程占用的用户态时间
                      system time,         进程占用的系统态时间
                      子进程的user time,   子进程占用的用户态时间
                      子进程的system time, 子进程占用的系统态时间
                      elapsed,             响应时间

os.nice(val_int)    设置进程nice值为val_int, 并返回最新的nice值
                    进程管理是没有绑定cpu 的, 请看线程管理部分, 设置nice 值已经算是进程调度了
'''
