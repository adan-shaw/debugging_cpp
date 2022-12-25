#coding=utf-8



# dict 排序声明:
'''
	由于sorted() 总是返回一个list, 
	因此, 对dict 进行排序, 会比想想中复杂!! 

	首先sorted() 后会自动转为list, 
	然后又由list 转为dict, 消耗十分大!!

	因此, 用dict 尽量不排序, 需要排序, 宁愿用list, 再复杂也用list;
	list 可以直接拍开, 变成一个表, 十分好操作;
'''





#
# 1.只对key/value 进行排序, 得出一个全是key/value 的list(弃用, 只做说明)
#
# 根据key排序
a = {'A': 120, 'T': 200, 'dd': 50, 'uu': 80}
print(sorted(a))        # dict默认操作
print(sorted(a.keys())) # 等价于dict默认操作


# 根据value排序
print(sorted(a.values()))





#
# 2.对'键值对'进行排序, 得出一个'键值对'混合的list
#
# 根据key排序
a = {'A': 120, 'T': 200, 'dd': 50, 'uu': 80}
print(sorted(a.items(), key=lambda x: x[0]))


# 根据value排序
print(sorted(a.items(), key=lambda x: x[1]))


