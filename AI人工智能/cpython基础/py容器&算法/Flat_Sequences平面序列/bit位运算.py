#coding=utf-8



# python bit位运算简介:
# 	python 中, 只有int 数据类型, 才可以进行位操作, 与c 语言类似;



# 适合bit 位操作的, 一般都是string/int 变量类型
x="-111"
y=999





# 判断输入的变量, 是否是int(废弃, 根本不需要用到int 类型判断; 直接用强转更安全)
if type(y) is int:
	print("tmp is int")


# 将任意变量强转为int 变量(不管是什么变量类型, 进行位操作之前, 直接强转int(), 防止出错)
'''
	如果变量的值, 是非数字, 或出现任何问题导致转换失败, 
	python 直接报错终止执行, 报错;
	所以强转int 操作, 是安全的, 不能转换, python 会直接报错;

	因此, 在进行bit 位操作之前, 根本不需要先判断变量的类型是否是int, 直接做一个int强转, 最安全;
'''
x=int(x)
y=int(y)





# bit 位运算
z = x | y
print(x,y,"或or",z)

z = x ^ y
print(x,y,"异或xor",z)

z = x & y
print(x,y,"与and",z)

z = x << 2
print(x,"左移<<",z)

z = x >> 2
print(x,"右移>>",z)

z = ~x
print(x,"取反not",z)





# int 强转为'二进制数据'的字符串
z=bin(x)
print(z)

# 求int 转换为'二进制数据'字符串时, 有多少个位;(多少个0/1)
print(x.bit_length())

# 去除int 转换为'二进制数据'字符串时, '-0b'前缀(-表示负数, 正整数无符号, 只有0b)
z=x
z=bin(z)
z = z.lstrip('-0b')
print(z)





# float浮点强转为int
x=5.9
y=5.4
z=5.0

# 强转不会四写五入, 而是直接丢弃小数位的值, 想要四写五入, 请用round()
print("float可无损转换为int?",x.is_integer(),"转换后:",int(x))
print("float可无损转换为int?",y.is_integer(),"转换后:",int(y))
print("float可无损转换为int?",z.is_integer(),"转换后:",int(z))

# float浮点强转为int(四写五入)
'''
	第二个参数是float 变量;
	第二个参数是保留的小数位;
		(0等于1个小数位都不保留, 直接转变为int, 
		 保证float 强转为int 时, 可以四写五入)
'''
print("float浮点强转为int(四写五入)",int(round(x,0)))
print("float浮点强转为int(四写五入)",int(round(y,0)))
print("float浮点强转为int(四写五入)",int(round(z,0)))



