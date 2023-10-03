#coding=UTF-8



# 设置python2/3 的统一utf-8 编码格式的方式:
import sys

if(sys.version_info.major == 3):
	# 少于3.3 跟2.7 类似
	if(sys.version_info.minor <= 3):
		import imp
		imp.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("utf-8")
	else:
		import importlib
		importlib.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("utf-8")

if(sys.version_info.major == 2):
	import imp
	imp.reload(sys)
	if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
		sys.setdefaultencoding("utf-8")



from jinja2 import Template, Environment, FileSystemLoader



# 1.jinja2 模板代码之间的做if判断
students=["adan","shaw","handsome","donkey","idiot"]

students_info = [
	{'name': 'Andrej', 'age': 34},
	{'name': 'Mark', 'age': 17},
	{'name': 'Thomas', 'age': 44},
	{'name': 'Lucy', 'age': 14},
	{'name': 'Robert', 'age': 23},
	{'name': 'Dragomir', 'age': 54},
]

content = 'This is about page'

file_loader = FileSystemLoader('.', 'utf-8', False)
env = Environment(loader=file_loader)

template = env.get_template('about.html')

output = template.render(content=content, students=students, students_info=students_info)
print(output)
