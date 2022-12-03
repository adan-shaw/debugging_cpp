#coding=utf-8



# sys.modules								# 返回python 所有已导入的模块,key是模块名,value是模块实体
# sys.modules.keys()				# 返回python 所有已导入的模块名(列表)
# sys.builtin_module_names	# 返回python 所有内建模块名(列表)
# sys.path									# 返回python 模块的搜索路径(等同于PYTHONPATH环境变量的值)
# sys.exec_prefix						# 返回python 的安装的位置
# sys.executable						# 返回python exe程度的位置



import sys
import imp
imp.reload(sys)
if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
	sys.setdefaultencoding("utf-8")
	#print(sys.getdefaultencoding())
	#sys.exit()






print("打印python 所有内建模块名:")
for x in sys.builtin_module_names:
	print(x)

print("打印python 所有已导入的模块名:")
for x in sys.modules.keys():
	print(x)



print("打印python 模块的搜索路径(等同于PYTHONPATH环境变量的值):")
# ps: python2路径一定不能出现中文(就算print支持中文, *.py文件路径也一定不能出现中文路径, 否则容易报错)
print(sys.path)


# 添加py模块的搜索路径, 到py环境变量中
project_path="/home/adan/py_mod_home"
sys.path.append(project_path)
print(sys.path)



# 返回python 的安装的位置
print(sys.exec_prefix)

# 返回python exe程度的位置
print(sys.executable)


