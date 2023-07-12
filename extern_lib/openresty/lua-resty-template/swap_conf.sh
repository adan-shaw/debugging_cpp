#!/bin/sh



user_name="www-data"

# nginx 安装路径
install_path="/home/tarball/openresty"



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



rm -r $install_path/conf
cp -r ./conf $install_path

chown $user_name:$user_name $install_path -R
# 对conf文件夹, 所有.conf配置文件, 开启只读权限
chmod 440 $install_path/conf -R
# 对my_nginx_conf文件夹, 开启550 进入读取功能
chmod 550 $install_path/conf/my_nginx_conf
# 对conf文件夹, 开启550 进入读取功能
chmod 550 $install_path/conf

