#coding=utf-8



#
# python 的gc 回收机制
#
# python 的命令gc 强制回收变量
tmp = [1,2,3,4,5]
del tmp


# 你也可以使用lua 的方式, 将变量重新置换为一个轻变量, 让gc 触发回收操作(每次赋值, 都是创建一个新的变量)
tmp = [1,2,3,4,5]
tmp = None





# 类型判断测试数据
class A:
	tmp = "A"

class B(A):
	tmp = "B"

tmp = 2
a=A()
b=B()
'''
isinstance() 与 type() 区别：
	isinstance() 适合'类变量'的实体(子类是一种父类类型, 考虑继承关系
	type() 性能好
	isinstance() 功能多, 可判断'类变量'实体的类型
'''
# type() 只判断单个变量的类型, 不考虑继承关系, 绝对正确的类型匹配;
if type(tmp) is int:
	print("tmp is int")

if type(a) is A:
	print("a is class A")

if type(b) is not A:
	print("b is not class A")



# isinstance() 可判断多个变量的类型, 考虑继承关系, 相对正确的类型匹配;
if isinstance(tmp,int):
	print("tmp is int")

# True (测试tmp的类型, 是str,int,list 中的其中一个)
if isinstance(tmp,(str,int,list)):
	print("tmp is one of [str/int/list]")

if isinstance(a,A):
	print("a is class A")

if isinstance(b,A):
	print("b is class A")



# issubclass() 判断类的父子关系(很少用, 判断类型, 程序员自己也会看, 不需要出现在代码中)
if issubclass(B,A):
	print("B是A的子类")





# type() 的两种用法(type()创建新类的方法, 很少用!!):
'''
	type(obj): 
		判断变量实体的类型

	type(name, bases, dict): 
		创建新的类, 其中:
			name 表示类的名称;[没用的? 废弃?]
			bases 表示一个元组,其中存储的是该类的父类;
			dict 表示一个字典,用于表示类内定义的属性或者方法;
'''

# 使用type() 定义一个新类
class_name = type('C', (B,), dict(tmp='i am the new class'))

# 实化type() 创建的新类
c = class_name()
print(c.tmp)

# 使用type() 第一参数的class name, 被废弃;
'''
tmp = C()
print(tmp.tmp)
'''



