#coding=UTF-8



# 强转为bool 类型
tmp = bool(1)
print(tmp)
# True

tmp = bool(0)
print(tmp)
# False

tmp = bool(None)
print(tmp)
# False





# 强转为int 整数(不能传入浮点, 复数)
print(int())
# 0 (不传入参数时, 得到结果0)

print(int('9'))
# 9





# 强转为float 浮点
print(float())
# 0.0

print(float(3))
# 3.0

print(float('3'))
# 3.0





# 强转为string 浮点
print(str())
# ''

print(str(None))
# 'None'

print(str(123))
# '123'





# 强转为complex 复数
print(complex(1, 2))
# (1 + 2j)

print(complex("1"))
# (1 + 0j) [当做字符串处理]

print(complex("1+2j"))
# (1 + 2j) [注意:在"+"号两边,不能有空格, 否则会报错]





# 强转为二进制数据块, 返回一个新的缓冲区(只有一个形参)
print(bytes('love you'))
print(bytes(123))
print(bytes([1,2,3,0]))





#
# 创建一个字节数组(每个元素大小都限定1字节, 类型不限, 可以为char, int[0,255]等)
#
# 当输入int n时, 返回n 个int 8bit的字节数组(无效?)
print(bytearray(123))

# 当输入str 时, str中, 每个char 都是一个元素, 可指定char 类型为'utf-8'
print(bytearray('love you', 'utf-8'))

# 当输入number list 时, list 中每个元素, 都应该属于[0,255]区间内, 否则, 有一个元素不属于这个区间都报错(无效?)
print(bytearray([1,2,3,0]))


# 当不输入参数时, 默认创建一个空字节数组''(无效?)
print(bytearray())





# 强转api 总览:
'''
	int(x)			将变量x强转为一个整数
	long(x)			将变量x强转为一个长整数
	float(x)		将变量x强转为一个浮点数
	complex(x)	将变量x强转为一个复数

	tuple(x)			将变量x强转为一个元组
	list(x)				将变量x强转为一个列表
	set(x)				将变量x强转为一个集合
	dict(x)				将变量x强转为一个字典(由于字典有'键-值对',所以字典只能自己)
	frozenset(x)	将变量x强转为'不可变'集合

	chr(x)		将一个整数, 强转为一个字符
	unichr(x)	将一个整数, 强转为Unicode字符
	hex(x)		将一个整数, 强转为一个十六进制字符串
	oct(x)		将一个整数, 强转为一个八进制字符串
	ord(x)		将一个字符, 强转为它的整数值

	str(x)		将变量x, 强转为字符串
	repr(x)		将变量x, 强转为'字符串表达式'
						(常用于将集合类型的对象,强转为'字符串表达式',方便'解析器'进行解析.
						 一般情况下, 不可将类api 进行强转.)
'''
