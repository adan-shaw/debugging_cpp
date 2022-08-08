#!/bin/sh

##############################################################
# 扫描'指定网段'的所有活跃ip
# 注意:
# 	外网也可以扫, 但是速度慢, 最好还是不要扫外网.
# 	a b 类就别想了, 扫到天黑都扫不完, 扫完了你也看不完这么多.
##############################################################

# 本shell 的名字
SHELL_NAME=$0
default_ip="18.18.18.0"
ip_max="254"

# 帮助函数
help(){
	echo -e "\
help()
声明: 本程序用作{ 扫描'指定网段'的所有活跃ip }
C 类网主机数MAX=254;(禁止扫描A,B类网, 数量太多, 没有意义)
usage:
$SHELL_NAME [network addr] [host sum(MAX)]
$SHELL_NAME  $default_ip    $ip_max"
	return
}



#
# ***尝试获取可用的tmpfs***
# ***本程序在debian 10中测试成功,普通用户可以在tmpfs分区"/run/lock"中获取读写权限***
#
# 获取所有的tmpfs 分区
tmpfs_all=$(df -h | grep tmpfs | awk '{print $6}')

# 检查哪个分区可以写入
tmp_file_check=$(date "+%Y%m%d%H%M%S")
tmp_file_check="tmp_file_check-$tmp_file_check"
tmp_fs=""
for tmp in $tmpfs_all
do
	echo "" > $tmp/$tmp_file_check
	if [ -f "$tmp/$tmp_file_check" ];then
		tmp_fs="$tmp"
		echo "tmp_fs now = $tmp_fs"
		rm $tmp/$tmp_file_check
		break
	fi
done
if [ -z $tmp_fs ];then
	echo "没有可用的tmpfs 分区!! tmp_fs=$tmp_fs"
	exit 1
fi



# 参数数量检查
if [ $# -gt "2" ];then
	(help)
	exit 0
fi
# 参数赋值 $1 = '指定网段'
if [ -n "$1" ];then
	default_ip=$1
fi
echo "指定网段:$default_ip"
# 参数赋值 $2 = '指定网段的主机总数'
if [ -n "$2" ];then
	ip_max=$2
fi
echo "指定网段的主机总数:$ip_max"
# 检查扫描主机数量是否过大
if [ $ip_max -gt "254" ];then
	read -r -p "检查到{扫描主机数量过大},这意味着扫描时间十分长,CPU-fork消耗很大,是否继续?[Y/n]" input;
	case $input in
		[yY][eE][sS]|[yY])
			echo "Yes, the program going on now!!";
		;;

		[nN][oO]|[nN])
		echo "No, thank you, the program now quit!!"
			exit 1;
		;;

		*)
			echo "Invalid input... the program now quit!!";
			exit 1;
		;;
	esac
fi



# 原型
#a='111|222|333'
#OIFS=$IFS; IFS="|"; set -- $a; aa=$1;bb=$2;cc=$3; IFS=$OIFS
#echo $aa $bb $cc 
#
# IP 字符串分拆, 改版:
OIFS=$IFS;
IFS=".";
set -- $default_ip;
aa=$1;
bb=$2;
cc=$3;
dd=$4;
IFS=$OIFS 
echo $aa $bb $cc $dd



# 创建测试home 文件夹
test_home_dir="$tmp_fs/net_ip_alive_check_test_home"
if [ -e $test_home_dir ]
then
	rm -r $test_home_dir
fi
mkdir $test_home_dir



