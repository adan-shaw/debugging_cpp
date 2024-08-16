#!/bin/sh

build_home="./build"

if [ -d "$build_home" ];then
	rm -r $build_home
fi

mkdir $build_home

cd $build_home



# -Wdev: 默认选项, 有等于没有
cmake .. -D CMAKE_BUILD_TYPE=Debug -Wdev

# --debug-output: 普通日志输出
#cmake .. -D CMAKE_BUILD_TYPE=Debug --debug-output

# --trace: 详细日志输出
#cmake .. -D CMAKE_BUILD_TYPE=Debug --trace



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
