#coding=UTF-8



#
# 1.python 的布尔类型值:
#
'''
关键字:
	False
	True
	None

字符串:
	除空字符串外, 任何字符串均为True

整数,浮点数字:
	除0 外, 任何数字均为True

列表,元组,集合,字典:
	除空列表外, 任何列表、元组、集合和字典均为True

None:
	None永远表示False

'''





#
# 2.if 基础写法
#
'''
	最好带() 小括号, 必须用':'分号+'换行/退格', 表示一个if 结构.
	if(): 结构不允许空执行, 可以用pass表示空执行
'''
x=1
if(x == 1):
	pass
elif(x != 0):
	print("x != 0")
else:
	print("x != 0 && x != 1")



# if 内嵌:
x=1
if(x == 1):
	if(x == 1):
		print("love yeah")
else:
	print("x != 1")





#
# 3.if 类对象
#
x=["apple", "banana"]
y=["apple", "banana"]
# '='号赋值, 并不会自动拷贝类对象, 只是代表: 该指针, 指向了这个类对象.
z=x


# is: 如果两个变量是同一个类对象, 则返回True.
if not (y is x):
	print("y[] is not x[]")
if(z is x):
	print("z[] is x")

# is not: 如果两个变量不是同一个类对象, 则返回True.
#if not (y is x):
if(y is not x):
	print("y[] is not x[]")


# in: 如果类对象中存在具有指定值的序列, 则返回True.
if("banana" in x):
	print("banana in x[]")

# not in: 如果类对象中不存在具有指定值的序列, 则返回True.
if("love" not in x):
	print("love not in x[]")





#
# 4.python 逻辑运算符(字母式)
#
x=5

# and 与
if(x>3 and x<10):
	print("and")

# or 或
if(x>3 or x<4):
	print("or")

# not 反
if not (x<3):
	print("not")





#
# 5.python 逻辑运算符(简写式)
#
# &: 如果两个位均为1, 则将每个位设为1. 
tmp=(1&1);print(tmp)

# |: 如果两位中的一位为1, 则将每个位设为1. 
tmp=(1|0);print(tmp)

# ^: 如果两个位中只有一位为1, 则将每个位设为1.
tmp=(1^0);print(tmp)

# ～: 反转所有位.
tmp=(~1);print(tmp)

# <<: 通过从右侧推入零来向左移动, 推掉最左边的位. 
tmp=(1<<1);print(tmp)

# >>: 通过从左侧推入最左边的位的副本向右移动, 推掉最右边的位. 
tmp=(1>>1);print(tmp)


