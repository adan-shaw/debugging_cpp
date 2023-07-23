#!/bin/sh

build_home="./build"

if [ -d "$build_home" ];then
	rm -r $build_home
fi

mkdir $build_home

cd $build_home



# -Wdev: 默认选项, 有等于没有
cmake .. -D CMAKE_BUILD_TYPE=Debug

# --debug-output: 普通日志输出
#cmake .. -D CMAKE_BUILD_TYPE=Debug --debug-output

# --trace: 详细日志输出
#cmake .. -D CMAKE_BUILD_TYPE=Debug --trace
