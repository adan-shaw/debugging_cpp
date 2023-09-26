#coding=utf8



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





# 判断变量类型/类实体的类型
d = 2;
print(isinstance(d,int)) # True
print(isinstance(d,str)) # False
print(isinstance(d,(str,int,list))) # True, d是int, 是这3 个类型的其中之一
print(isinstance(a,father)) # True
print(isinstance(b,father)) # True
print(isinstance(c,father)) # True
'''
isinstance() 判断规律如下:
	*1.如果定义了__nonzero__()方法, 会调用这个方法, 
		 并按照返回值判断这个对象等价于True还是False, 以此来判断类实体是否创建成功;

	*2.如果没有定义__nonzero__()方法但定义了__len__()方法, 会调用__len__()方法, 
		 当返回0时为False, 否则都为True;

	*3.如果都没有定义, 所有的对象都为True, 只有None变量根本不存在, 则返回False;
'''



# 以列表的形式, 返回类实体的所有元素(包括类中的变量+API)
'''
	如果类实体包含方法__dir__(), 该方法将被调用;
	如果类实体不包含__dir__(), 该方法将最大限度地收集类实体的匹配信息;
	(这个函数有一定消耗, 会触发遍历和重建列表)
'''
api_list=dir(a)
print(api_list)



# vars()参数可以是: 类实体/类定义声明, 
'''
	当参数=类实体时, 返回类的__dict__属性的内容(前提是有定义__dict__属性, 否则返回{}空字典)
	当参数=类定义声明时, 返回类的所有元素(包括公开/私有的), 并打印详细信息
'''
api_dict=vars(a)
print(api_dict)
api_dict=vars(father)
print(api_dict)





#
# 类实体操作(以下操作, 均需类实体, 不能是类定义声明):
#
# hasattr() 检查类实体是否含有某个元素, 存在&可被访问, 返回True; 不存在or 不能被访问的私有变量, 返回False;
print(hasattr(a,'x1'))
print(hasattr(a,'__x2'))
print(hasattr(a,'x3'))



# getattr() 获取类实体的元素值
print(getattr(a,'x1'))
print(getattr(a,'x3'))

# 不存在/不可访问的元素, 但提供了'自己输入的默认值', 则报错时, 返回默认值;
# 不存在/不可访问的元素, 又未提供默认值, 则使用系统报错信息;
# 报错必然会终止继续运行;
#print(getattr(a,'__x2'),"if __x2 is cant found/private, return this default value")



# setattr() 设置类实体的元素值(私有变量如果被重置, 则会变成public公开变量)
setattr(a,'x1','101010');
print(a.x1)

setattr(a,'__x2','202020');
print(a.__x2)

setattr(a,'x3','303030');
print(a.x3)

print(hasattr(a,'x1'))
print(hasattr(a,'__x2'))
print(hasattr(a,'x3'))
print(a.x1)
print(a.__x2)
print(a.x3)



# delattr() 删除类实体的元素
delattr(a,'__x2')

# 但可判断一下元素是否可以被访问, 不存在/private私有变量, 都会是False
print(hasattr(a,'__x2'))
# 删除后访问会报错!!
#print(a.__x2)



# callable() 检测对象是否可被调用(类中的实例必须有__call__()方法才可调用)
class B:
	def __call__(self):
		print('instances are callable now.');

callable(B);	# True 类B是可调用对象
b = B();			# 调用类B
callable(b);	# True 实例b是可调用对象
b();					# 调用实例b成功









