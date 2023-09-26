#coding=utf8

import time



# 测试上限
test_max=10000000



# 无引用优化测试(需要大概2 秒)
print(time.time())
while(test_max > 0):
	time.time()
	test_max-=1
print(time.time())



# 有引用优化测试(秒杀, 瞬间完成)
ptime=time.time
print(ptime())
while(test_max > 0):
	ptime()
	test_max-=1
print(ptime())



# 证明: 最小化引用, 是python/lua 通用脚本优化的基本准则, 是边读边解释语言的基本准则, 通用优化规则;
# 未编译的情况下, python2.7 比python3.7 快, 无解了!! python3.11 同样很慢, 和python3.7 差不多;



# 尝试编译之后, 执行编译块, 执行速度仍然没有差异, 还是必须优化!!:
# python -OO -m py_compile py最小引用优化测试.py
# python3 -OO -m py_compile py最小引用优化测试.py



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