# fork son(fork 执行的子函数)
# shell 的fork 就是直接用变量tmp=$(执行命令), 然后还能返回结果到tmp 变量中.
#
# ping尝试的次数=4
try_ping_count="4"
# 每次ping超时时间为2秒
ping_timeout="2"
function fork_son(){
	# 组建ping 命令-原型
	#comm_tmp=$(ping -c $try_ping_count -W $ping_timeout $default_ip &)
	count_tmp="0"
	while :
	do
		count_tmp=$(($count_tmp+1))
		#if test $count_tmp -gt 255
		if test $count_tmp -gt $ip_max
		then
			break # 退出
		else
			#ping -c $try_ping_count -W $ping_timeout "$aa.$bb.$cc.$count_tmp" > "$test_home_dir/$aa.$bb.$cc.$count_tmp" & 
			# 组建ping 命令
			# 双引号, 可以引用变量!! 单引号中没有任何变量!!
			# 单引号可以写入任何字符串!! 无任何特殊字符!!
			# 双引号有特殊字符!! 特殊字符需要用 \" 转意字符表示
			# 想要组装'执行命令的字符串', 得这样组装:
			comm_tmp="ping -c $try_ping_count -W $ping_timeout $aa.$bb.$cc.$count_tmp"
			# 保存ping 命令的结果
			$comm_tmp > "$test_home_dir/$aa.$bb.$cc.$count_tmp" & 
		fi
	done

	sleep 2
	wait

	# 分析每一个ping 结果
	#ping_results=$("ls $test_home_dir") # 错误!! 执行命令, 不能带双引号, 要么:
	#ping_results=$(ls "$test_home_dir")
	ping_results=$(ls $test_home_dir)
	err_net_count="0"
	err_unknow="0"
	alive_count="0"
	cant_conn_count="0"
	for tmp in ${ping_results[@]};
	do
		#echo $tmp
		comm_tmp=$(cat "$test_home_dir/$tmp")
		case $comm_tmp in
			*"4 packets transmitted, 0 received, 100% packet loss"*)
				# 关闭不显示loss 主机
				#echo "`basename $tmp` 100% packet loss."
				let cant_conn_count=$cant_conn_count+1
				;;
			*"4 packets transmitted, 4 received, 0% packet loss"*)
				echo "host `basename $tmp` 0% packet loss, this host alive!!"
				let alive_count=$alive_count+1
				#echo $comm_tmp
				;;
			*"4 packets transmitted, 3 received, 25% packet loss"*)
			#*"4 packets transmitted, 3 received, +1 errors, 25% packet loss"*)
				echo "host `basename $tmp` 25% packet loss, this host alive!!"
				let alive_count=$alive_count+1
				#echo $comm_tmp
				;;
			*"4 packets transmitted, 2 received, 50% packet loss"*)
			#*"4 packets transmitted, 2 received, +2 errors, 50% packet loss"*)
				echo "host `basename $tmp` 50% packet loss, this host alive!!"
				let alive_count=$alive_count+1
				#echo $comm_tmp
				;;
			*"4 packets transmitted, 1 received, 75% packet loss"*)
			#*"4 packets transmitted, 1 received, +3 errors, 75% packet loss"*)
				echo "host `basename $tmp` 75% packet loss, this host alive!!"
				let alive_count=$alive_count+1
				#echo $comm_tmp
				;;
			*"4 packets transmitted, 0 received, +1 errors, 100% packet loss"*)
				let err_net_count=$err_net_count+1
				;;
			*"4 packets transmitted, 0 received, +2 errors, 100% packet loss"*)
				let err_net_count=$err_net_count+1
				;;
			*"4 packets transmitted, 0 received, +3 errors, 100% packet loss"*)
				let err_net_count=$err_net_count+1
				;;
			*"4 packets transmitted, 0 received, +4 errors, 100% packet loss"*)
				let err_net_count=$err_net_count+1
				;;
			*)
				echo "host `basename $tmp` alive, but network performance is not so good."
				let err_unknow=$err_unknow+1
				echo $comm_tmp
				;;
		esac
	done

	echo -e "\n\nping results:"
	echo "err_unknow=$err_unknow"
	echo "err_net_count('网络错误'且'100% packet loss')=$err_net_count"
	echo "alive_count=$alive_count"
	echo "cant_conn_count=$cant_conn_count"

	return 0
}


# 执行主体功能函数: fork_son()
fork_son




# 删除本shell 创建的所有资源
#rm -r $test_home_dir
echo "$test_home_dir deleted, bye !!"






exit 0;











# 阻止程序继续向下跑三连击!!
exit 0;
exit 0;
exit 0;
########################################################
# 旧版!! 已经惨遭废弃！！
########################################################





#
net_ip=""
count="0" # 去除0 广播位和255 位... 起始位为 0+1=1
timeout="25"
pint_times="1"
ping_count_max="255"
#
if [ $# = "0" ];then
	(help)
	exit
fi
if [ $# -gt "3" ];then
	(help)
	exit
fi
if [ $# = "2" ];then
	timeout=$2
fi
net_ip=$1
echo ""
echo "*ip network segment:" "$net_ip"".x"
echo "*timeout:" "$timeout" "ms"
echo ""
echo ""
#
transmitted="0"
received="0"
loss="0"
good="0"
bad="0"
while : 
do	
	count=$(($count+1))
	if test $count -gt $ping_count_max;then # if ($count 大于 255) then
		break # 退出
	else
		# 只ping 一次, 每次timeout 25 ms, 如果不在同一个内网, timeout 应该大于80ms
		transmitted_tmp="0"
		received_tmp="0"
		ip_addr_tmp="$net_ip.""$count"
		tmp=$(ping -c $pint_times -W $timeout $ip_addr_tmp)
		tmp2="1 packets transmitted"
		if [[ $tmp = *$tmp2* ]];then # 通配比对
			transmitted=$(($transmitted+1))
			transmitted_tmp="1"
		fi
		tmp2="1 received"
		if [[ $tmp = *$tmp2* ]];then
			transmitted=$(($transmitted+1))
			received_tmp="1"
		fi
		tmp2=" 0% packet loss"
		if [[ $tmp = *$tmp2* ]];then # ping 顺畅的ip addr
			good=$(($good+1))
		fi
		tmp2=" 100% packet loss"
		if [[ $tmp = *$tmp2* ]];then
			bad=$(($bad+1))
		fi
		echo "ping " $net_ip.$count "	transmitted:" $transmitted_tmp "	received:" $received_tmp
	fi
done
echo ""
echo ""
echo "search result:"
echo "****************************************"
echo "good count :" $good
echo "bad count :" $bad
echo "****************************************"



