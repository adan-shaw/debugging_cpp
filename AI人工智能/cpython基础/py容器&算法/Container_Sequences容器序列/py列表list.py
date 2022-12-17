#coding=utf-8



#
# 1.list 初始化/拷贝
#
# 内嵌list 初始化
plist=[[1,2],11,12,13,14,15,16,17,18,19]

# 切片拷贝(性能不好, 慎用, 会触发逻辑循环, 废物写法)
plist2=plist[:]

# 逻辑拷贝(性能不好, 慎用, 废物写法, 看上去高大上而已)
plist2=[i for i in plist]



# list 拷贝dict->key, dict->value (dict 的keys/values 元素类型一致的情况)
d1={'a':1,'b':2,'c':3,'d':4,'e':5}
l1_keys=list(d1.keys())
print(l1_keys)
l1_values=list(d1.values())
print(l1_values)

# list 拷贝dict->key, dict->value (复杂dict, 元素类型不统一时, 拷贝还需做if+type()剔除非目标元素)
d1={'a':1,'b':2,'c':3,'d':4,'e':[1,2,3]}
for x in d1.values():
	if type(x) is int:
		plist2.append(x)



# copy拷贝实体
import copy
plist=[[1,2],11,12,13,14,15,16,17,18,19]
plist2=copy.deepcopy(plist)
print(plist2)





#
# 2.Sequences序列通用操作
#
'''
所有Sequences序列的通用的操作:
	下标索引(indexing) [ 下标从0 开始!! ]
	下标分片(sliceing) [ 切片也可以用作区间索引,区间访问,但性能一般; 先拷贝,再索引,性能还能好到哪里去? 但也比逻辑索引强; ]
	迭代(iteration)
	运算符'+'加号重载(adding)     [ 运算符重载, list+list返回list, tuple+tuple返回tuple; *乘号同理, :冒号切片同理 ]
	运算符'*'乘号重载(multiplying)
'''

# 下标索引(indexing)
plist=[[1,2],11,12,13,9,15,16,17,18,19,9,9,9]
print(plist[0][1])
print(plist[1])
print(plist[2])
print(plist[-1])		# 倒数第一位


# 下标切片(sliceing)
print(plist[0:4])	# 第0-4个字符
print(plist[5:8])	# 第5-8个字符
print(plist[4:])	# 截取第4个字符, 到字符串结束为止! 
print(plist[-5:])	# 截取倒数第5位, 到字符串结束为止!
print(plist[-1:])	# 截取最后一个字符



# 迭代(iteration)
# 直接对array 进行迭代, 性能最好
for x in plist:
	print(x)

# 先将list 强转为list, 再迭代, 没必要!(废弃, 语法错误, 禁止这样用)
'''
for x in range(plist):
	print(x)
'''

# 迭代找子字符串(废弃, 没必要, 直接用str.find()/str.index() 更好) [array数组也有array.index()]
if 9 in plist:
	print("9 in plist: ", plist)

if plist.index(9):
	# 找不到元素则报错
	print("plist.index(9)已找到, 返回下标; 根据下标再返回元素值: ", plist[plist.index(9)])



# 运算符'+'加号重载(adding)
tmp=plist+plist
print(tmp)

# 不同类型, 即便是signed int 与unsigned int 也不能相加!! 否则报错;
'''
plist2=['a','b']
tmp=plist+plist2
print(tmp)
'''


# 运算符'*'乘号重载(multiplying)
print(plist*3)
# 不能是两个字符串相乘, 两个list 相乘, 没意义; 要向量相乘, 请用numpy
#print(plist*plist)



# len() 获取list 长度
plist=[1,2,3,4,5,6,7,8,9,0]
print(len(plist))





#
# 3.python List API:
#
'''
append()	在列表的末尾添加一个元素
count()		返回具有指定值的元素数量
extend()	将列表元素(或任何可迭代的元素)添加到当前列表的末尾
index()		返回具有指定值的第一个元素的索引
insert()	在指定位置添加元素
pop()			删除指定位置的元素
remove()	删除具有指定值的项目
reverse()	颠倒列表的顺序
sort()		对列表进行排序

clear()		删除列表中的所有元素(python2 没有这个api)
copy()		返回列表的副本(python2 没有这个api)
'''


# append() 从list底部/尾部, 插入元素
plist=[]
plist.append(1)
plist.append("love you")
print(plist)
print(type(plist[0]))
print(type(plist[1]))

# insert() 在list 指定位置插入元素
plist.insert(0,"love you again")
print(plist)



# remove() 删除指定索引
plist.remove("love you again")
print(plist)

# pop() 不制定索引值, 自动删除最后一项
print(plist.pop())
print(plist)

# 根据下标del 删除list 元素
del plist[0]
print(plist)

# 删除整个list
del plist



# 颠倒List 的元素顺序
plist=[[22,33],1,2,3,3,3,3,3,6,"fff","fff","fff"]
plist.reverse()
print(plist)

# 对列表进行重新排序(根据首字母的acsII 码值进行排序/数字的数值大小进行排序; 如果需要重新指定排序cmp 比较函数, 请用sorted())
# python3 不允许元素类型的list 进行排序, python2 可以;(另有详细解释, 这里不展开)
#plist.sort()
print(plist)



# 返回指定索引值的count 计数(数字)
print(plist.count(3))

# 返回指定索引值的count 计数(字符串)
print(plist.count("fff"))





#
# 4.合并两个list
#
# 运算符重载
plist=[[22,33],1,2,3,4,5,6]
plist2=["diao", "you","love"]
plist2=plist + plist2
plist[0][0]=99999999
print(plist2)


# extend() 附加到尾部合并两个list(合并后, 所有数据都在plist2)
plist=[[22,33],1,2,3,4,5,6]
plist2=["diao", "you","love"]
plist2.extend(plist)
plist[0][0]=777777
print(plist2)


# 逻辑合并
plist=[[22,33],1,2,3,4,5,6]
for x in plist2:
	if(x == "love"):
		continue
	else:
		plist.append(x)
print(plist)





# 将所有元素合并成一个字符串, 并添加分割符号
plist=['2','4','5']
x=(',').join(plist)
print(x)











