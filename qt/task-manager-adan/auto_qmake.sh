#!/bin/sh

mkdir build

cd build

qmake -makefile ../task-manager-adan.pro
make

g++ -g3 ../test_main.cpp -o r
g++ -g3 ../test_main.cpp -o e
g++ -g3 ../test_main.cpp -o v
g++ -g3 ../test_main.cpp -o n
g++ -g3 ../test_main.cpp -o g

chmod 755 ./task-manager-adan
chmod 755 ./r
chmod 755 ./e
chmod 755 ./v
chmod 755 ./n
chmod 755 ./g

# 查看task-manager-adan 是否带有-g3 符号信息
echo " *** if there is no any debug message about task-manager-adan: -g3, then you need to insert -g3 option into qmake script *** "
echo " *-* *************************************************************** *-* "
readelf -S ./task-manager-adan | grep debug
echo " *-* *************************************************************** *-* "

cd ..
