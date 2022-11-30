#!/bin/sh



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



# venv 安装路径
venv_home="/home/adan/venv_py311"

install_path="/home/tarball/cpython311"
py="$install_path/bin/python3.11"
pip_home="$venv_home/lib/python3.11/site-packages"

# 属主(adan/tarball)
owner="adan"

# tarball_home tarball根目录
tarball_home="$(pwd)"



# 更新venv 中的pip(禁止更新pip, 永久禁止!! 一旦更新pip, 则全部配置pip 配置都会失效!!)
#sudo -u $owner $venv_home/bin/python3 -m pip install --upgrade pip -i https://mirrors.aliyun.com/pypi/simple/



# 创建venv 之后, 会自动还原pip, 所以要重新删除
rm $install_path/bin/pip
rm $install_path/bin/pip3
rm $install_path/bin/pip3.10
rm $install_path/bin/pip3.11

echo -e "/bin/sh -c \"$py $install_path/bin/pip3.11 \$*\"" > $install_path/bin/pip3
cp -r $tarball_home/proc_exe_sh/pip3.11 $install_path/bin/pip3.11
chmod 555 $install_path/bin/pip3

ln -s $install_path/bin/pip3 $install_path/bin/pip3.10
ln -s $install_path/bin/pip3 $install_path/bin/pip
