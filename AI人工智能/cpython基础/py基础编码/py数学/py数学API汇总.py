#coding=UTF-8



# 0 python 执行数学表达式:
'''
	eval()表达式函数是一个安全隐患, 绝对不能让用户自己输入表达式, 
	否则会有被注入shell/python 命令的可能, 
	服务端自己确定的输入式, 可以
'''
x=8
print(eval('3 * x + 2 - 1 * 5'))
print(eval('pow(2,2) * 2 / 4 * 9'))
print(eval("x + 4"))





# 1 python number 转换:
'''
int(x [,base ])         将x转换为一个整数
long(x [,base ])        将x转换为一个长整数
float(x)                将x转换到一个浮点数
complex(real [,imag ])  创建一个复数
str(x)                  将对象 x 转换为字符串
repr(x)                 将对象 x 转换为表达式字符串
eval(str)               用来计算在字符串中的有效Python表达式,并返回一个对象
tuple(s)                将序列s 转换为一个元组
list(s)                 将序列s 转换为一个列表
chr(x)                  将一个整数转换为一个字符
unichr(x)               将一个整数转换为Unicode字符
ord(x)                  将一个字符转换为它的整数值
hex(x)                  将一个整数转换为一个十六进制字符串
oct(x)                  将一个整数转换为一个八进制字符串
'''





# 2 python 数学常量
'''
pi 数学常量pi(圆周率, 一般以π来表示)
e 数学常量e, e即自然常数(自然常数)
'''
import math;
print(math.pi)
print(math.e)





# 3. python 基础数学函数
'''
abs(x)             返回数字的绝对值, 如abs(-10), 返回10
ceil(x)            返回数字的上入整数, 如math.ceil(4.1), 返回5
cmp(x, y)          如果x < y, 则返回-1; 如果x == y, 则返回0; 如果x > y, 则返回1
exp(x)             返回e的x次幂(ex), 如: math.exp(1), 返回2.718281828459045
fabs(x)            返回数字的绝对值, 如: math.fabs(-10), 返回10.0
floor(x)           返回数字的下舍整数, 如: math.floor(4.9), 返回4
log(x)             如: math.log(math.e), 返回1.0; math.log(100,10)返回2.0
log10(x)           返回以10为基数的x的对数, 如: math.log10(100), 返回2.0
max(x1, x2,...)    返回给定参数的最大值
min(x1, x2,...)    返回给定参数的最小值
modf(x)            返回x的整数部分与小数部分, 两部分的数值符号与x相同,整数部分以浮点型表示
pow(x, y)          x**y 运算后的值
round(x [,n])      返回浮点数x的四舍五入值, 如给出n值, 则代表舍入到小数点后的位数
sqrt(x)            返回数字x的平方根
'''





# 3. python随机数函数
'''
choice(seq)        从序列的元素中随机挑选一个元素, 比如random.choice(range(10)), 从0到9中随机挑选一个整数
shuffle(lst)       将序列的所有元素随机排序
uniform(x, y)      随机生成下一个实数, 它在[x,y]范围内
random()           随机生成下一个实数, 它在[0,1)范围内
seed([x])          改变随机数生成器的种子seed(如果你不了解其原理, 你不必特别去设定seed, Python会帮你选择seed)
randrange ([start,] stop [,step])    从指定范围内, 按指定基数递增的集合中获取一个随机数, 基数缺省值为1
'''





# 4. python三角函数
'''
acos(x)            返回x的反余弦弧度值
asin(x)            返回x的反正弦弧度值
atan(x)            返回x的反正切弧度值
atan2(y, x)        返回给定的X及Y坐标值的反正切值
cos(x)             返回x的弧度的余弦值
hypot(x, y)        返回欧几里德范数sqrt(x*x + y*y)
sin(x)             返回的x弧度的正弦值
tan(x)             返回x弧度的正切值
degrees(x)         将弧度转换为角度,如degrees(math.pi/2), 返回90.0
radians(x)         将角度转换为弧度
'''




