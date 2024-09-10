#!/bin/sh



# 不存在save 备份文件夹时, 拒绝执行!!(防止删空了所有数据)
if [ ! -d "./save" ];then
	echo "*********************************************************"
	echo "./save 不存在, 拒绝执行swig_restore.sh, 防止数据丢失"
	echo "*********************************************************"
	exit 0
fi


# 删除当前文件夹所有文件(只查找当前文件夹, 不做深入探索)
find . -maxdepth 1 -type f -exec rm {} \;

# 还原源码文件
find ./save -maxdepth 1 -type f -exec cp {} . \;

# 删除build
if [ -d "./build" ];then
	rm -r ./build
fi

# 删除save
if [ -d "./save" ];then
	rm -r ./save
fi
