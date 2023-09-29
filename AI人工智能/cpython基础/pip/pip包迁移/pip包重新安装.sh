#!/bin/sh

# --find-links: 指定pip 包路径
# -r:           指定requirements.txt 文件的位置
# --target:     指定安装位置
#pip install --no-index --find-links=./pip_packages -r ./pip_packages/requirements.txt --target ../../../runtime/python/lib/python2.7/site-packages/
python -m pip install --no-index --find-links=./pip_packages -r ./pip_packages/requirements.txt --target ../../../runtime/python/lib/python2.7/site-packages/


# 如果pip 版本过低, 需要先升级pip
#apt-get install python-pip python-setuptools
#python -m pip install --upgrade pip --trusted-host mirrors.cloud.tencent.com


