#coding=utf-8



# ps: 以下的方式, 实用意义都有限, 无太多参考价值, 只做展示python function 的用法;



# 普通函数
def max(a, b):
	# python 中没有3目运算符, 将就一下
	if a > b:
		return a
	else:
		return b

print(max(1, 9))



# 多值返回函数
def swap(a, b):
	# 高速交换方法
	a,b = b,a
	return a,b

a,b = swap(1, 9)
print(a,b)



# 设置默认参数值函数
def S(r, P=3.14):
	# 计算圆面积的方式
	return r*r*P/2

print(S(2))



# 引用赋值
def appoint(m_list):
	m_list.append([1,2,3,4])
	return

m_list=[]
appoint(m_list)
print(m_list)





# 装饰器: 多值参数传入
'''
	Python中有两种多值参数:
		参数名前增加一个*可以接收元组, 一般用*args存放元组参数, args是arguments缩写;
		参数名前增加两个*可以接收字典, 一般用**kwargs存放字典参数, kw是keyword缩写;
'''
# 如果入参是变量, 那么多值参数传递遵循拆包语法
def test(*args, **kwargs):
	print(args)
	print(kwargs)

tup = (1, 2, 3)
d = {"name":"horizon","11":1}

# 拆包语法
test(*tup, **d)





