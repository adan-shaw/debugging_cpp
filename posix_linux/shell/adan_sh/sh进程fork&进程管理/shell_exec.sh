#!/bin/sh

#
# 1.exec 的使用:
#
# 1.1 exec 简介
:<<!
	*exec
	shell 的并发, 只有多进程;
	exec 并不会阻塞调用父进程, 而是新建的'exec进程';
	'exec进程'将会是一个独立的进程, 与原来的父进程, 同等关系, 
	有自己的'shell解析器', 与原来的父进程组, 同属一个会话组.
	父进程如果退出结束, 'exec进程'会成为孤儿进程, 被init进程收养.


	exec 一般用来执行新的shell 脚本文件.
	不应该用来执行命令!!(正反引用命令都不应该用)
	不应该用来执行shell函数!!

	一般会放到'&'后台执行exec.

	直接执行exec, 则当前进程会被彻底替换.
	替换后, 原来的shell脚本, 所有的代码会被终止.
	'shell解析器'不再向下阅读, 而是'从头阅读'新的shell脚本.
!



# 1.2 exec 执行'另一个shell脚本':

# 被执行的脚本名
sh_name="./__sh_test.sh"

# 3.0 自动生成"./__sh_test.sh"测试脚本:
# 脚本exit 0: 表示脚本'正常结束'.
# 脚本exit [非0]: 表示脚本'异常结束'(是一个8 bit的值, 由你任意定制)
# 							 [1,255] or [-128,0)&&(0,+128]
echo "echo love;exit 0" > ./$sh_name
# 赋予执行权限
chmod 755 ./$sh_name



# 1.3 想要'另一个shell脚本'的返回结果:
# 截取返回结果
echo_ret=$(exec "$sh_name")
# 询问执行状态
func_ret=$?
# 不需要wait, 残废. 你用echo_ret截取exec调用结果, 会自动阻塞.
#wait
echo "$sh_name执行结果:$echo_ret"
echo -e "echo_ret=‘exec $sh_name‘ status(执行状态):\n$func_ret"

echo -e "\n\n"



# 1.4 不想要'另一个shell脚本'的返回结果
exec "$sh_name" &
func_ret=$?
echo -e "exec $sh_name status(执行状态):\n$func_ret"



# 自动删除测试脚本
rm ./$sh_name


exit 0







#
# 2.exec 错误例子, 用来执行'反引用命令'
#

# 这种写法, 虽然没错, 但是脱了裤子放屁, 
# 还不如用fork() 性能更高更方便, 基本用不到!!
#############################################################################
# echo_ret=$("exec ping 192.168.5.1 -c 4") # 错误写法!!exec不能执行一条字符串
echo_ret=$(exec ping "192.168.5.1" -c 4)
func_ret=$?

# 由于你使用‘echo_ret’变量, 截取执行结果, 这里会自动阻塞, 不需要wait
#wait

echo $echo_ret
echo -e "echo_ret='exec ping 192.168.5.1 -c 4' return:\n$func_ret"
#############################################################################





#
# 3.exec 错误例子, 用来执行'正引用命令'
#
# 错误写法,这种写法,'exec进程'会直接接管,替代'当前父进程', 成为新的父进程.
#exec ping "192.168.1.1" -c 4 
#
# 必须加 &, 这样就不会导致父进程被'接管, 替代'.
exec ping "192.168.1.1" -c 4 &
func_ret=$?
# wait 失效, 新建的'exec进程'是独立的进程, 只是同一个会话组的关系.
wait
echo -e "exec ping 192.168.1.1 -c 4 return:\n$func_ret"





#debug point
#exit 0






