#coding=UTF-8



# format(val, type), 可对多个变量进行格式化处理, 格式化成统一格式的数据;
'''
	int(), str(), float() 等操作, 只能将一个变量格式化为一种新类型;
	format() 可以将多个变量格式化为一种新类型;

	format() 一般只用在字符串格式化拼凑, 
	数字变量, 可直接int(), float() 强转, python 会在适当的场合, 自动使用'科学记数法';
'''

x=1
y=9.9
z="fffff"


print("字符串格式化")
# 字符串格式化
print(format('some string'))				# 默认格式化为字符串, 省缺参数, 即None
print(format('some string','s'))		# 强制指定格式化为字符串
print('some string'.format())				# 字符串格式化的另外一种写法

# 字符串平接, 类似printf()
print('some string: {}, {}, {} 中文'.format(x,y,z))

# 字符串平接, 类似printf(), 调乱顺序
print('some string: {2}, {0}, {1} 中文'.format(x,y,z))




