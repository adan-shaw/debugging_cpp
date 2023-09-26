#coding=utf8



import random
import string



# 一个随机整数:
print(random.randint(1,50))

# 一个随机选取0到100间的偶数(从0开始, 101结束, 步进为2):
print(random.randrange(0, 101, 2))

# 一个随机浮点数:
print(random.random())
print(random.uniform(1, 10))

# 按bit 位数来确定随机数的范围. 1bit=[0,1], 2bit=[0,3], 3bit=[0,7]
print(random.getrandbits(2))





# 从字符串中, 取一个随机字符:
print(random.choice('abcdefghijklmnopqrstuvwxyz!@#$%^&*()'))

# 从字符串中, 取N个字符, 生成list列表
print(random.sample('zyxwvutsrqponmlkjihgfedcba',5))





# 从a-z, A-Z, 0-9 中, 随机生成N个字符(需要用到'字符串实体变量'中的join() 方法):
print(''.join(random.sample(string.ascii_letters + string.digits, 8)))

# 从多个字符串的list中, 随机选取N个字符串, 组成新字符串:
print(''.join(random.sample(['j1','i222','h3','g','f55','e6','d','c','b','a'], 5)))

# 从多个字符串的list中, 随机选取1个字符串:
print(random.choice(['剪刀', '石头', '布']))





# 打乱list 的排序
num_list=[1, 2, 3, 4, 5, 6, 7, 8, 9, 0]
random.shuffle(num_list)
print(num_list)
# 错误用法:
# 	random.shuffle() 一定返回None, 所以不能承接random.shuffle() 的返回值
#num_list=random.shuffle(num_list)

str_list = ['j1','i222','h3','g','f55','e6','d','c','b','a']
random.shuffle(str_list)
print(str_list)





# 带权值, 返回随机数(参数: -100.1 , 100 , -30, 表示随机数会更多的抽中[-30,-100.1]区间的数)
print(random.triangular(-100.1,100,-30))






#
# python random 模块api
#
'''
seed()				初始化随机数生成器
getstate()		返回随机数生成器的当前内部状态
setstate()		恢复随机数生成器的内部状态

# 随机整形数
getrandbits()	返回表示随机位的数字
randrange()		返回给定范围之间的随机数
randint()			返回给定范围之间的随机数

# 随机序列-列表
choice()			返回给定序列中的随机元素
choices()			返回一个列表, 其中包含给定序列中的随机选择
shuffle()			接受一个序列, 并以随机顺序返回此序列
sample()			返回序列的给定样本

# 随机浮点数
random()			返回0与1之间的浮点数
uniform()			返回两个给定参数之间的随机浮点数
triangular()	返回两个给定参数之间的随机浮点数, 
							您还可以设置模式参数以指定其他两个参数之间的中点

# 不常用的
betavariate()			基于 Beta 分布(用于统计学)返回0到1之间的随机浮点数
expovariate()			基于指数分布(用于统计学), 返回0到1之间的随机浮点数, 
									如果参数为负, 则返回0到-1之间的随机浮点数
gammavariate()		基于 Gamma 分布(用于统计学)返回0到1之间的随机浮点数
gauss()						基于高斯分布(用于概率论)返回0到1之间的随机浮点数
lognormvariate()	基于对数正态分布(用于概率论)返回0到1之间的随机浮点数
normalvariate()		基于正态分布(用于概率论)返回0到1之间的随机浮点数
vonmisesvariate()	基于 von Mises 分布(用于定向统计学)返回0到1之间的随机浮点数
paretovariate()		基于 Pareto 分布(用于概率论)返回0到1之间的随机浮点数
weibullvariate()	基于 Weibull 分布(用于统计学)返回0到1之间的随机浮点数
'''





