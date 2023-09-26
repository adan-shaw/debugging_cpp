#coding=utf8



'''
Python 在以下组中划分运算符:
	*1.算术运算符
	*2.赋值运算符
	*3.比较运算符
	*4.逻辑运算符
	*5.位运算符

	*6.身份运算符
	*7.成员运算符
'''



#
# 1.python 算术运算符
#
x=1
y=1

# + - * / 加
tmp=(x+y+1);print(tmp);
tmp=(x-y-5);print(tmp);
tmp=(x*y*3);print(tmp);
# 注意: 除以一个整数, 小数部分会被丢弃
tmp=(x/y/4);print(tmp);
tmp=(x/y/4.0);print(tmp);


# 求余
tmp=(x%y);print(tmp);
tmp=(x%2);print(tmp);


# 求幂(指数x^y)
tmp=(2**10);print(tmp);
tmp=(2**2);print(tmp);


# 地板除(取整数,舍弃小数部分)
tmp=(99//2);print(tmp);
tmp=(1//2);print(tmp);





#
# 2.python 赋值运算符(注意: 赋值运算符不能加'()' 小括号!! 其他运算符可以)
#
x=5;
x+=3;		 # 等价于 x=x+3
x-=3;		 # 等价于 x=x-3
x*=3;		 # 等价于 x=x*3
x/=3;		 # 等价于 x=x/3
x%=3;		 # 等价于 x=x%3
x//=3;	 # 等价于 x=x//3
x**=3;	 # 等价于 x=x**3

# python3 不兼容
#x&=3;		 # 等价于 x=x&3
#x|=3;		 # 等价于 x=x|3
#x^=3;		 # 等价于 x=x^3
#x>>=3;	 # 等价于 x=x>>3
#x<<=3;	 # 等价于 x=x<<3





#
# 3.python 比较运算符
#
(x==y);		# 等于
(x!=y);		# 不等于
(x>y);		# 大于
(x<y);		# 小于
(x>=y);		# 大于&&等于
(x<=y);		# 小于&&等于


















