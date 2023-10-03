#coding=UTF-8



# 设置python2/3 的统一UTF-8 编码格式的方式:
import sys
import random

if(sys.version_info.major == 3):
	# 少于3.3 跟2.7 类似
	if(sys.version_info.minor <= 3):
		import imp
		imp.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("UTF-8")
	else:
		import importlib
		importlib.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("UTF-8")

if(sys.version_info.major == 2):
	import imp
	imp.reload(sys)
	if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
		sys.setdefaultencoding("UTF-8")





# python 的字符串本质:
'''
	表示一串unicode字符的字节'array数组', 即字符串实际是用'array数组'存储的;
	Python2默认使用编码ascii, 不支持中文;
	Python3默认使用编码UTF-8, 支持中文, 其英文编码部分和ascii相同;
	ascii = 1bit, UTF-8 = 2bit;

	python 中的字符串是一个不可变类型, 跟c/c++ 中的字符串很不一样;
	python 中的字符串可根据下标访问字符串中的单个字符, 但不能修改(只准访问,不允许修改), 否则报错:
		TypeError ‘str’ object does not support item assignment
'''


#
# 1.定义字符串变量(不要命名变量为str, 因为str()是内置函数名, 会与关键字冲突)
#

# 单行
pstr1='love you asshole中文'
pstr2="love you asshole中文"

# 多行(ps: 注释字符串会产生局部/全局字符串数据)
pstr3='''
love you 
asshole
中文
'''
pstr4="""
love you 
asshole
中文
"""

print(pstr1)
print(pstr2)
print(pstr3)
print(pstr4)





#
# 2.字符串变量格式化(类似c语言中的printf())
#
x=3
y=567
z=49.95

# 默认参数顺序
tmp="I want {} pieces of item {} for {} dollars.中文"
print(tmp.format(x, y, z))

# 强制指定参数顺序
tmp="I want {2} pieces of item {0} for {1} dollars.中文"
print(tmp.format(x, y, z))

# 格式化的另一种写法:
tmp="%s 字符串: %s\n长度是: %d" %("别样写法:\n", tmp, len(tmp))
print(tmp)





#
# 3.Sequences序列通用操作
#
'''
所有Sequences序列的通用的操作:
	下标索引(indexing) [ 下标从0 开始!! ]
	下标分片(sliceing) [ 切片也可以用作区间索引,区间访问,但性能一般; 先拷贝,再索引,性能还能好到哪里去? 但也比逻辑索引强; ]
	迭代(iteration)
	运算符'+'加号重载(adding)
	运算符'*'乘号重载(multiplying)
'''

# 下标索引(indexing)
pstr='love you asshole中文'
print(pstr[0])
print(pstr[1])
print(pstr[2])
print(pstr[-1])		# 倒数第一位


# 下标切片(sliceing)
print(pstr[0:4])	# 第0-4个字符
print(pstr[5:8])	# 第5-8个字符
print(pstr[4:])		# 截取第4个字符, 到字符串结束为止! 
print(pstr[-5:])	# 截取倒数第5位, 到字符串结束为止!
print(pstr[-1:])	# 截取最后一个字符

xid = str(random.randint(0, 9999))
print(xid[0:4])

xid = str(random.randint(0, 999999999999))
print(xid[0:12])



# 迭代(iteration)
# 直接对array 进行迭代, 性能最好
for x in pstr:
	print(x)

# 先将array 强转为list, 再迭代, 没必要!(废弃, 语法错误, 禁止这样用)
'''
报错: TypeError: 'str' object cannot be interpreted as an integer
for x in range(pstr):
	print(x)
'''

# 迭代找子字符串(废弃, 没必要, 直接用str.find()/str.index() 更好) [array数组也有array.index()]
z="China is a great country中文"
if "ina" in z:
	print("\"ina\" in z: ", z)

print(z.find("ina"))

if z.find("ina"):
	print("str.find(\"ina\") == True: ", z)

if z.index("ina"):
	print("str.index(\"ina\") == True: ", z)



# 运算符'+'加号重载(adding)
x="Hello中文"
y="World"
z=x+" "+y
print(z)
print(x+" "+y+z+"中文")


