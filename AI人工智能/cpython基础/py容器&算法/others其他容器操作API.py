#coding=utf8



# 1.检查容器中的所有元素的有效性
'''
	一般可用来检查: 
		list, dict, tuple, set;

	all() 检查原则:
		空容器, 返回False;
		容器非空, 则: 
			如果存在false, none, 0, ""(空字符串) 等元素, 则返回False;
			否则返回True;
'''

pdict={ "M":{"b":90909, "a":6666},"A":191, "B":"love0.0", "C":999, "D":"you^_^" }
plist=[1,2,3,4,5];
plist2=[1,2,3,4,5,0];

print(all(pdict))
print(all(plist))
print(all([]))
print(all(plist2))





# 2.检查容器是否有一个以上的有效元素
print(any([0,1,2]))	# True -- 列表元素有一个为True, 则返回True
print(any([0,0]))		# False -- 列表元素全部为False, 则返回False
print(any([]))			# False -- 空列表
print(any({}))			# False -- 空字典
print(any({"":0}))	# False -- 字典只有一串空字符串, 则返回False





# 3.检查一个变量, 是否可以unhashable(成为hash 容器dict 的key)
print(hash('test'))  # 字符串
print(hash(1))       # 数字
#print(hash([1,2,3])) # list不行
#print(hash({'1':1})) # dict不行





#
# 下面的操作, 没有啥参考意义, 一般不会这么用!! 也可以找到更好的替代方法, 基本不会用的, 了解一下即可;
#





# 4.filter()容器过滤函数[返回True 保留, False 删除, 总是返回一个list, dict不能用(性能消耗大, 比手动if 剔除, 好不了多少)]
# filter()过滤条件函数
def filter_func(n):
	return n % 2 == 1;

oldlist = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
newlist = filter(filter_func, oldlist)
print(oldlist)
print(newlist)

# 总是返回一个list, dict不能用(弃用, 傻逼用法)
'''
olddict={ 1:{"b":90909, "a":6666},2:191, 3:"love0.0", 4:999, 5:"you^_^" }
newdict = filter(filter_func, olddict)
print(olddict)
print(newdict)
'''



# 5.map()使用指定方法去作用传入的每个可迭代对象的元素, 类似filter, 返回生成新的可迭代对象(对象无类型);(弃用, 啥玩儿?)
a = map(ord,'abcd')		# ord 是内建函数, map 也是传入一个操作函数, 操作对象内的所有元素的
print(a)							# <map object at 0x03994E50>
print(list(a))				# [97, 98, 99, 100], 将对象类型化为列表



# 6.zip() 合并两个容器(聚合操作), 总是返回一个list, dict不能用;
'''
	取最小长度的list进行合并, 其他多出来的部分丢弃, 返回一个list
'''
x = [1,2,3]
y = [4,5,6,7,8]
# [(1, 4), (2, 5), (3, 6)]
z = zip(x,y)
print(z)

# 总是返回一个list, dict不能用(弃用, 傻逼用法)
'''
x = [1,2,3]
pdict={ "M":{"b":90909, "a":6666},"A":191, "B":"love0.0", "C":999, "D":"you^_^" }
pp=zip(x,pdict)
print(pp)
'''





# 7.slice 切片对象(Flat_Sequences平面级序列通用功能, 一般不用slice() 关键字)
print(slice(5))
print(slice(2,5))
print(slice(1,10,3))


















