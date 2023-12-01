#!/bin/sh

mkdir build

cd build

qmake -makefile ../task-manager-adan.pro
make

g++ ../test_main.cpp -o r
g++ ../test_main.cpp -o e
g++ ../test_main.cpp -o v
g++ ../test_main.cpp -o n
g++ ../test_main.cpp -o g

chmod 755 ./task-manager-adan
chmod 755 ./r
chmod 755 ./e
chmod 755 ./v
chmod 755 ./n
chmod 755 ./g

cd ..
