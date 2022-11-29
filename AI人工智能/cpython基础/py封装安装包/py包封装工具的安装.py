#
# 1.python 安装包封装工具
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





#
# 2.常用的python 包封装工具命令:
#
# 创建egg 包:
python setup.py bdist_egg

# 创建tar.gz 包:
python setup.py sdist --formats=gztar

# 安装应用:
python setup.py install

# 开发方式安装:
python setup.py develop
