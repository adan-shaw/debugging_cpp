#coding=UTF-8



import sys

# imp 模块会被警告, 不推荐使用, 直接忽略即可;
import imp

# 重载sys 模块, 让setdefaultencoding() 生效
imp.reload(sys)



# 显示字节序: 大端=big, 小端=little
print(sys.byteorder)




# sys.getdefaultencoding() = *.py 文件的默认编码;
# sys.getfilesystemencoding() = file system 文件系统的默认编码;
'''
	如果*.py 文件的默认编码, 与file system 文件系统的默认编码不一致, 则统一修改为: UTF-8;
	这样做可以保证python 程序不出现中文乱码!!
'''
if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
	#print(sys.getdefaultencoding())
	sys.setdefaultencoding("UTF-8")
	#print(sys.getdefaultencoding())
