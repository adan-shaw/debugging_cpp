#coding=UTF-8



import re
import urllib
import sys
import os





# __init__.py 中的__all__变量:
__all__ = ['os', 'sys', 're', 'urllib'];

# 等价于:
import re
import urllib
import sys
import os

# __all__变量, 就是py 模块的初始化list, 
# 每一个元素, 都是一个'子py模块', 
# 每一个元素, 都会使用import 调用一次





# 4.精准导入, 减少初始化'包面积'
'''
	只需适当减少导入面积即可, 
	最小面积是单个py 文件, 并不是py 函数, 不能跟lua 一样, 细致到单个函数api;
	一般来说, 包的结构可以是这样的:
	package
		|- son_package1
			|- __init__.py
			|- a.py
				|- func_api1()
				|- func_api2()

		|- son_package2
			|- __init__.py
			|- b.py
				|- func_api1()
				|- func_api2()
			|- gson_package3
				|- __init__.py
				|- c.py
					|- func_api1()
					|- func_api2()
'''
# 有以下几种导入方式:
# 将son_package1模块中的a.py导入全局命名空间
import son_package1.a

# 将son_package1模块中的a.py导入全局命名空间(写法2)
from son_package1 import a
from son_package2 import b

# 迭代包写法
from son_package2.gson_package3 import c





