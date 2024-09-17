#!/bin/sh

#
# protobuf 编译语法
#

# $SRC_DIR:  .proto 所在的源目录
# --cpp_out: 生成c++ 代码
# $DST_DIR:  生成代码的目标目录
# xxx.proto: 要针对哪个proto 文件生成接口代码

# format:
#protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/xxx.proto



# demo
#protoc -I=. --cpp_out=. ./test.proto

# better
SRC_DIR="."
DST_DIR="."
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/test.proto



#
# 编译测试文件
#
g++ -std=c++11 ./test.pb.cc ./main.cpp -o x -lprotobuf -pthread



