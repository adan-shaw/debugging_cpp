'''
1.__init__.py 文件的作用是: 
	每个py模块的包中, 都有一个__init__.py 文件
	将文件夹下, 所有的py 文件, 联合组成一个新的py模块, 供import 导入到python;


	具体引用哪些py 文件, 由setup.py 文件指定;
	具体怎样初始化这个封装包, 怎样初始化这些*.py 文件, 由__init__.py文件指定;
	(我们在导入一个包时, 实际上是导入了它的__init__.py文件)


	__init__.py 中, 可以引入其它python 封装包,
	如果引入python 内建封装包, 则不需要额外依赖包;
	如果引入第三方封装包, 则这个第三方封装包, 就会成为额外依赖包;


	__init__.py 文件可以为空, 类似空构造函数, 默认构造函数一样;
'''





'''
2.__init__.py 引入其它python 封装包: 
	在__init__.py 中import 导入的封装包, 为该模块的公共封装包,
	引入后, 该py 模块中的所有py 文件, 都可以直接引用这个封装包, 而不需要单独import;
	例如:
		在__init__.py 文件中, 声明导入:
			import re
			import urllib
			import sys
			import os
	则:
		该py 模块下的所有py 文件, 都把这些import 当成是公共导入资源, 可以直接使用, 不需要重复import

	缺点/优点:
		没有缺点, 只有优点!!
		python 是script 脚本机, 不是c/c++ 机械编码程序;
		不存在'多一个py 文件'占用模块, 就会多一份内存, 提早初始化, 性能更好(统一初始化)
'''
import re
import urllib
import sys
import os





# 3. __init__.py 中的__all__变量:
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












