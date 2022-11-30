#!/bin/sh

#
# this shell working on debian/ubuntu linux.
# test platform: 
# 	debian7/10 running in intel chips
#

#set -x;

# tarball_home tarball根目录
tar_home="$(pwd)"
# 超级用户名
root_name="root"
# 成功安装的标记
ok_tarball_mark="$tar_home/ok_tarball_mark"



# 检查用户是否是超级用户$USER=="root"
echo "login user: $USER"
if [ "$USER" = "$root_name" ];then
	echo -e "(adan_shaw@qq.com): \nsuper authority ok, user=$USER."
else
	echo "login user=$USER is not root-user,"
	echo "please make sure that login user has enouhg authority to run this shell script."
	echo "root-user better!!"

	# 询问用户是否继续
	read -r -p "Are You Sure That You Wanna Go On? [Y/n] " input;
	case $input in
		[yY][eE][sS]|[yY]) echo "Yes, the program going on."; ;;
		[nN][oO]|[nN]) echo -e "bye bye\n" "bye bye\n" "bye bye\n"; exit 1; ;;
		*) echo "Invalid input!!\n" "Invalid input!!\n" "program abort.\n"; exit 1; ;;
	esac
fi






echo -e "\n\n\n"
echo "*************************************"
echo "uninstall tarball-package started !!!"
echo "*************************************"





#
# 卸载nginx
#
FILE2="$ok_tarball_mark"
FILE3="/home/tarball/cpython311"

if [ -f "$FILE2" ];then
	rm $FILE2
fi
if [ -d "$FILE3" ];then
	rm -r $FILE3
fi



# 执行码源重置(默认不打开!!)
#./restore_this_tarball_src.sh



echo "*************************************"
echo "uninstall tarball-package finished !!"
echo "*************************************"
