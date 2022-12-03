#coding=utf-8



import sys
import imp
imp.reload(sys)
if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
	sys.setdefaultencoding("utf-8")



#
# 1.定义字符串变量(尽量不要命名为str, 因为str 是内置字符串库的名)
#

# 单行
pstr1='fuck you bitch中文'
pstr2="fuck you bitch中文"

# 多行(ps: 注释字符串会产生局部/全局字符串数据)
pstr3='''
fuck you 
bitch
中文
'''
pstr4="""
fuck you 
bitch
中文
"""

print(pstr1)
print(pstr2)
print(pstr3)
print(pstr4)





#
# 2.python 的字符串本质: 表示一串unicode字符的字节数组, 即字符串实际是用数组存储的.
#

# 注意, python 字符串数组的起点是[0]位
pstr='fuck you bitch中文'
print(pstr[0])
print(pstr[1])
print(pstr[2])
print(pstr[3])

# python 字符串, 根据数组位进行剪裁
print(pstr[0:4])
print(pstr[5:8])
# 截取第4个字符, 到字符串结束为止! 
print(pstr[4:])
# 截取倒数第5位, 到字符串结束为止! 截取时, 没办法表示最后一位字符, 那就不表示.
print(pstr[-5:])
# 截取最后一个字符(区间截取法)
print(pstr[-1:])
# 截取最后一个字符(直接索引法)
print(pstr[-1])





#
# 3.字符串串联
#
a="Hello中文"
b="World"

c=a+b
print(c)

c=a+" "+b
print(c)





#
# 4.字符串变量直接格式化(主要用来插入number, list 等非字符串数据)
#

# 默认顺序(推荐)
quantity=3
itemno=567
price=49.95
myorder="I want {} pieces of item {} for {} dollars.中文"
print(myorder.format(quantity, itemno, price))

# 强制修改变量的排序顺序
myorder="I want to pay {2} dollars for {0} pieces of item {1}.中文"
print(myorder.format(quantity, itemno, price))





#
# 5.字符串检索
#

# 检查以下文本中是否存在短语 "ina"
txt="China is a great country中文"
x="ina" in txt
print(x)

# 检查以下文本中是否没有短语 "ina"
txt="China is a great country中文"
x="ain" not in txt
print(x) 





#
# x.常用的字符串API
#
pstr=' fuck you, bitch!! you are fucking fuck, fuck you again          中文'

# 获取字符串长度
print("字符串: %s 的长度是: %d" %(pstr, len(pstr)))

# 自动删除字符串的前后多余空格
print(pstr.strip())

# 将整串字符串转换为大写
print(pstr.upper())

# 将整串字符串转换为小写
print(pstr.lower())

# 替换目标子字符串
print(pstr.replace("fuck", "kill"))

# 指定分隔符, 将字符串切割为n 部分, 有多少个分割符号, 就切割多少次.
# 切割之后, 可以用list 将切割后的'子字符串'装起来.
print(pstr.split(","))





#
# x1.强转为ascii() [python3 专属]
#
#ascii(1)
#ascii('&')
#ascii(9000000)
# 中文用的比较多, 如果项目不支持utf8, 可能会需要用到ascii() 转换
#ascii('中文')





#
# x2.format(val, type), 对数据进行格式化处理
#
# 字符串可以提供的参数 's' None
format('some string','s');	# 'some string'
format('some string')			# 'some string'

# 整形数值可以提供的参数有 'b' 'c' 'd' 'o' 'x' 'X' 'n' None
format(3,'b')	# 转换成二进制 '11'
format(97,'c')# 转换unicode成字符 'a'
format(11,'d')# 转换成10进制 '11'
format(11,'o')# 转换成8进制 '13'
format(11,'x')# 转换成16进制 小写字母表示 'b'
format(11,'X')# 转换成16进制 大写字母表示 'B'
format(11,'n')# 和d一样 '11'
format(11)		# 默认和d一样 '11'

