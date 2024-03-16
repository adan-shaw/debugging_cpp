#!/bin/sh

build_home="./build"

if [ -d "$build_home" ];then
	rm -r $build_home
fi

mkdir $build_home

cd $build_home



# -G 指定编译时的Buildsystem 是什么系统(母系统, 不是target 系统, makefile 主要跑在Buildsystem 上面, 不需要跑在target 系统上)
:<<!
	cmake -G "NMake Makefiles" ..					# 生成Windows平台的Makefile 的相关选项
	cmake -G "Unix Makefiles"  ..					# 生成Linux/Unix平台下标准的Makefile 的相关选项
	cmake -G "Visual Studio"  ..					# 生成Visual Studio项目与解决方案的相关选项
	cmake -G "Ninja"  ..									# 生成编译优化Ninja 的相关选项
	cmake -G "Xcode"  ..									# 生成Xcode项目
!

# -Wdev: 默认选项, 有等于没有
#cmake -G "NMake Makefiles" .. -D CMAKE_BUILD_TYPE=Release -Wdev
cmake -G "Unix Makefiles" .. -D CMAKE_BUILD_TYPE=Release -Wdev 

# --debug-output: 普通日志输出
#cmake -G "Unix Makefiles" .. -D CMAKE_BUILD_TYPE=Release --debug-output

# --trace: 详细日志输出
#cmake -G "Unix Makefiles" .. -D CMAKE_BUILD_TYPE=Release --trace



# debug 技巧:
:<<!
	cmake 调试十分艰巨!!
	有时候报错, 不是因为前面的语句出问题, 而是后面的语言有严重的错误!!
	最明显的就是: 
		如果cmake 执行过程中, gcc/g++ 编译器都找不到了, 
		这种情况下, 肯定就是后面有语句出问题了, 建议最小化调试;

	还有各式各样的报错, cmake 跟普通脚本不一样:
		普通脚本, 运行到哪里, 哪里就安全, 就没有问题;
		但cmake 十分诡异, 如果后面的语句有问题, 甚至会导致cmake 运行过程中, gcc/g++ 编译器都找不到, 非常痛苦;
		编写cmake 真的一句一句添加比较可靠, 如果实在不行的话;
!



# 执行编译
make

echo "*******"
echo "do test"
echo "*******"
home=$(pwd)
/bin/sh -c "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$home $home/obj_out"
