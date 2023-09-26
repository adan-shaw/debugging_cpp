#coding=utf8
import copy



# python 高效编程与lua 的共性(解析型脚本的通病):
'''
	由于python 并没有local 局部变量关键字, 但高效应用内建模块的方法, 和lua 类似;
	例如:
		# 这样做最小引用, 是最好的, 有疑问, 请看: py最小引用优化测试.py
		import time
		ptime = time.time
		print(ptime())

	内建全局函数, 不需要引用优化了, 如: type(), max(), globals() 等, 这个lua 上也不需要引用优化的;
	但内建库/第三方库的函数, 使用最小引用优化后, 性能更好;

	很有意思的是, python 是一个优秀的算法语言, 所以math 数学库, 就是内建全局函数, 不需要引用优化;
	需要import 的第三方库,
	不需要import 的内建库, 如time, sys 等, 
	都需要最小引用优化;
'''



# locals()局部变量字典/globals()全局变量字典的简介:
'''
	* locals()只读, globals()可读可写;
	* locals()存在于函数内, globals()存在于'python进程'内;
	* 多'python进程'之间, globals()互相独立;
	* 访问locals()/globals() 不会有性能问题, 但'多线程'访问globals() 会有访问冲突问题;
	* 拷贝locals()/globals() 会有性能问题, 主要消耗是拷贝消耗;
	* locals()/globals() 返回值是不一样的, globals() 返回一个指针引用, locals() 返回一份浅拷贝copy;
'''

#
# globals() 全局变量
#
# 查看全局变量字典(包含变量, 子字典, list等)
print(globals())


# 实时访问全局变量(性能不太好)
print(globals()["__file__"])


# 实时新增全局变量(在非函数区定义的变量, 统一都当成全局变量, 跟c/c++/lua 语言一样)
tmp = 1
print(globals()["tmp"])


# 实时改变全局变量
globals()["tmp"] = 2
print(globals()["tmp"])


# 引用全局变量
gVal = globals()
gVal["tmp"] = 3
print(gVal["tmp"])
gVal["tmp"] = 4
print(globals()["tmp"])


# copy浅拷贝全局变量字典
gVal_copy=globals().copy()
#gVal_copy=dict(globals())
gVal_copy["tmp"] = 5
print(gVal_copy["tmp"])
print(globals()["tmp"])


# copy深拷贝全局变量字典(深拷贝失败)
'''
gVal_copy2=copy.deepcopy(globals())
gVal_copy2["tmp"] = 6
print(gVal_copy2["tmp"])
'''





# locals() 函数内的局部变量字典(只读)
def func():
	# 访问locals()
	print(locals())

	# 新增locals() 局部变量字典
	tmp = 1
	print(locals()["tmp"])

	# 浅拷贝locals() 局部变量字典
	# (不同于全局变量, locals()/globals() 返回值是不一样的, globals() 返回一个指针引用, locals() 返回一份浅拷贝copy)
	lVal_copy = locals()
	lVal_copy["tmp"] = 2
	print(lVal_copy["tmp"])
	print(locals()["tmp"])

func()


