#coding=utf-8



# import 导入语句简介
'''
	import 可以导入py模块( python3 中, 可以导入*.py 文件, 但不推荐你这样做)
	模块:
		请看./py封装安装包, 看看是如何封装一个python 包的;

	不管你执行了多少次import, 都只会被导入一次, 防止导入操作一遍又一遍地执行;
	这意味着: 导入的模块, 会直接进驻python 机的全局变量中;

	import 默认会自动调用__init__.py 文件, 初始化py模块;

	ps:
		from . import . 的用法:
			from package_name import py_filename
		只有在import 模块时, 可用!! 导入单个*.py 文件, 不可用from;



	导入*.py 文件, 请用__import__('py_filename'):
		被导入的*.py 文件中, 如有:
			如果有类定义, 则全局可见; 
			如果有类实体, 则成为全局变量; 
			如果有变量/函数, 则成为全局变量/全局函数;
	ps:
		导入单个*.py 文件时, 该*.py 文件会被自动编译成*.pyc 二进制封装, 
		否则, 不是二进制封装, 无法导入!!
'''



# 可被import语句导入的对象是以下类型:
'''
	+ 单个*.py文件
	+ 含有多个*.py 文件的py封装包, 含有__init__.py 初始化文件
	+ pip 模块库(pip 内建/最新安装的, 都可直接import 导入)
	+ C/c++共享链接库(已编译为.so/.dll)
	+ .pyo/.pyc/.pyd, 已编译成二进制封装的*.py 文件, 同样可以视作导入*.py 文件

	因此, import可导入的文件类型有:
		.py、.pyo、.pyc、.pyd、.so、.dll、py封装包)
'''





# 导入pip库模块
import sys
import os


# 添加当前路径到'py模块的搜索路径'中(py进程环境变量)
py_mod_path="."
sys.path.append(py_mod_path)


# 导入*.py 文件(同样需要去掉'.py'后缀)
__import__('py创建类')
# python3 中专用, 不建议用import 导入*.py 文件, 应该用__import__('')
#import py创建类



# import 导入操作的搜索顺序:
# 	每次执行import 导入文件时, python 机都会根据sys.path 中的先后顺序, 来查找目标文件/包/动态库;
print("打印'py模块的搜索路径'")
print(sys.path)



print("获取当前路径")
base_dir=os.path.dirname(os.path.abspath(__file__))
print(base_dir)



# reload 重新装载模块
'''
	由于python 是脚本程序, 因此python 模块可以被热重载(跟lua一样);
	重新调用import <package_name>, 并不能重新导入模块,
	热重载需用reload <package_name>
'''





'''
在导入模块时，解释器做以下工作:
	1.已导入模块的名称创建新的命名空间, 通过该命名空间就可以访问导入模块的属性和方法.
	2.在新创建的命名空间中执行源代码文件
	3.创建一个名为源代码文件的对象, 该对象引用<模块的名字空间>, 这样就可以通过<这个对象>访问模块中的函数及变量
	4.import 语句可以在程序的任何位置使用, 你可以在程序中多次导入同一个模块, 
		但模块中的代码仅仅在该模块被首次导入时执行, 后面的import语句只是简单的创建一个到模块名字空间的引用而已.
	5.sys.modules字典中保存着所有被导入模块的模块名到模块对象的映射.
'''



# python 导入'py库文件'的3种写法介绍:
'''
1.import [package_name]:
	导入一个'*.py文件'/py模块

2.from [package_name] import [py_filename]:
	导入一个py模块中某个'*.py文件'

3.__import__('py_filename'):
	动态导入py文件, 这种方式类似import, 不同的是: 
		不会自动调用__init__.py 文件, 不会初始化py模块;
		import 会自动调用__init__.py 文件, 会初始化py模块;
	因此, __import__('py_filename')一般用作导入py文件;



详细import导入用法, 有5种:
	+1.导入单个模块
		import [package_name]

	+2.导入单个模块中的某个函数api
		from [package_name] import [py_filename]

	+3.起别名:
		import [package_name] as [name1]
		from [package_name] import [py_filename] as [name2]

	+4.路径深度: <project_home>/path_A/path_B/c.py
		import path_A.path_B.c

	+5.通配符号的使用:
		# 通配搜索,导入文件夹下所有文件(默认参数)
		# 如果没有指明导入py包中哪个*.py文件, 则默认导入这个包所有的*.py文件)
		# 等价通配import:
		import path_A.path_B.*
		import path_A.path_B
		# 导入指定的*.py文件
		from [package_name] import [py_filename]
'''


