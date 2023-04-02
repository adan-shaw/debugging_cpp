#coding=utf-8



import sys

if(sys.version_info.major == 3):
	if(sys.version_info.minor <= 3):
		import imp
		imp.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("utf-8")
	else:
		import importlib
		importlib.reload(sys)
		if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
			sys.setdefaultencoding("utf-8")

if(sys.version_info.major == 2):
	import imp
	imp.reload(sys)
	if(sys.getdefaultencoding() != sys.getfilesystemencoding()):
		sys.setdefaultencoding("utf-8")



#
# 1.input() 读取用户输入值
#
'''
	input()相等于eval(raw_input(strings)), 需要输入数学表达式才行!!
	input()在python2 中, 类似eval(raw_input(strings));
	input()在python3 中, 类似sys.stdin.readline();
	所以, 录入数据, 尽量用: sys.stdin.readline;
	需要转为表达式, 直接使用: eval() + string字符串;
'''
eval_results = input('please input mathematical expression:\n')
print(eval_results)

# python3 中已被废弃, 在python2 中, 与sys.stdin.readline() 功能一样;
#s = raw_input('please input strings:\n')
#print(s)





#
# 2.标准io
#
# 初始化标准输出API(提高引用效率)
stdout=sys.stdout.write
stdin=sys.stdin.readline
stderr=sys.stderr.write



# 标准输出API的使用
stdout("hello world\n")
stderr("hello world\n")

stdout("请输入一行(readline):\n")
tmp=stdin()
stdout(tmp)





#
# 3.print格式化打印输出
#
# 格式化打印输出
str_tmp='love you bitch'
print("字符串: %s 的长度是: %d" %(str_tmp, len(str_tmp)))

# 不允许多个 %() 格式化参数列表(一般只有一个)
#print("字符串: %s 的长度是: %d" %(str_tmp) %(len(str_tmp)))


# 打印不换行, 只附加一个空格(结尾加逗号)
print("'love',")
print("'you!!'")


# 格式化: 引用字符串变量
print("\"网站名:{name}, 地址 {url}\".format(name=\"菜鸟教程\", url=\"www.runoob.com\")")


# 格式化: 自动根据dist 的key, 匹配变量值(通配)
site = {"name": "菜鸟教程", "url": "www.runoob.com"}
print("\"网站名:{name}, 地址 {url}\".format(**site)")


# 格式化: 根据列表索引设置参数
myList = ['菜鸟教程', 'www.runoob.com']
print("\"网站名:{0[0]}, 地址 {0[1]}\".format(myList)")



