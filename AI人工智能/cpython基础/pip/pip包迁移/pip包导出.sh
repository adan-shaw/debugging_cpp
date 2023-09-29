#!/bin/sh

# 查看包版本
pip freeze

# 导出pip 包信息
pip freeze -l > requirements.txt

# 创建文件夹, 装载导出的pip包
mkdir pip_packages
cd pip_packages
mv ../requirements.txt .



# 从pip 包镜像站下载requirements.txt 中的包, 及其对应版本(版本不对会中断退出, please check)
# pip.conf 里面指定哪个url, 就--trusted-host 哪个url
pip download -r requirements.txt --trusted-host mirrors.cloud.tencent.com



cd ..