# 运算符'*'乘号重载(multiplying)
x="Hello中文 "
y="World"
z=x*3
print(z)
# 不能是两个字符串相乘, 两个array 相乘, 没意义; 要向量相乘, 请用numpy
#print(x*y)








#
# x.常用的字符串API
#
pstr='   love you, asshole!! you are loveing love, love you again      中文  '

# 数字, 其他类型, 强转为字符串
print(str(1111111111199999999))

# 获取字符串长度
print(len(pstr))

# 自动删除字符串的前后多余空格(中间的空格没办法删除)
print(pstr.strip())

# 将整串字符串转换为大写
print(pstr.upper())

# 将整串字符串转换为小写
print(pstr.lower())

# 替换目标子字符串(找到则替换, 找不到不操作)
print(pstr.replace("love", "kill"))
#print(pstr.replace("xlove", "kill"))

# 按照指定分隔符, 将字符串切割(遇到一个分割符号, 切割一次); 切割后, 返回一个list
print(pstr.split(","))

# 查找子字符串
print(pstr.find("kill"))	# 找不到返回-1(-1也表示, 找到字符串最后一位也找不到)
print(pstr.index("love")) # 找到返回起始下标, 找不到会报错中断
#print(pstr.index("kill"))



# 强转为ascii() [python3 专属]
#ascii(1)
#ascii('&')
#ascii(9000000)
# 中文用的比较多, 如果项目不支持UTF-8, 可能会需要用到ascii() 转换
#ascii('中文')





#
# 字符串API 总表
#
'''
capitalize()	把首字符转换为大写
casefold()		把字符串转换为小写
center()			返回居中的字符串
count()				返回指定值在字符串中出现的次数
encode()			返回字符串的编码版本
endswith()		如果字符串以指定值结尾, 则返回True
expandtabs()	设置字符串的 tab 尺寸
find()				在字符串中搜索指定的值并返回它被找到的位置
format()			格式化字符串中的指定值
format_map()	格式化字符串中的指定值
index()				在字符串中搜索指定的值并返回它被找到的位置
isalnum()			如果字符串中的所有字符都是字母数字, 则返回True
isalpha()			如果字符串中的所有字符都在字母表中, 则返回True
isdecimal()		如果字符串中的所有字符都是小数, 则返回True
isdigit()			如果字符串中的所有字符都是数字, 则返回True
isidentifier()如果字符串是标识符, 则返回True
islower()			如果字符串中的所有字符都是小写, 则返回True
isnumeric()		如果字符串中的所有字符都是数, 则返回True
isprintable()	如果字符串中的所有字符都是可打印的, 则返回True
isspace()			如果字符串中的所有字符都是空白字符, 则返回True
istitle()			如果字符串遵循标题规则, 则返回True
isupper()			如果字符串中的所有字符都是大写, 则返回True
join()				把可迭代对象的元素连接到字符串的末尾
ljust()				返回字符串的左对齐版本
lower()				把字符串转换为小写
lstrip()			返回字符串的左修剪版本
maketrans()		返回在转换中使用的转换表
partition()		返回元组, 其中的字符串被分为三部分
replace()			返回字符串, 其中指定的值被替换为指定的值
rfind()				在字符串中搜索指定的值, 并返回它被找到的最后位置
rindex()			在字符串中搜索指定的值, 并返回它被找到的最后位置
rjust()				返回字符串的右对齐版本
rpartition()	返回元组, 其中字符串分为三部分
rsplit()			在指定的分隔符处拆分字符串, 并返回列表
rstrip()			返回字符串的右边修剪版本
split()				在指定的分隔符处拆分字符串, 并返回列表
splitlines()	在换行符处拆分字符串并返回列表
startswith()	如果以指定值开头的字符串, 则返回True
strip()				返回字符串的剪裁版本
swapcase()		切换大小写, 小写成为大写, 反之亦然
title()				把每个单词的首字符转换为大写
translate()		返回被转换的字符串
upper()				把字符串转换为大写
zfill()				在字符串的开头填充指定数量的0 值
'''


