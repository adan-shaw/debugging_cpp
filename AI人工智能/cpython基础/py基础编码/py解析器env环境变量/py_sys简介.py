#coding=UTF-8



import sys
# sys.argv											# 命令行参数List,第一个元素是程序本身路径
# sys.exc_info()								# 获取当前正在处理的异常类,exc_type、exc_value、exc_traceback当前处理的异常详细信息
# sys.exc_clear()								# 用来清除当前线程所出现的当前的或最近的错误信息
# sys.exit(n)										# 程序,正常退出时exit(0)
# sys.maxint										# 最大的Int值(python2 专用)
# sys.maxunicode								# 最大的Unicode值(python2 专用)
# sys.modules										# 返回python 所有已导入的模块,key是模块名,value是模块实体
# sys.modules.keys()						# 返回python 所有已导入的模块名(列表)
# sys.builtin_module_names			# 返回python 所有内建模块名(列表)
# sys.path											# 返回python 模块的搜索路径(等同于PYTHONPATH环境变量的值)
# sys.exec_prefix								# 返回python 的安装的位置
# sys.executable								# 返回python exe程度的位置
# sys.stdout										# 标准输出
# sys.stdin											# 标准输入
# sys.stderr										# 错误输出
# sys.stdin.readline						# 从标准输入读一行,sys.stdout.write("a") 屏幕输出a
# sys.copyright									# 记录python版权相关的东西
# sys.platform									# 返回操作系统平台名称
# sys.api_version								# 解释器的C的API版本
# sys.version_info							# 获取Python解释器的版本信息
# sys.hexversion								# 获取Python解释程序的版本值,16进制格式如:0x020403F0
# sys.version										# 获取Python解释程序的版本信息
# sys.getwindowsversion					# 获取Windows的版本
# sys.byteorder									# 本地字节规则的指示器,big-endian平台的值是'big',little-endian平台的值是'little'
# sys.getdefaultencoding				# 返回当前你所用的默认的字符编码格式
# sys.getfilesystemencoding			# 返回将Unicode文件名转换成系统文件名的编码的名字
# sys.setdefaultencoding(name)	# 用来设置当前默认的字符编码(python2.7开始就被废弃)



# 最大的Int值
#print(sys.maxint)

# 最大的Unicode值
#print(sys.maxunicode)



# 打印当前平台os 的标识(linux2 = amd64, win32)
print(sys.platform)

# 打印python 编译器版本信息
print(sys.version)

# 解释器的C的API版本
print(sys.api_version)

# 获取Python解释器的版本信息
print(sys.version_info)


# 退出py 程序(当参数非0时, 会引发一个SystemExit异常, 无返回值)
sys.exit(0)





