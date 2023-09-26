#coding=utf8



# 需要排序的场合, 再复杂也用list!!



#
# key=cmp_to_key() 的用法:
#
from functools import cmp_to_key

# cmpEx() 比较函数的编写规则:
'''
	如果逻辑上认为a < b,  返回 -1
	如果逻辑上认为a > b,  返回 1
	如果逻辑上认为a == b, 返回 0
'''
def cmpEx(x, y):
	return x-y

a = [1, 0, -5, 3, 9, 4, -6, 8, -7, 0, 12]
print(sorted(a, key=cmp_to_key(cmpEx)))





#
# lambda 关键字的用法:
#
L = [('b', 9), ('a', 1), ('c', 3), ('d', 4)]

# 假设每个元素为x, 以每个元素的'第二个值', 作为比较对象;
print(sorted(L, key = lambda x: x[1]))

# 假设每个元素为x, 以每个元素的'第一个值', 作为比较对象;
print(sorted(L, key = lambda x: x[0]))

# 假设每个元素为y, 以每个元素的'第一个值', 作为比较对象; (关键字可以随意更换, 但不建议使用两个不同的关键字, 因为一个list 没有两个'每个元素')
print(sorted(L, key = lambda y: y[0]))



# 复杂list 排序
a = [('win', 3, 'b'), ('linux', 2, 'a'), ('mac', 1, 'd'), ('linux0.0', 2, 'e'),('MAC', 1, 'c')]
print(sorted(a, key=lambda x: x[1]))
print(sorted(a, key=lambda x: x[2]))
# x[1] 相同的情况下, 比较x[2]的大小
print(sorted(a, key=lambda x: (x[1], x[2])))



# 字符串先切割, 后根据'首个字符的ascii码'大小排序
a = "This is a test string from Andrew"
a.split()
sorted(a, key=str.lower)





# 复杂list 排序demo(list 内嵌dict):
school=[
	{
		'name': 'lisi',
		'age': 18,
		'iq': 88
	},
	{
		'name': 'nisi',
		'age': 20,
		'iq': 77
	},
	{
		'name':'zhangsan',
		'age':20,
		'iq':98
	},
	{
		'name': 'wangwu',
		'age': 20,
		'iq': 30
	},
	{
		'name': 'yangqing',
		'age': 15,
		'iq': 30
	},
	{
		'name': 'mike',
		'age': 22,
		'iq': 21
	}
]

def cmpEx2(a,b):
	# 第一顺位: iq
	if a['iq']>b['iq']:
		return 1
	elif a['iq']<b['iq']:
		return -1
	# 第二顺位: age
	elif a['age']>b['age']:
		return -1
	elif a['age']<b['age']:
		return 1
	else:
		return 0

print(sorted(school,key=cmp_to_key(cmpEx2)))


