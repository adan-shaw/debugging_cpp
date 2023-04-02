#coding=utf-8



import sys

# 打印版本字符串全集, 格式如下: sys.version_info(major=3, minor=6, micro=6, releaselevel='final', serial=0)
print(sys.version_info)

# 打印主要版本号, 2/3
print(sys.version_info.major)



# 判断python 版本
if(sys.version_info.major == 3):
	if(sys.version_info.minor <= 3):
		print("<=3.3")
	else:
		print(">3.3")

if(sys.version_info.major == 2):
	print("2")

