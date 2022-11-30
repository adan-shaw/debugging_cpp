#!/bin/sh



# cpython3 安装路径
install_path="/home/tarball/cpython311"
# tarball_home tarball根目录
tarball_home="$(pwd)"
# pip 库路径
pip_path="$install_path/lib/python3.11/site-packages"

# src_home file path '编译源文件path'(更新依赖库版本时,需要修改一下'编译源文件path'的名):
src_home="$tarball_home/Python-3.11.0"

#ssl_path="/usr/include/openssl"
ssl_path="/home/tarball/openssl"

# 成功安装的标记
ok_tarball_mark="$tarball_home/ok_tarball_mark"

# 属主(adan/tarball)
owner="adan"


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

# 检查openssl 是否已经tarball
if [ ! -d "$ssl_path" ];then
	echo "$ssl_path path error, please tarball openssl first"; exit 1; fi



#
# 检查'编译源文件path'是否正常
#
if [ ! -d "$src_home" ];then
	echo "src_home sourcefile path error: $src_home"; exit 1; fi


# 检查是否已安装过'this tarball package',防止重复安装(可手动删除该文件,实现手动重置)
if [ -f "$ok_tarball_mark" ];then
	echo "*****************************************************************************"
	echo "install sign:\"$ok_tarball_mark\" is already existed"
	echo "if you want to reinstall this package, run $tarball_home/untarball.cpython3.sh first"
	echo "*****************************************************************************"
	exit 0
fi



# 安装GNU Pth using system threads
apt-get install libnpth0 libnpth0-dev

# 安装依赖
apt-get install tcl-snack tcl-snack-dev 

# (尽量不要安装, 容易导致系统崩溃)
#apt-get install libsnack-alsa libsnack-oss python-tksnack python3-tksnack 
# 安装tk 图形库(有用的, py 可能要做二维码分析)
apt-get install tk8.6 tk8.6-dev tkblt tkblt-dev 
# 这两个依赖包, 绝对不能安装, 否则Linux python 崩溃, linux 系统崩溃(安装任何带有img 的软件, 都应该谨慎!!)
#libtk-img libtk-img-dev 

# 安装openssl 1.1.1, ssl头文件路径: /usr/include/openssl(cpython3 默认不安装ssl, 必须强制安装ssl模块)
apt-get install libssl-dev

# 如果可以, 最简单编译即可; python 编译十分黑暗, 绝对要跟操作系统脱离, 不能直接替换系统自带的python, 否则系统会出问题, 会崩溃
cd $src_home
./configure --prefix=$install_path \
--enable-shared \
--disable-profiling \
--enable-optimizations \
--disable-ipv6 \
--with-pymalloc \
--without-valgrind \
--without-pydebug \
--with-openssl=$ssl_path


make -j8
make install



rm $install_path/bin/pip
rm $install_path/bin/pip3
rm $install_path/bin/pip3.10
rm $install_path/bin/pip3.11
cd $tarball_home
chmod 555 $tarball_home/proc_exe_sh -R
cp -r $tarball_home/proc_exe_sh/* $install_path/bin

cp $tarball_home/pip.conf $install_path/pip.conf
chmod 444 $install_path/pip.conf


py="$install_path/bin/python3.11"
rm $install_path/bin/2to3
rm $install_path/bin/idle3
rm $install_path/bin/pydoc3
echo -e "/bin/sh -c \"$py $install_path/bin/2to3-3.11 \$*\"" > $install_path/bin/2to3
echo -e "/bin/sh -c \"$py $install_path/bin/idle3.11 \$*\"" > $install_path/bin/idle3
echo -e "/bin/sh -c \"$py $install_path/bin/pip3.11 \$*\"" > $install_path/bin/pip3
echo -e "/bin/sh -c \"$py $install_path/bin/pydoc3.11 \$*\"" > $install_path/bin/pydoc3
chmod 555 $install_path/bin/2to3
chmod 555 $install_path/bin/idle3
chmod 555 $install_path/bin/pydoc3
chmod 555 $install_path/bin/pip3
rm $install_path/bin/pip3.10

# 给python3.11 核心改名
mv $install_path/bin/python3.11 $install_path/bin/.python3.11 

# 取代原来的python3.11
mv $install_path/bin/python311 $install_path/bin/python3.11

# 修改属主
chown $owner:$owner $install_path -R

# 添加成功安装标致
pwd > $ok_tarball_mark


exit 0
