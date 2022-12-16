#coding=utf-8



import sys
import gc

# ps: python的gc 其实和lua的gc, 都差不多, 大同小异;



#
# 1.python gc 的基础操作手段
#
# 启用/停用gc
gc.disable()
gc.enable()

# 判断是否启用gc
if not (gc.isenabled()):
	gc.enable()
else:
	print(gc.isenabled())



# 判断是否启用debug, 启用即封禁
if(gc.get_debug() != 0):
	gc.set_debug(0)
else:
	print(gc.get_debug())



# 手动命令gc 执行完全回收操作, 也可以指定0,1,2 三代cache 中的其中一代;
gc.collect()

# 打印回收步进, 当前步进计数(没有必要, 不需要操作)
print(gc.get_count())

# 获取当前回收步进max 值(达到这个值就执行一次完全回收)
print(gc.get_threshold())

# 设置当前回收步进max 值
gc.set_threshold(600, 10, 10)
print(gc.get_threshold())








#
# 2.python gc 原理探讨
#
# 返回变量在python vm 中的唯一标识符(是一串数字, 每个变量都不一样)
a = 'some text'
print(id(a))


# 返回变量在python vm 中的实体count 计数
print(sys.getrefcount(a))



# python gc 的原理:
'''
	Python的内存机制以金字塔行, 各层的工作任务如下:
		第0层是C中的malloc, free等内存分配和释放函数进行操作;
		第1层和第2层是内存池, 有Python的接口函数PyMem_Malloc函数实现, 当对象小于256K时有该层直接分配内存;
		第3层是最上层, 也就是我们对Python对象的直接操作;


	在c/c++中, 如果频繁的调用malloc()/free()时, 是会产生性能问题的,
	再加上频繁的分配与释放小块的内存会产生内存碎片;

	python 也是基于c/c++ 实现的, 这种问题python 也没办法避免, 因此, 在此基础上, python gc 的主要任务有:
		* 如果请求分配的内存在1~256字节之间, 就使用自己的内存管理系统, 否则直接使用malloc.
		* 小内存还是会调用malloc()分配内存, 但每次会分配一块大小为256k的大块内存.
		* 经由内存池登记的内存到最后还是会回收到内存池, 并不会调用c/c++ 的free()释放掉, 以便下次使用;
'''



# python gc重用'变量内存'的规则:
'''
	声明:
		python 中的字符串是一个不可变类型, 跟c/c++ 中的字符串很不一样;
		python 中的字符串可根据下标访问字符串中的单个字符, 但不能修改(只准访问,不允许修改), 否则报错:
			TypeError ‘str’ object does not support item assignment

		想要更改只能先释放字符串, 再重新装载新的字符串;
		因此, python 中实际上有3 种不可更改的简单变量:
			* number
			* string
			* tuple元组

		其它都是复杂变量, 就算是:
			array
			list
			deque
			dict
			...
			都是复杂变量!!


	因此, python 的'变量内存'重用规则如下(根据'左值/右值'的概念进行划分):
		* 左值=变量本身,
			右值='常量/实体常量/函数返回/new 关键字返回/copy拷贝函数返回/+号*号运算符重载', 
			则: 变量本身(即左值), 触发copy拷贝;

		* 左值=变量本身,
			右值=另一个变量的实体, 
			则: 变量本身(即左值), 引用另一个变量的实体(即右值);


		* 左值=复杂变量的子元素,
			右值='常量/实体常量/函数返回/new 关键字返回/copy拷贝函数返回/+号*号运算符重载', 
			则: 复杂变量的子元素(即左值), 触发copy拷贝, 复杂变量母体id地址不变;

		* 左值=复杂变量的子元素,
			右值=另一个变量的实体, 
			则: 复杂变量的子元素(即左值), 引用另一个变量的实体(即右值), 复杂变量母体id地址不变;


		* 左值=简单变量的子元素,
			右值=...
			则: 无意义, python 不允许修改简单变量!! (废弃)


	ps:
		如何深拷贝?
		答:
			python 复杂变量, 想要得到copy拷贝, 需要手动调用:
				* 复杂变量自带的copy() 函数;
				* 复杂变量自身的构造函数;
				* copy.deepcopy() 函数;
'''



print("左值=变量本身, 右值='常量/实体常量/函数返回/new 关键字返回/copy拷贝函数返回/+号*号运算符重载'")
a=(1,2,3,4,5)
b=(1,2,3,4,5)
print(id(a))
print(id(b))

print("左值=变量本身, 右值=另一个变量的实体")
a=(1,2,3,4,5)
b=a
print(id(a))
print(id(b))



print("左值=复杂变量的子元素, 右值='常量/实体常量/函数返回/new 关键字返回/copy拷贝函数返回/+号*号运算符重载'")
a=[1,2]
b=a
print(a,id(a))
print(b,id(b))
a[1]=9
print(a,id(a))
print(b,id(b))

print("左值=复杂变量的子元素, 右值=另一个变量的实体")
a=[1,2]
b=[3,4]
a[1]=b
print(a,id(a))
print(b,id(b))
b[1]=9
print(a,id(a))
print(b,id(b))



print("复杂对象, 母体id地址不变, 除非你手动深拷贝")
A = { 'a':'hello', 'b':'world', 'c':1000.00 }
B=A
print(id(A))
print(id(B))

A['a']='really?'
print(id(A))
print(id(B))

A['d']='really?'
print(id(A))
print(id(B))

print("复杂对象手动深拷贝: 构造函数")
A = { 'a':'hello', 'b':'world', 'c':1000.00 }
B=dict(A)
print(id(A))
print(id(B))

print("复杂对象手动深拷贝: copy()函数")
A = { 'a':'hello', 'b':'world', 'c':1000.00 }
B=A.copy()
print(id(A))
print(id(B))

print("复杂对象手动深拷贝: copy.deepcopy()函数")
import copy
A = { 'a':'hello', 'b':'world', 'c':1000.00 }
B=copy.deepcopy(A)
print(id(A))
print(id(B))




