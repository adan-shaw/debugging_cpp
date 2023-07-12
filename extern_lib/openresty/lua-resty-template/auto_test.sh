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



cd $install_path

$install_path/bin/resty -I lua_home/adan_lua lua_home/my_test/hello_world.lua

$install_path/bin/resty -I lua_home/adan_lua lua_home/my_test/hello_world2.lua

$install_path/bin/resty -I lua_home/adan_lua lua_home/my_test/hello_world3.lua

$install_path/bin/resty -I lua_home/adan_lua lua_home/my_test/hello_world4.lua



$install_path/bin/resty -I lua_home/adan_lua lua_home/my_test/loaded_test.lua
