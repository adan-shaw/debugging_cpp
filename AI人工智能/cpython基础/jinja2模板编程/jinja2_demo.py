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



# 1.jinja2: python 变量+注释(简单的string替换)
name = "adan shaw"
age = 22
tm = Template("Hello {{ name }}, you are already {{ age }} years old {# 注释, 不会打印出模板 #}")
msg = tm.render(name=name,age=age)
print(msg)





# 2.jinja2: 类成员'变量/函数结果返回'(简单的string替换)
# 	[必须是返回string 的函数, 如果是print 那种无返回/打印数据到stdout 标准流的API 操作, 可以自定义jinja2 过滤器实现, 但比较复杂]
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





# 3.jinja2: 单点访问dict词典元素(简单的string替换)
person = { 'name': 'adan shaw', 'age': 22 }

tm = Template("My name is {{ p.name }} and I am {{ p.age }}")
print(tm.render(p=person))

tm = Template("My name is {{ p['name'] }} and I am {{ p['age'] }}")
print(tm.render(p=person))





# 4.jinja2: 从string 中读取template 模板string
data = '''
{% raw %}
His name is {{ name }}
{% endraw %}
'''

tm = Template(data)
msg = tm.render(name='Peter')
print(msg)





# 5.jinja2: 从file 中读取template 模板string应用中变量的值变更后, 将变更结果保存到file 中
file_path="tmp.txt"
school = [
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0},
	{'name': "", 'age': 0}
]

# 扫描'指定文件路径下的所有文件'(最好别扫描太多, 否则会触发循环遍历深入扫描)
file_loader = FileSystemLoader('.', 'utf-8', False)
# 把扫描到的文件路径都装进env (jinja2 的固定用法)
env = Environment(loader=file_loader)
# 使用jinja2 解析指定的file
template = env.get_template(file_path)
# 执行jinja2 渲染
output = template.render(school=school)
print(output)





# 6.jinja2: template 文件中的循环逻辑
ptuple=("aa","bb","c")
plist=["aa","bb","c"]
pdict={"aa":1,"bb":2,"c":3}
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

output = template.render(ptuple=ptuple,plist=plist,pdict=pdict,school2=school2)
print(output)





# 7.jinja2: template 文件中的调用默认过滤器函数
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
