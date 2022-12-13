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

			python gc 内存重用规则:
				* 对于简单对象(数值、字符串, 元组):
					简单的Python对象'数值、字符串, 元组(tuple不允许被更改)', 采用的是复制的方式(深拷贝), 
					也就是说当将另一个变量B赋值给变量A时, 虽然A和B的内存空间仍然相同, 
					但当A的值发生变化时, 会重新给A分配空间, A和B的地址变得不再相同(写时复制技术);

				* 对于复杂对象():
					复杂的Python对象'字典(dict), 列表(List)'等, 改变一个就会引起另一个的改变, 也称之为浅拷贝;
					永久共享, 想要深拷贝, 需要自己手动操作;
'''



print("简单对象, 写时复制")
a=(1,2,3,4,5)
b=a
print(id(a))
print(id(b))

a=(1,2,3)
print(a)
print(b)
print(id(a))
print(id(b))



print("复杂对象, 永远不复制, 需要你手动深拷贝")
A = { 'a':'hello', 'b':'world', 'c':1000.00 }
print(id(A))

A
print(id(A))

B=A
B
print(id(B))

A['a']='really?'
A
B
print(id(A))
print(id(B))



