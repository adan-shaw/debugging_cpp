#coding=utf8



# format(val, type), 可对多个变量进行格式化处理, 格式化成统一格式的数据;
'''
	int(), str(), float() 等操作, 只能将一个变量格式化为一种新类型;
	format() 可以将多个变量格式化为一种新类型;

	format() 一般只用在字符串格式化拼凑, 
	数字变量, 可直接int(), float() 强转, python 会在适当的场合, 自动使用'科学记数法';
'''

# 整形数格式化, 可以提供的参数有 'b' 'c' 'd' 'o' 'x' 'X' 'n' None
print(format(3,'b'))	# 转换成二进制 '11'
print(format(97,'c'))	# 转换unicode成字符 'a'
print(format(11,'d'))	# 转换成10进制 '11'
print(format(11,'o'))	# 转换成8进制 '13'
print(format(11,'x'))	# 转换成16进制 小写字母表示 'b'
print(format(11,'X'))	# 转换成16进制 大写字母表示 'B'
print(format(11,'n'))	# 和d一样 '11'
print(format(11))			# 默认和d一样 '11'

# 字符串格式化
print(format('some string'))				# 默认格式化为字符串, 省缺参数, 即None
print(format('some string','s'))		# 强制指定格式化为字符串
print('some string'.format())				# 字符串格式化的另外一种写法





# 数字变量定义为科学记数法: 
'''
	底数10, 后面是次方数, 此方数可为正负(正=超大数, 负等于超小数)
	(适用于超大浮点, 超小浮点, 超大整数)
'''
x = 1e+3
x1 = 1e+03
x2 = 1e+003
x3 = 1E+3
x4 = 1E-3
print(x)		# 1000.0
print(x1)		# 1000.0
print(x2)		# 1000.0
print(x3)		# 1000.0
print(x4)		# 0.001





# 数字格式化: 强制使用'小数点记数法'
print(format(314159267,'f'))					# 小数点记数法, 默认保留6位小数 '314159267.000000'
print(format(3.14159267000,'f'))			# 小数点记数法, 默认保留6位小数 '3.141593'
print(format(3.14159267000,'0.8f'))		# 小数点记数法, 指定保留8位小数 '3.14159267'
print(format(3.14159267000,'0.10f'))	# 小数点记数法, 指定保留10位小数 '3.1415926700'
print(format(3.14e+1000000,'F'))			# 小数点记数法, 无穷大时, 则会转换成大小字母 'INF'





# format() 等同于float() 强转;
print(float(9415922677199415926777.9415926777))
print(format(9415922677199415926777.9415926777))

# 数字格式化: 强制使用'科学记数法' (格式化肯定会自动四舍五入, 但不一定会自动使用'科学记数法'):
'''
	科学记数法: 
		底数10, 后面是次方数, 此方数可为正负(正=超大数, 负等于超小数)
		普通数根本没有用'科学记数法'的必要, 如果数值太长, 建议直接'四舍五入'更好!!


	float 浮点数:
		超小浮点:
			小于0.0000**** 的数(小数点后4个0), 默认采用'科学记数法', 例如: 负数e-1, e-2, e-3 等;
			(超小浮点, 整数位已经不重要, 不影响数值结果了)

		普通浮点:
			大于0.0000**** 的数, 无论数值多长, 默认都不会格式化为'科学记数法';
			* 可强制使用'科学记数法', 但没啥意义;
				'科学记数法'只会是正数e+1, e+2, e+3 等, 完全不能体现出小数的作用, 
				一般这种场合, 不适合使用'科学记数法'!!
			(默认只保留11 位数, 正整数位越多, 小数位越小, 会被自动四舍五入, 后续直接丢弃)

		超大浮点:
			大于11 位数的浮点, 默认采用'科学记数法', 例如: 正数e+1, e+2, e+3 等;
			(超大浮点, 小数位已经不重要, 无论小数部分有多少个位, 直接丢弃, 不影响数值结果了)


	int 整型数:
		无论多长, 都不使用'科学记数法', 除非你显式将整形数, 格式化为'科学记数法'


	总结:
		'科学记数法'只适用于:
			* 超小浮点: 负数e-n, n是位数
			* 超大浮点: 正数e+n, n是位数
			* 超大整形: 正数e+n, n是位数

	大小写无区别:
		g = e
		G = E
'''

print("超小浮点(自动使用科学记数法):")
print(format(0.000031))							# 小数点后有4个以上的0, 自动使用科学记数法, 省缺保留1位小数点(四舍五入)
print(format(0.00003141566,'.1g'))	# 小数点后有4个以上的0, format为科学记数法, 保留0位小数点 '3e-05'(四舍五入)
print(format(0.00003141566,'.2g'))	# 小数点后有4个以上的0, format为科学记数法, 保留1位小数点 '3.1e-05'(四舍五入)(省缺值)
print(format(0.00003141566,'.3g'))	# 小数点后有4个以上的0, format为科学记数法, 保留2位小数点 '3.14e-05'(四舍五入)
print(format(0.00003141566,'.4G'))	# 小数点后有4个以上的0, format为科学记数法, 保留3位小数点, E使用大写 '3.142E-05'(四舍五入)

print(format(0.00003141566,'.1n'))	# 等同'.1g'
print(format(0.00003141566,'.3n'))	# 等同'.3g'


print("普通浮点:")
print(format(0.00011))								# 小数点后只有3个以上的0, 都不会用科学记数法(四舍五入)
print(format(1.00009141566,'.5g'))		# 小数点后有4个以上的0, 但整数部分大于0, 则不会用科学记数法 '1.0001'(四舍五入)
print(format(3.1415926777,'.1g'))			# 不保留小数位(四舍五入)
print(format(3.1415926777,'.2g'))			# 保留1数位(四舍五入)
print(format(3.941592267719222211))		# 保留11数位(四舍五入)
print(format(1415.941592267719222211))# 保留11数位(四舍五入)
print("普通浮点: 乱用科学记数法, 无任何意义:")
print(format(1415.941592267719222211,'.2g')) # 强制使用'科学记数法', 无任何实际数学意义, 简直混绕视听, 傻逼做法, 虽然编程上可行;
print(format(145.9,'.2g'))


print("超大浮点:")
print(format(9415922677199415926777.9415926777))	# 默认保留11数位 '9.4159226772e+21'
print(format(1415922677126777.1415926777,'.3g'))	# 保留2数位 '1.42e+15'



print("整数:")
print(format(1,'g'))							# 这样做无任何实际意义!!
print(format(3267))
print(format(3141592670000000111111))




