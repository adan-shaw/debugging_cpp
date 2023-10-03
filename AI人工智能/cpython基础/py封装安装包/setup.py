#
# setup.py file 编写:
#
# 新建setup.py 文件, 然后将下面的代码写入setup.py;



#coding:UTF-8
from setuptools import setup, find_packages
setup(
	# python 版本要求
	python_requires='>=2.7',
	# 应用名
	name='MyApp',
	# 版本号
	version='1.0',

	# 将./src 中的所有.py 文件, 全部包含进本程序中
	packages=find_packages('./src'),

	# 启用py安装包附带'数据清单data list'文件: MANIFEST.in
	include_package_data=True,

	# '数据清单data list'文件: MANIFEST.in中, 排除所有带.gitignore 后缀的文件
	exclude_package_date={'./res':['.gitignore']},

	# pip 依赖列表(pip 可以下载的库)
	install_requires=[
		'Flask>=0.10',
		'Flask-SQLAlchemy>=1.5,<=2.1',
	],

	# 互联网依赖包下载路径(pip 不能下载的库)
	dependency_links=[
		'http://localhost/dep1.tar.gz',
		'http://localhost/dep2.tar.gz',
	],

	# 对python 安装包进行压缩(压缩后一般用egg 后缀来存放)
	# 有些工具不支持zip 压缩, 压缩后也不方便调试, 建议将其设为zip_safe=False
	zip_safe=True,

	# 表明该安装包支持2to3
	use_2to3=True,
	# 忽略转换doc&tests 文档, 只转换py 代码
	convert_2to3_doctests=['doc/README.txt'],
	# fixers固定器list
	use_2to3_fixers=['zope.fixers'],



	# 其它信息
	author = "adan shaw",
	author_email = "adan_shaw@qq.com",
	description = "描述",
	license = "GNU",
	keywords = "包名(搜索关键字)",
	url = "http://包的下载站点.com/pkg/pkg_name.tar.gz"
)










