#
# 1.python 包的组成文件结构:
#
'''
	setup.py				安装文件, 描述压缩方式, pip依赖, 依赖包存放路径, 包含的*.py文件路径等;
	__init__.py			import 导入封装包时, 自动调用的init 初始化文件;
	*.py						封装包内的.py 源码文件;
	MANIFEST.in			指明封装包中各种资源文件的路径(需要在setup.py中设置include_package_data=True)

'''





#
# 2.python 安装包封装工具
#
'''
安装setuptools:
	apt-get install python-setuptools
	apt-get install python3-setuptools

	pip install setuptools
	pip3 install setuptools

	python2 -m pip install setuptools
	python3 -m pip install setuptools



python 是text 编码语言, 如果你不想代码泄漏, 可以直接用pyo,pyc 进行发行.
但pyo,pyc 也容易被人'逆向破解, 导出源码(逆向工程)'[python, java 都很容易被逆向导出源码]



封装py 安装包, 需要一个setup.py 文件, 
最好也添加__init__.py 初始化文件, 这样可以优化py 包的装载机制
'''






