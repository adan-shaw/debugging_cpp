#coding=utf-8



import sys
py_version=sys.version_info.major



# tuple元组简介
'''
	tuple元组基本上是不变动的, 不支持增/删/改, 只支持查询;
	一旦需要更改, 必然先导出数据, 此时选择list[] 作为导出数据的对象, 就最合理了;
	不过一般情况下, 如果元素有变更的情况, 请自觉使用list[], 不要乱用;

	tuple元组同样不能排序, 想排序必须先导出数据, 强转为list, 排序后再转为tuple元组;
'''

#
# tuple元组的创建与删除, 基本api 操作
#
# tuple元组的创建
ptuple=((1,2,3,4),"love","you","idiot",1,1,1,1,2,"","love",0)

# 将容器的内容[全部元素], 转换为可打印, 可传递的字符串
ptmp=str(ptuple)
print(ptmp)

# list拷贝tuple元组
print(list(ptuple))

# tuple元组拷贝list
print(tuple(list(ptuple)))

# tuple元组下标访问
print(ptuple[1])
print(ptuple[0][0])

# 删除元组
#del ptuple



# 统计某个元素出现的次数
print(ptuple.count(1))
print(ptuple.count("love"))









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
ptuple=((1,2,3,4),"love","you","idiot",1,1,1,1,2,"","love",0,9,9,9)
print(ptuple[0][1])
print(ptuple[1])
print(ptuple[2])
print(ptuple[-1])		# 倒数第一位


# 下标切片(sliceing)
print(ptuple[0:4])	# 第0-4个字符
print(ptuple[5:8])	# 第5-8个字符
print(ptuple[4:])		# 截取第4个字符, 到字符串结束为止! 
print(ptuple[-5:])	# 截取倒数第5位, 到字符串结束为止!
print(ptuple[-1:])	# 截取最后一个字符



# 迭代(iteration)
# 直接对array 进行迭代, 性能最好
for x in ptuple:
	print(x)

# 先将tuple元组 强转为list, 再迭代, 没必要!(废弃, 语法错误, 禁止这样用)
'''
for x in range(ptuple):
	print(x)
'''

# 迭代找子字符串(废弃, 没必要, 直接用str.find()/str.index() 更好) [array数组也有array.index()]
if 9 in ptuple:
	print("9 in ptuple: ", ptuple)

if ptuple.index(9):
	# 找不到元素则报错
	print("ptuple.index(9)已找到, 返回下标; 根据下标再返回元素值: ", ptuple[ptuple.index(9)])



# 运算符'+'加号重载(adding)
tmp=ptuple+ptuple
print(type(tmp),tmp)

# 不同类型, 即便是signed int 与unsigned int 也不能相加!! 否则报错;
'''
ptuple2=['a','b']
tmp=ptuple+ptuple2
print(tmp)
'''


# 运算符'*'乘号重载(multiplying)
print(type(tmp),ptuple*3)
# 不能是两个字符串相乘, 两个tuple元组 相乘, 没意义; 要向量相乘, 请用numpy
#print(ptuple*ptuple)



# len() 获取tuple元组 长度
ptuple=(1,2,3,4,5,6,7,8,9,0)
print(len(ptuple))



# 判断ptuple 是否为空(在Python中, False,0,'',"",[],{},()都可以视为假)
#print("check empty")
ptuple2=()
if not ptuple2:
	print("ptuple2 is empty")

if not len(ptuple2):
	print("ptuple2 is empty [cheap, dont use this way to make a logic-IF]")




