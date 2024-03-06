#!/bin/sh

#
# 1.wait 父进程主动阻塞, 等待所有子进程的执行返回(在'多个子进程'执行任务的时候常用)
#
echo "wait非阻塞"
# 能ping通, 返回快
ping 192.168.5.1 -c 4 &
# 不能ping通, 返回慢
ping 192.168.99.4 -c 4 &
echo "wait非阻塞-end"

# 等待所有子进程返回, 才会揭开阻塞.
wait
echo -e "如果此处不wait, 就会直接结束父进程,
子进程ping 需要一定时间才会结束, 因此子进程肯定会成为孤儿进程!!
子进程肯定会被init进程接管, 但子进程ping 仍然会继续运行, 直到结束返回."





#
# 2.wait 等待后台子进程, 测试方法2
#
echo -e "\n\n"
ping 127.0.0.1 -c 4 &

# 请注释测试, 两句话只能使用一个
echo "first sentence"

wait
echo -e "使用wait 之后, \n这句话会在\"ping 127.0.0.1 -c 4 & \"的echo回显结果, 后面, 才展示!!"

# 第二种'&'子进程表达方法:
comm="ping 127.0.0.1 -c 4"
$comm &
wait





#
# 3.函数获取返回值的方法, 与wait无关!!
#
func_test() {
	sleep 1
	echo "func_test() finished"
	return $1
}

# 函数的正确用法(前台):
# 说明: wait 对函数无效, 不能用在函数身上!!
#      shell 执行函数肯定是阻塞的, 如果弹到后台执行函数, 即新建一个子进程去执行该函数.
echo -e "\n\nwait正确用法:(想要获取函数的结果, 只能阻塞等待了, 或者 > tmpfs.)"
func_test "123"
echo "func_test() return: $?(非'&'后台运行函数, 前台阻塞执行函数, 不使用wait)"






