#!/bin/sh

key="./server.key"

gcc -g3 des.h des.c des_demo.c -o x 

# 加密-e (Encrypt)
./x -e $key input.file output.file
echo "***************************************"
echo "input.file(原文):       `cat input.file`"
echo "*.key(密钥):            $key"
echo "output.file(加密后):    `cat output.file`"
echo "***************************************"





# 解密-d (decrypt)
./x -d $key output.file input.file
echo "***************************************"
echo "input.file(还原文):     `cat input.file`"
echo "***************************************"
