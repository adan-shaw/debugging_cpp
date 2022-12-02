#coding=utf-8

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

