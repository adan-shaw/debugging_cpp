# cygwin 与linux 无异, 只是多了一个 /cygdrive/d 盘符不一样而已, 命令还是linux 命令;
# 所以cygwin 实际上是很值得玩的, 但cygwin 的软件性能很差, 软件太旧;
# windows 不太愿意兼容cygwin, cygwin 的大部分软件都是2005-2008 的, 这也是windows 系统为什么抛弃xp 的最主要原因

# 注意:
# 	在windows 下, 安装了cygwin, 就不要安装wsl2 linux 子内核模块了, 否则会搅混乱的;
# 	最好还是用cygwin, 比较容易可控, 可惜的是cygwin 的软件比较旧

# cygwin 进入之后, 有没密码, 可以手动修改: 
# 	/etc/passwd 文件, 手动新增用户, 或者直接赋予当前windows 用户同名的cygwin 用户0:0 超级管理员权限



# windows 如何避开盘符
# D: = /cygdrive/d
# C: = /cygdrive/c



# cygwin 下使用export, echo, ls, mkdir 等linux 命令
export QT_DIR=$1
export INSTALLER_NAME=$2
export BUILD_TYPE=RelWithDebInfo
#call "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Auxiliary/Build/vcvars64.bat"

cur_path=`pwd`

echo "1=$1"
echo "2=$2"
ls $1
ls $cur_path

export CMAKE_PREFIX_PATH=$1
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

mkdir build
cd build

cmake .. -D CMAKE_MAKE_PROGRAM=mingw32-make -G "MinGW Makefiles" \
--no-warn-unused-cli \
-D BUILD_WITH_SIMCORE=ON \
-D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
-D CMAKE_BUILD_TYPE:STRING=$BUILD_TYPE \
-D CMAKE_PREFIX_PATH:PATH=/cygdrive/d/Qt/6.5.3/msvc2019_64 \
-G Ninja \
-D CMAKE_C_COMPILER=gcc \
-D CMAKE_CXX_COMPILER=g++

cp "$cur_path/build-jenkins/x64/bin/qt6advanceddocking.dll" "$1/bin/qt6advanceddocking.dll"

cd -
