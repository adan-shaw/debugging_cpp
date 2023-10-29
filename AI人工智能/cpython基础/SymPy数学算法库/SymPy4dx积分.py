#coding=UTF-8

# 定积分, 就是求线性函数的面积;



# 使用python-sympy 库来计算定积分(非常专业的数学库, 不一定会单纯输出double 浮点数值, 有时候会输出数学公式, 很专业)
# 安装:
# 	apt-get install python-sympy python3-sympy



# 坑:
# 	expr = 数学表达式(需要先转换, 才能用; sympify() 是转换函数)
# 	exp = e 为底的自然数



from sympy import *



# 定义一个变量符号'x'(数学公式中有多少个未知元, 这里就有多少个)
x = symbols('x')



# 1.计算f sin(x) dx [-1,3] 的定积分(输出数学公式结果)
x = symbols('x')
y = integrate(sin(x), (x, -1, 3))
print("f sin(x) dx [-1,3] = ", y)

# 计算f sin(x) dx [0,pi] 的定积分(输出数值结果)
y = integrate(sin(x), (x, 0, pi))
print("f sin(x) dx [0,pi] = ", y)



# 2.计算f 2x+1 dx [0,2] 的定积分(输出数值结果)
x = symbols('x')

# 将数学表达式字符串, 转换为sympy 专用的表达式
expr_s = "2*x+1"
expr = sympify(expr_s)

y = integrate(expr, (x, 0, 2))
print("f 2x+1 dx [0,2] = ", y)



# 3.创建二元方程
x, y = symbols('x y')

# 三角形面积公式的积分(虽然不知道有什么用? 求以三角形为底的体积? 上限,下限就是高h)
expr_s = "x*y/2"
expr = sympify(expr_s)

z = integrate(expr, (x, 0, 5), (y, 0, 5))
print("f x*y/2 dx [0,5] = ", z)

