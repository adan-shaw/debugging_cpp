#!/bin/sh

#set -x

# 查看消息的进程命令: ps -aux | grep ping

#
# 1.'&'fork()子进程到后台,执行命令(不阻塞shell解析器,继续向下读取命令)
#
# 将子进程挂到后台
echo_ret=$(ping 192.168.5.1 -c 4 &)
func_ret=$?
echo $echo_ret
echo -e "直接执行shell命令 return:$func_ret"


# 将子进程挂到后台, 不理会shell执行的'echo回显结果', 直接继续执行父进程
echo_ret=$(ping 192.168.5.1 -c 4 & > /dev/null)
func_ret=$?
echo $echo_ret
echo -e "直接执行shell命令,不理会shell执行的'echo回显结果',return:$func_ret"



# 将3 个语句统一拉到后台执行的demo:
{
echo "1"
echo "2"
echo "3"
} &

# debug point
exit 0






