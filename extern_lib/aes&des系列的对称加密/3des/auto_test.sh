#!/bin/sh

key="1234567890abcdefghijklmn"
memory_size=1024
ncores=1

gcc -g3 3DES.h 3DES.c des_demo.c -o x 
gcc -g3 3DES.h 3DES.c 3des_demo.c -o x2

echo "***************************************"
./x
echo "***************************************"


# 加密-e (Encrypt)
./x2 -e $memory_size $ncores $key input.file output.file
echo "***************************************"
echo "input.file(原文):       `cat input.file`"
echo "*.key(密钥):            $key"
echo "output.file(加密后):    `cat output.file`"
echo "***************************************"





# 解密-d (decrypt)
./x2 -d $memory_size $ncores $key input.file output.file
echo "***************************************"
echo "input.file(还原文):     `cat input.file`"
echo "***************************************"
