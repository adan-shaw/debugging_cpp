#!/bin/sh

#set -x


#
# 1.直接执行, 正引用命令
#
ping 192.168.5.4 -c 4
func_ret=$?
pid_son=$$
echo -e "直接执行shell命令 pid:$pid_son"
echo -e "直接执行shell命令 return:$func_ret"
echo -e "\n\n"





#
# 2.反引用命令[ `` or $() ]
#
# 写法1:
echo_ret=$(ping 192.168.5.1 -c 4 1>/dev/null)
# 由于标准输出'>'重定向到/dev/null, 
# 因此, 这里不会有任何返回.echo_ret="", func1_ret="0":表示执行成功.
func1_ret=$?
pid_son1=$!

# 写法2:
echo2_ret=$(ping "192.168.1.1" -c 4)
func2_ret=$?
pid_son2=$!

# 写法3:
echo "love you $(ls)"
func3_ret=$?
pid_son3=$!

# 写法4:
echo "love you" $(ls)
func4_ret=$?
pid_son4=$!



# 错误写法:
# 错误写法!! 参数也不能完全字符串化!! 命令参数也不是字符串
#echo_ret=$(ping "192.168.5.1 -c 4")

# 错误写法!! 不能执行一段字符串!! 命令不是字符串
#echo_ret=$("ping 192.168.5.1 -c 4")


echo -e "\n\n\n"
echo "$echo_ret"
echo "$echo2_ret"
echo -e "shell命令1 pid:$pid_son1"
echo -e "shell命令2 pid:$pid_son2"
echo -e "shell命令3 pid:$pid_son3"
echo -e "shell命令4 pid:$pid_son4"
echo -e "shell命令1 return:$func1_ret"
echo -e "shell命令2 return:$func2_ret"
echo -e "shell命令3 return:$func3_ret"
echo -e "shell命令4 return:$func4_ret"





