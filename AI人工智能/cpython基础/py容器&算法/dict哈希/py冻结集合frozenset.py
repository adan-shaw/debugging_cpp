#coding=utf-8



# 'frozenset集合'简介
'''
	'frozenset集合'不允许有重复元素, 
	重复元素会被自动吞灭, 最终只保留一个; 
	'frozenset集合'还会自动从小到大排序, 不能修改'frozenset集合'的顺序;

	'frozenset集合'内的元素类型必须是: 可哈希(hashable)类型;

	'frozenset集合'可以做交集,补集,差异集,等运算, 但只能做引入端, 不能做最终修改端;
	'frozenset集合'不允许增删元素;
'''



#
# 1.'frozenset集合'的创建和拷贝
#
# 创建'frozenset集合'(必须通过构造函数来拷贝一份frozenset())
pset={9,1,2,5,7,3,2,6,23,(1,2),"idiot"}
pfset=frozenset(pset)
print(pset)

# copy()拷贝('frozenset集合'没有深拷贝)
pset0=pfset.copy()
print(pset0)

# deepcopy()拷贝
import copy
pset0=copy.deepcopy(pfset)
print(pset0)





#
# 2.'frozenset集合'访问
#
'''
	* 无法通过'下标'来访问'frozenset集合'中的元素, 即便'frozenset集合'是有序的;
	* 无法通过'key键值'来访问'frozenset集合'中的元素, 'frozenset集合'是哈希, 但不是'键值对'的;
'''
pset={9,1,2,5,7,3,2,6,23,"love you"}
pfset=frozenset(pset)

# 检查某个值, 是否存在于'frozenset集合'中
if(1 in pfset):
	print("1 in pfset yes")

pint=2
if(pint in pfset):
	print("pint in pfset yes")



# len() 获取'frozenset集合'的长度
print("pfset len()=%d" %(len(pfset)))





#
# 3.'frozenset集合'运算: 交集,补集,差异集, 等等
#
# 补集(保护式):
'''
	union() 融合两个'frozenset集合', 并触发重新排序, 去除重复元素.
	调用union() 方法的'frozenset集合', 不会有任何改变,
	补集结果, 返回到新的'set集合'中;
'''
pset1 = {"a", "b" , "c"}
pfset1=frozenset(pset1)
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset = pfset1.union(pfset2)
print(pfset1)
print(pfset2)
print(pset)

# 补集(破坏式): 
'''
	update() 融合两个'frozenset集合', 并触发重新排序, 去除重复元素.
	将一个'frozenset集合', 插入到另外一个'set集合'中; 
	被插入的'set集合', 值会有变动, 除非插入了空集;
'''
pset1 = {"a", "b" , "c"}
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset1.update(pfset2)
print(pset1)
print(pfset2)



# 去除交集(保护式):
'''
	difference() 剔除一个'frozenset集合'中, 与另外一个'frozenset集合'的共有的元素, 
	将结果组成一个新'set集合'返回, 原'frozenset集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pfset1=frozenset(pset1)
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset3=pfset1.difference(pfset2)
print(pfset1)
print(pfset2)
print(pset3)

# 去除交集(破坏式):
'''
	difference_update() 剔除一个'set集合'中, 与另外一个'frozenset集合'的共有的元素, 
	直接在原'set集合'上删除'所有共有元素';
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset1.difference_update(pfset2)
print(pset1)
print(pfset2)



# 先去除交集, 得出两个差集后, 再融合这两个差集(保护式): 
'''
	symmetric_difference() 剔除两个'frozenset集合'的共有的元素, 
	将两个'frozenset集合'剩下所有非共有的元素,
	最后组成一个新的'set集合'返回, 原'frozenset集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pfset1=frozenset(pset1)
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset3=pfset1.symmetric_difference(pfset2)
print(pfset1)
print(pfset2)
print(pset3)

# 先去除交集, 得出两个差集后, 再融合这两个差集(破坏式): 
'''
	symmetric_difference_update() 剔除两个'frozenset集合'的共有的元素, 
	将两个'frozenset集合'剩下所有非共有的元素,
	最后将这'两个'frozenset集合'的非共有的元素', 写入到调用symmetric_difference_update() 的那个'set集合';
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset1.symmetric_difference_update(pfset2)
print(pset1)
print(pfset2)



# 取得交集(保护式):
'''
	intersection() 拾取两个'frozenset集合'中'共有的元素', 
	将结果组成一个新的'set集合'返回, 原'frozenset集合'不改动;
'''
pset1 = {"a", "b" , "c", 1, 2}
pfset1=frozenset(pset1)
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset3=pfset1.intersection(pfset2)
print(pfset1)
print(pfset2)
print(pset3)

# 取得交集(破坏式):
'''
	intersection_update() 拾取两个'frozenset集合'中'共有的元素', 
	直接在原'set集合'上删除'所有非共有元素';
'''
pset1 = {"a", "b" , "c", 1, 2}
pset2 = {1, 2, 3}
pfset2=frozenset(pset2)
pset1.intersection_update(pfset2)
print(pset1)
print(pfset2)





#
# 4.两个'frozenset集合'的判断操作
#
# 判断AA 是否是BB 的子集
A={1,2,3}
AA=frozenset(A)
B={1,2,3,4,5,6}
BB=frozenset(B)
if(AA.issubset(BB)):
	print("AA: BB是我爸爸")

# 判断BB 是否是AA 的超集
if(BB.issuperset(AA)):
	print("BB: AA是我儿子")


# 判断两个'frozenset集合'的交集是否为空集
print(AA.isdisjoint(BB))
print(BB.isdisjoint(AA))

if(not AA.isdisjoint(BB)):
	print("AA, BB 的交集不为空")

if(not BB.isdisjoint(AA)):
	print("AA, BB 的交集不为空")



