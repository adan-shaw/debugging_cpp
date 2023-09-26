#coding=utf8



#
# 1.python 运行环境声明:
#
'''
python类似shell, 可以指明使用哪个python机(一般不推荐指认固定的python 机):
	#!/usr/bin/env python
	#!/usr/bin/env python3
	#!/usr/bin/python python
	#!/usr/bin/python3 python3

然后是文本编码格式声明:
	# -*- coding: UTF-8 -*-
	#coding=utf8
'''



# python 的编码格式(中文支持性):
'''
Python2 中默认的编码格式是ASCII 格式, 
				在没修改编码格式时, 无法正确打印汉字, 所以在读取中文时会报错.

Python3 中默认的编码格式是UTF-8, 可以直接使用中文.
'''





#
# 2.python 注释:
#

# 单行注释

'''
多行注释1 = 字符串, 占用一定内存, 最好还是单行注释. 不过业务机, 影响不大
'''

"""
多行注释2 = 字符串, 占用一定内存, 最好还是单行注释. 不过业务机, 影响不大
"""





#
# 3.保留关键字
#
'''
and       exec    not
assert    finally or
break     for     pass
class     from    print
continue  global  raise
def       if      return
del       import  try
elif      in      while
else      is      with
except    lambda  yield
'''





# 4.进入帮助手册模式:
'''
help()

然后你输入: 
	open     # 用来查看open API 的函数说明.
	q        # 退出help() 模式
'''

# 直接获取函数API 说明:
help("print")
help('print')





# 5.python3 专属
copyright()





# 6.python 发行协议声明:
license()





# 7.幕后感谢名单
credits()


