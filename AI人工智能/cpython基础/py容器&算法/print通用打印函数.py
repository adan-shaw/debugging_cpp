#coding=UTF-8



# 关于'变量/容器'数据内容打印:
'''
	python 跟lua 不一样, python 的print() 函数, 会:
		* 自动识别类型;
		* 自动递归深度;
	然后自动遍历, 打印所有数据内容, 十分方便;
	因此, python 并不需要自己搭建通用的打印测试函数, 直接使用print()打印即可;
'''



# 废弃, 直接使用print() 即通用数据打印函数, 这点比lua 方便太多;
def print_all(p):
	print(p)



# 测试打印常量
print(int('9'))
print(float('9'))
print(complex('1+2j'))
print(bool('False'))
print(str('idiot'))
print(chr(48))		# str
print(ord('a'))		# int
print(hex(255))		# str
print(oct(20))		# str

# 测试打印None
print(None)



# 测试打印容器
ptuple=(1,2,3,4,"love","you","idiot",1,1,1,1,2,"","love",0)
print(ptuple)

plist=[[1,2],11,12,13,14,15,16,17,18,19]
print(plist)

pset={9,1,2,5,7,3,2,6,23}
print(pset)

pdict={ "A":191, "B":"love", "C":999, "D":"you" }
print(pdict)








#
# 废弃, dead!!
#
# 根据实例的类型判断, python 可以做出一个通用的打印函数, 直接递归就行;
# ps: 本模块性能消耗较大, 禁用于测试;
'''
def print_all(p):
	# 由于python 不支持switch-case, 则改用if #

	# 变量p为常量
	if isinstance(p,(int,float,complex,bool,str)):
		#print(t,p) # for test only
		print(p)

	# 变量p为None
	if p is None :
		#print(t,p) # for test only
		print(p)

	# 其实print() 函数本身就是通用打印的, 因此, 废弃本函数!!
	print(p)
'''