# 浮点数可以提供的参数有 'e' 'E' 'f' 'F' 'g' 'G' 'n' '%' None
format(314159267,'e')					# 科学计数法, 默认保留6位小数 '3.141593e+08'
format(314159267,'0.2e')			# 科学计数法, 指定保留2位小数 '3.14e+08'
format(314159267,'0.2E')			# 科学计数法, 指定保留2位小数, 采用大写E表示 '3.14E+08'
format(314159267,'f')					# 小数点计数法, 默认保留6位小数 '314159267.000000'
format(3.14159267000,'f')			# 小数点计数法, 默认保留6位小数 '3.141593'
format(3.14159267000,'0.8f')	# 小数点计数法, 指定保留8位小数 '3.14159267'
format(3.14159267000,'0.10f')	# 小数点计数法, 指定保留10位小数 '3.1415926700'
format(3.14e+1000000,'F')			# 小数点计数法, 无穷大转换成大小字母 'INF'

# g的格式化比较特殊, 假设p为格式中指定的保留小数位数, 先尝试采用科学计数法格式化, 得到幂指数exp, 如果-4<=exp<p, 则采用小数计数法, 并保留p-1-exp位小数, 否则按小数计数法计数, 并按p-1保留小数位数
format(0.00003141566,'.1g')	# p=1,exp=-5 ==> -4<=exp<p不成立, 按科学计数法计数, 保留0位小数点 '3e-05'
format(0.00003141566,'.2g')	# p=1,exp=-5 ==> -4<=exp<p不成立, 按科学计数法计数, 保留1位小数点 '3.1e-05'
format(0.00003141566,'.3g')	# p=1,exp=-5 ==> -4<=exp<p不成立, 按科学计数法计数, 保留2位小数点 '3.14e-05'
format(0.00003141566,'.3G')	# p=1,exp=-5 ==> -4<=exp<p不成立, 按科学计数法计数, 保留0位小数点, E使用大写 '3.14E-05'
format(3.1415926777,'.1g')	# p=1,exp=0 ==> -4<=exp<p成立, 按小数计数法计数, 保留0位小数点 '3'
format(3.1415926777,'.2g')	# p=1,exp=0 ==> -4<=exp<p成立, 按小数计数法计数, 保留1位小数点 '3.1'
format(3.1415926777,'.3g')	# p=1,exp=0 ==> -4<=exp<p成立, 按小数计数法计数, 保留2位小数点 '3.14'
format(0.00003141566,'.1n')	# 和g相同 '3e-05'
format(0.00003141566,'.3n')	# 和g相同 '3.14e-05'
format(0.00003141566)				# 和g相同 '3.141566e-05'



# 格式化: 引用字符串变量
print("\"网站名:{name}, 地址 {url}\".format(name=\"菜鸟教程\", url=\"www.runoob.com\")")

# 格式化: 自动根据dist 的key, 匹配变量值(通配)
site = {"name": "菜鸟教程", "url": "www.runoob.com"}
print("\"网站名:{name}, 地址 {url}\".format(**site)")

# 格式化: 根据列表索引设置参数
myList = ['菜鸟教程', 'www.runoob.com']
print("\"网站名:{0[0]}, 地址 {0[1]}\".format(myList)")








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
isspace()		如果字符串中的所有字符都是空白字符, 则返回True
istitle()		如果字符串遵循标题规则, 则返回True
isupper()		如果字符串中的所有字符都是大写, 则返回True
join()			把可迭代对象的元素连接到字符串的末尾
ljust()			返回字符串的左对齐版本
lower()			把字符串转换为小写
lstrip()		返回字符串的左修剪版本
maketrans()	返回在转换中使用的转换表
partition()	返回元组, 其中的字符串被分为三部分
replace()		返回字符串, 其中指定的值被替换为指定的值
rfind()			在字符串中搜索指定的值, 并返回它被找到的最后位置
rindex()		在字符串中搜索指定的值, 并返回它被找到的最后位置
rjust()			返回字符串的右对齐版本
rpartition()返回元组, 其中字符串分为三部分
rsplit()		在指定的分隔符处拆分字符串, 并返回列表
rstrip()		返回字符串的右边修剪版本
split()			在指定的分隔符处拆分字符串, 并返回列表
splitlines()在换行符处拆分字符串并返回列表
startswith()如果以指定值开头的字符串, 则返回True
strip()			返回字符串的剪裁版本
swapcase()	切换大小写, 小写成为大写, 反之亦然
title()			把每个单词的首字符转换为大写
translate()	返回被转换的字符串
upper()			把字符串转换为大写
zfill()			在字符串的开头填充指定数量的 0 值
'''
