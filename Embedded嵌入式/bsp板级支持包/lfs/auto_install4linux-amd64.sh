#!/bin/sh

# 快速重建, 就是:
# 	* 将编译好的二进制bin工具, 直接拷贝到/usr/bin
# 	* 将编译好的二进制lib工具, 直接拷贝到/usr/lib
# 	* 将编译时用到的.h 头文件, 直接拷贝到/usr/include

sudo cp ./buildifier-linux-amd64 /usr/bin/buildifier
sudo cp ./buildozer-linux-amd64 /usr/bin/buildozer
sudo cp ./unused_deps-linux-amd64 /usr/bin/unused_deps

chmod 755 /usr/bin/buildifier
chmod 755 /usr/bin/buildozer
chmod 755 /usr/bin/unused_deps

buildifier --help
buildozer --help
unused_deps --help
