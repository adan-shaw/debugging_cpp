#!/bin/sh

# 如有疑问, 请参考: 
# shell成功小程序/ls批量改名和删除/批量改名/old_shell.find批量-遍历改名(别删,有参考价值).sh



# 最终选定的tmpfs分区="tmpfs_sub"[ sub=subarea=分区 ]
tmpfs_sub=""
tmpfs_file="adan"


# 获取所有的tmpfs 分区
tmpfs_all=$(df -h | grep tmpfs | awk '{print $6}')



# 根据日期, 自动生成tmpfs文件名
tmp=$(date "+%Y%m%d%H%M%S")
tmp="tmp-$tmp"

# 检查哪个分区可以写入
for i in $tmpfs_all
do
	echo "" > $i/$tmp
	if [ -f "$i/$tmp" ];then
		tmpfs_sub="$i"
		echo "choose the tmpfs subarea = $tmpfs_sub !!"
		rm $tmpfs_sub/$tmp
		break
	fi
done
if [ -z $tmpfs_sub ];then
	echo "没有可用的tmpfs 分区!! tmpfs_sub=$tmpfs_sub"
	exit "1"
fi

# 自动确认tmpfs 文件名(分区路径/自定义前缀-日期自动生成的数字串)
tmpfs_file="$tmpfs_sub/$tmpfs_file-$tmp"
echo "$tmpfs_file"



# 向tmpfs 文件写入数据(格式化写)
echo "hello world yes" > $tmpfs_file

# 向tmpfs 文件写入数据(追加写)
echo "hello world yes again" >> $tmpfs_file



# 从tmpfs 文件读出数据(整个文件读) [cat会自动读取到std_out]
cat $tmpfs_file

# 故意将std_out重定向到std_err
# (多此一举, 但是'&'符号只能在0,1,2之间用, 不能与其他命令混用!!)
cat $tmpfs_file 1>&2





# 从tmpfs 文件读出数据(逐行读取)
cat $tmpfs_file | while read line
do
	# 逐行打印
	echo "**$line**"
done





# 删除tmpfs 文件
rm $tmpfs_file


