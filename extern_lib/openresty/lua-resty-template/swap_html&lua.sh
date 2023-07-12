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



# 拷贝views 视图 + js 到指定目录
rm -r $install_path/html
cp -r ./html $install_path
chown $user_name:$user_name $install_path/html -R
chmod 555 $install_path/html -R
find $install_path/html -type f -exec chmod 444 {} \;



# 拷贝lua_home 到指定目录, 注意: lua_home 不能存放在$install_path/html 中, 
# 否则别人可以wget 获取你的lua_home 中的文件(openresty 默认不允许这种情况出现, 无论你怎样调试, 这样做的结果都是失败的)
rm -r $install_path/lua_home
cp -r ./lua_home $install_path
chown $user_name:$user_name $install_path/lua_home -R
chmod 555 $install_path/lua_home -R
find $install_path/lua_home -type f -exec chmod 444 {} \;

# 如果存在$install_path/nginx, 则建立一个软链接
if [ -d "$install_path/nginx" ];then
	ln -s $install_path/lua_home $install_path/nginx/lua_home; fi



# 仅仅开放给lua 文件执行权限, 否则content_by_lua_file 会失败, 不能执行;




