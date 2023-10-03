#coding=UTF-8



# python 下划线的特殊用途(简称: 魔术方法, magic):
'''
	类:
		以单下划线开头'_xxx'的代表不能直接访问的类属性, 需通过类提供的接口进行访问, 不能用 from xxx import * 而导入;
		以双下划线开头的'__xxx'代表类的私有成员;
		__init__() 代表类的构造函数;

	包:
		__init__.py	: 不同于类的__init__(), 这是一个python 包的初始化*.py 文件;
		__name__		: 每个python 包的包名(每个python 包唯一)
		__main__		: python 机正在运行的包(每个python 机唯一)
		__all__			: 指明python 包中有多少个*.py 文件
		 __doc__		: 包的注释文本
		__file__		: 
		__call__		: 
		__slots__		: 
		...
'''



# python 构建类的基础准则:
'''
	尽量使用一个python 类, 一个python 文件;


	构造函数:
		__init__()

	析构函数:
		__del__()

	让类实体能够像函数一样被调用(python 特有):
		__call__(): 
			调用__call__()之前, 类必然会调用__init__() 构造函数first, 类的创建过程, 释放过程不变, 
			只是使用方式, 可以模仿函数一样调用, 有点类似运算符重载;
			同时, __call__() 也有权访问类中的私有成员;


	私有'变量/函数':
		以'__xxx' 双下划线开头的, 都是私有'变量/函数', 只有类内部可以访问;

	self-python类的函数定义:
		在类的内部, 使用'def关键字'来定义一个方法, 类方法必须包含参数self, 且为第一个参数, 
		self 代表的是类的实例, 类似c/c++ 中的this 指针;

	classmethod-python类的静态函数定义:
		将某个类方法设置为static 静态方法, 由所有类实体共享一份函数实体, 类似c/c++ 中的static 关键字;
		类中的static 静态方法, 不能被外部调用, 只能由类中的元素调用;


	super()超级调用(只能在类内部使用, 类似self 指针):
		(慎用, python2/3 不兼容, python3 中用得比较多; python2 中, 你需要换一种思路, 不需要执着在一个死调用方法上)
		子类调用函数时, 偶尔会用到super();
		如果子类与父类都有这个函数(同名函数), 则local 本地原则, 调用子类中的同名函数;
		如果子类没有这个函数, 父类有这个函数, 则调用父类中的函数;
		如果子类与父类都没有这个函数, 则报错;
'''



# 父类
class father():
	# 构造函数
	def __init__(self):
		# 调用静态函数, 初始化静态变量
		self.func_x3()

	# 析构函数
	def __del__(self):
		# 调用伪造析构函数
		self.__func_x2()



	# 公开(可继承)
	x1 = 111
	#self.x1 = 111 (类变量定义, 可以省略self)

	def func_x1(self):
		print("father: self.func_x1():", self.x1)
		return

	def func_x11(self):
		print("father: self.func_x11():", self.x1)
		return



	# 私有(不可继承,'__'双下划线打头)
	__x2 = 999

	def __func_x2(self):
		print("father析构: self.__func_x2():", self.__x2)
		return



	# 静态(所有类实体共享, 唯一; 只能修饰方法, 不能修饰变量)
	#@classmethod
	x3 = 0

	@classmethod
	def func_x3(self):
		# 静态'变量/函数', 只初始化一次
		if(self.x3 == 0):
			self.x3 = 333
			print("father构造: self.func_x3():", self.x3)
		return



# 子类
class son(father):
	def __init__(self):
		print('son构造: __init__()')

	# 公开(可继承)
	x1 = 111000

	def func_x1(self):
		print("son: self.func_x1():", self.x1)
		return

	def func_x12(self):
		print("son: self.func_x12():", self.x1)
		#python3 中有效
		#super().func_x1()
		#super().func_x11()
		return




# 测试
a = father()
a.func_x1()

b = father()
b.func_x1()

c = son()
c.func_x12()





