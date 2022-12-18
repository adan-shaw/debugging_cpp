#coding=utf-8



# 'set集合'简介
'''
	'set集合'不允许有重复元素, 
	重复元素会被自动吞灭, 最终只保留一个; 
	'set集合'还会自动从小到大排序, 不能修改'set集合'的顺序;

	'set集合'内的元素类型必须是: 可哈希(hashable)类型;

	'set集合'可以做交集,补集,差异集,等运算;
	'set集合'允许增删元素;
'''



# python Set API:
'''
add()									向'set集合'添加元素
clear()								删除'set集合'中的所有元素
copy()								返回'set集合'的副本
difference()					返回包含两个或更多'set集合'之间差异的'set集合'
difference_update()		删除此'set集合'中也包含在另一个指定'set集合'中的项目
discard()							删除指定项目
intersection()				返回为两个其他'set集合'的交集的'set集合'
intersection_update()	删除此'set集合'中不存在于其他指定'set集合'中的项目
isdisjoint()					返回两个'set集合'是否有交集
issubset()						返回另一个'set集合'是否包含此'set集合'
issuperset()					返回此'set集合'是否包含另一个'set集合'
pop()									从'set集合'中删除一个元素
remove()							删除指定元素
union()								返回包含'set集合'并集的'set集合'
update()							用此'set集合'和其他'set集合'的并集来更新'set集合'
symmetric_difference()				返回具有两组'set集合'的对称差集的'set集合'
symmetric_difference_update()	插入此'set集合'和另一个'set集合'的对称差集
'''



#
# 1.'set集合'的创建和拷贝
#
# 创建'set集合'
pset={9,1,2,5,7,3,2,6,23,(1,2),"idiot"}
print(pset)

# set()构造函数拷贝
pset0=set(pset)
print(pset)

# copy()拷贝('set集合'没有深拷贝)
pset0=pset.copy()
print(pset0)

# deepcopy()拷贝
import copy
pset0=copy.deepcopy(pset)
print(pset0)





#
# 2.'set集合'运算: 交集,补集,差异集, 等等
#
# 补集(保护式):
'''
	union() 融合两个'set集合', 并触发重新排序, 去除重复元素.
	调用union() 方法的'set集合', 不会有任何改变,
	补集结果, 返回到新的'set集合'中
'''
pset1 = {"a", "b" , "c"}
pset2 = {1, 2, 3}
pset3 = pset1.union(pset2)
print(pset1)
print(pset2)
print(pset3)

# 补集(破坏式): 
'''
	update() 融合两个'set集合', 并触发重新排序, 去除重复元素.
	将一个'set集合', 插入到另外一个'set集合'中; 
	被插入的'set集合', 值会有变动, 除非插入了空集.
'''
pset1 = {"a", "b" , "c"}
pset2 = {1, 2, 3}
pset1.update(pset2)
print(pset1)
print(pset2)



# 去除交集(保护式):
'''
	difference() 剔除一个'set集合'中, 与另外一个'set集合'的共有的元素, 
	将结果组成一个新'set集合'返回, 原'set集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset3=pset1.difference(pset2)
print(pset1)
print(pset2)
print(pset3)

# 去除交集(破坏式):
'''
	difference_update() 剔除一个'set集合'中, 与另外一个'set集合'的共有的元素, 
	直接在原'set集合'上删除'所有共有元素';
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset1.difference_update(pset2)
print(pset1)
print(pset2)



# 先去除交集, 得出两个差集后, 再融合这两个差集(保护式): 
'''
	symmetric_difference() 剔除两个'set集合'的共有的元素, 
	将两个'set集合'剩下所有非共有的元素,
	然后, 组成一个新的'set集合'返回, 原'set集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset3=pset1.symmetric_difference(pset2)
print(pset1)
print(pset2)
print(pset3)

# 先去除交集, 得出两个差集后, 再融合这两个差集(破坏式): 
'''
	symmetric_difference_update() 剔除两个'set集合'的共有的元素, 
	将两个'set集合'剩下所有非共有的元素,
	然后, 将这'两个'set集合'的非共有的元素', 组成一个新'set集合'返回;
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset1.symmetric_difference_update(pset2)
print(pset1)
print(pset2)



# 取得交集(保护式):
'''
	intersection() 拾取两个'set集合'中'共有的元素', 
	将结果组成一个新的'set集合'返回, 原'set集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset3=pset1.intersection(pset2)
print(pset1)
print(pset2)
print(pset3)

# 取得交集(破坏式):
'''
	intersection_update() 拾取两个'set集合'中'共有的元素', 
	直接在原'set集合'上删除'所有非共有元素';
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pset1.intersection_update(pset2)
print(pset1)
print(pset2)





#
# 3.两个'set集合'的判断操作
#
# 判断A 是否是B 的子集
A={1,2,3}
B={1,2,3,4,5,6}
if(A.issubset(B)):
	print("A: B是我爸爸")

# 判断B 是否是A 的超集
if(B.issuperset(A)):
	print("B: A是我儿子")


# 判断两个'set集合', 是否有交集(没任何交集才是True)
if(not A.isdisjoint(B)):
	print("A, B 没有交集")

if(not B.isdisjoint(A)):
	print("B, A 没有交集")





#
# 4.'set集合'访问
#
'''
	* 无法通过'下标'来访问'set集合'中的元素, 即便'set集合'是有序的;
	* 无法通过'key键值'来访问'set集合'中的元素, 'set集合'是哈希, 但不是'键值对'的;
'''
pset={9,1,2,5,7,3,2,6,23,"love you"}

# 检查某个值, 是否存在于set 'set集合'中
if(1 in pset):
	print("1 in pset yes")

pint=2
if(pint in pset):
	print("pint in pset yes")



# len() 获取'set集合'的长度
print("pset len()=%d" %(len(pset)))



# add()添加单个元素
if("love" in pset):
	pset.add("love")

# update([])添加多个元素(相当于插入了一个list)
if("you","bitch",666 in pset):
	pset.update(["you","bitch",666])
print(pset)



# 'set集合'删除元素
# remove() 方法(删除空元素都会引发错误)
if("love" in pset):
	pset.remove("love")

# discard() 方法(删除空元素返回None)
if("you" in pset):
	pset.discard("you")
print(pset.discard("you"))

# pop() 方法(不能指定删除哪个元素)
if(len(pset)):
	pset.pop()
print(pset)



# clear() 方法(清空所有元素)
pset.clear()
print(pset)

# 彻底删除'set集合'
del pset




