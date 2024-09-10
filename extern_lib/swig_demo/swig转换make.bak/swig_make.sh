#!/bin/sh

# 只支持c++, 默认情况下, 不会对c 语言有特殊支持!!
# 反正c++ 包含c 语言, 所以这个项目实际上是以c++ 为标准语言的项目!!


# 预编译之后, 会生成配对文件:
# python/python3:
# 	*.py
# 	*_wrap.cxx
# lua/luajit(lua 以.so模块导入, 不需要*.lua 引用文件, 所以没有导出*.lua):
# 	*.lua[只有*.lua 没有引导文件]
# 	*_wrap.cxx
# go:
# 	*.go
# 	*_wrap.cxx
# php:
# 	php_*.h[比其他语言多生成一个头文件, 编译时需要引用, 才能正确swig_build_output.sh]
# 	*.php
# 	*_wrap.cxx





# swig 预编译声明文件(.i), 类似于c++ 编译器预编译
swig_i="./example.i"

# 前缀
swig_i_front_name=${swig_i%.*}

# 后缀
swig_i_rear_name=${swig_i##*.}



# 创建save 文件夹
if [ ! -d "./save" ];then
	mkdir save
fi
cd save

# 保存原始项目的数据
# (不能保存具有深度为2 的数据, 言外之意, 项目中不能有文件夹, 必须所有.h .cpp文件, 放在同一个文件夹下)
before_all_h=$(ls ../*.h)
before_all_cpp=$(ls ../*.cpp)
before_all_i=$(ls ../*.i)
before_all_sh=$(ls ../*.sh)

echo $before_all_h
cp $before_all_h .
echo $before_all_cpp
cp $before_all_cpp .
echo $before_all_i
cp $before_all_i .
echo $before_all_sh
cp $before_all_sh .

# 返回工作目录
cd ..





# 预编译成c++ && python模块
swig -c++ -python $swig_i
CXXFLAGS_TMP="-I/usr/include/python2.7"

# 预编译成c++ && python模块
#swig -c++ -python3 $swig_i
#CXXFLAGS_TMP="未知"


# 预编译成c++ && lua模块
#swig -c++ -lua $swig_i
#CXXFLAGS_TMP="/home/tarball/lua51/include/"
#CXXFLAGS_TMP="/home/tarball/luajit/include/luajit-2.1/"


# 预编译成c++ && go模块(go 需要指定int 变量类型的大小, 是32 or 64 bit, 
# 										因为go 编译出来的是机器码, 所以必须声明清楚才能用)
#swig -c++ -go -intgosize 64 $swig_i
#CXXFLAGS_TMP="未知"


# 预编译成c++ && php模块
#swig -c++ -php $swig_i
#CXXFLAGS_TMP="未知"





# 创建build 文件夹
if [ ! -d "./build" ];then
	mkdir build
fi
cd build



# 组装所有.h 头文件
all_h=$(ls ../*.h)

# 组装所有的.cpp 文件
all_cpp=$(ls ../*.cpp)

# 新增编译选项(一般是编译优化 + 不同script 脚本之间的头文件引用)
CXXFLAGS="-O2 $CXXFLAGS_TMP"

# 执行编译
g++ -fpic -c $all_h $all_cpp $CXXFLAGS

# 自动获取所有新生成的.o 文件
all_o=$(ls *.o)
echo $all_o
# 生成.so 动态库
g++ -shared $all_o -o $swig_i_front_name.so


