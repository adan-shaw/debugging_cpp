#coding=utf-8



# 设置python2/3 的统一utf-8 编码格式的方式:
import sys

if(sys.version_info.major == 3):
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



# 1.jinja 模板替换: python 变量 + 注释
name = "adan shaw"
age = 22
tm = Template("Hello {{ name }}, you are already {{ age }} years old {# 注释, 不会打印出模板 #}")
msg = tm.render(name=name,age=age)
print(msg)





# 2.jinja 模板替换: 类成员'变量/函数结果返回'
class person:
	IQ = 180
	def __init__(self, name, age):
		self.name = name
		self.age = age
	def getAge(self):
		return self.age
	def getName(self):
		return self.name

A = person('adan shaw', 22)

tm = Template("My name is {{ p.getName() }} and I am {{ p.getAge() }} (ps: IQ = {{ p.IQ }})")
msg = tm.render(p=A)
print(msg)





# 3.jinja 模板替换: dict词典元素
person = { 'name': 'adan shaw', 'age': 22 }

tm = Template("My name is {{ p.name }} and I am {{ p.age }}")
print(tm.render(p=person))

tm = Template("My name is {{ p['name'] }} and I am {{ p['age'] }}")
print(tm.render(p=person))





# 4.jinja 模板替换: 
data = '''
{% raw %}
His name is {{ name }}
{% endraw %}
'''

tm = Template(data)
msg = tm.render(name='Peter')
print(msg)





# 5.jinja 模板替换: 从file 中读取应用数据, 替换应用中变量的值(改变量必须存在, 且与file中的数据对齐)
# 	应用中变量的值变更后, 将变更结果保存到file 中
school = [
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0}
]

# 扫描指定文件路径下的所有文件(最好别太多, 否则会触发遍历)
file_loader = FileSystemLoader('.', 'utf-8', False)
# 把这些文件都装进env (jinja 的固定用法)
env = Environment(loader=file_loader)
# 使用jinja 解析指定的file
template = env.get_template('tmp.txt')
# 执行jinja 解析
output = template.render(school=school)
print(output)





# 6.jinja 模板替换: 从file 中读取应用逻辑, 计算应用中变量的值(复杂版本:file中带有python 逻辑语句)
school2 = [
	{'name': 'Andrej', 'age': 34},
	{'name': 'Mark', 'age': 17},
	{'name': 'Thomas', 'age': 44},
	{'name': 'Lucy', 'age': 14},
	{'name': 'Robert', 'age': 23},
	{'name': 'Dragomir', 'age': 54},
]

file_loader = FileSystemLoader('.', 'utf-8', False)
env = Environment(loader=file_loader)
env.trim_blocks = True
env.lstrip_blocks = True
env.rstrip_blocks = True

template = env.get_template('tmp2.txt')

output = template.render(school2=school2)
print(output)





# 7.jinja 模板替换: 从file 中读取应用逻辑, 计算应用中变量的值
cars = [
	{'name': 'Audi', 'price': 23000}, 
	{'name': 'Skoda', 'price': 17300}, 
	{'name': 'Volvo', 'price': 44300}, 
	{'name': 'Volkswagen', 'price': 21300}
]

file_loader = FileSystemLoader('.', 'utf-8', False)
env = Environment(loader=file_loader)

template = env.get_template('tmp3.txt')

output = template.render(cars=cars)
print(output)





# 7.jinja2 模板代码之间的内嵌(about.html 中内嵌base.html, 内嵌是通过extends指令完成的)
content = 'This is about page'

file_loader = FileSystemLoader('.', 'utf-8', False)
env = Environment(loader=file_loader)

template = env.get_template('about.html')

output = template.render(content=content)
print(output)
