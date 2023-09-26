#coding=utf8



#
#1.python 常规数字使用介绍
#
'''
Python 中有三种数字类型:
	int      整形(正数或负数, 没有小数, 长度不限)
	float    浮点(包含小数的正数或负数, 也可以是带有"e"的科学数字, 表示 10 的幂)
	complex  复数(复数用"j"作为虚部编写)
'''


# int 整形
x=10
y=37216654545182186317
z=-465167846

print(type(x))
print(type(y))
print(type(z))



# float 浮点
x=3.50
y=2.0
z=-63.78
x2=27e4
y2=15E2
z2=-49.8e100

print(type(x))
print(type(y))
print(type(z))
print(type(x2))
print(type(y2))
print(type(z2))



# complex 复数
x=2+3j
y=7j
z=-7j

print(type(x))
print(type(y))
print(type(z))








#
# 2.python 里面的数字, 不是字符串!! 而是单独的数据体.
#
'''
	python 为了做计算, 数字数据会单独用容器装载.
	所以, python数字变量中, 不能有任何字符!!

	想要混合输出'字符串 & 数字', 需要用格式化打印API
'''
a="Hello"
b="World"
x=66666666

# 错误, 数字&字符串不能混淆在一起运算(直接语法错误)
#c=a+b+x

# 错误, 数字&字符串不能混淆在一起定义(直接语法错误)
#err=666qqq



