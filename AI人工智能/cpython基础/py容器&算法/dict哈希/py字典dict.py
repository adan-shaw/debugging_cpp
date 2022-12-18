#coding=utf-8



# '原地可变类型/不可变类型'简介
'''
	原地不可变类型又叫: 可哈希(hashable)类型, 
	原地可变类型又叫: 不可哈希类型;

	原地不可变类型:
		* 数字类型: int, float, decimal.Decimal, fractions.Fraction, complex
		* 字符串类型: str, bytes
		* tuple
		* frozenset
		* 布尔类型: True, False
		* None

	原地可变类型:
		* list
		* dict
		* set

	只有可以hash的类型才可以作为dict的键;
	只有可以hash的类型才可以放入set中, 所以set本身不可以嵌套存放在set中;


	如何实际检验是否原地可变?
	答:
		hash还是会返回原地不可变类型的hash值, 
		如果对一个原地可变的类型调用次函数, 那么会返回TypeError;
'''



#
# 1.创建字典
#
pdict={ "A":191, "B":"love", "C":999, "D":"you" }
print(pdict)


# 一体化内嵌dict
myfamily={
	"child1": {
		"name": "Phoebe Adele",
		"year": 2002
	},
	"child2": {
		"name": "Jennifer Katharine",
		"year": 1996
	},
	"child3": {
		"name": "Rory John",
		"year": 1999
	}
}
print(myfamily)


# 最直观的内嵌方式(有插入元素的消耗)
child1={
	"name": "Phoebe Adele",
	"year": 2002
}
child2={
	"name": "Jennifer Katharine",
	"year": 1996
}
child3={
	"name": "Rory John",
	"year": 1999
}

myfamily2={
	"child1": child1,
	"child2": child2,
	"child3": child3
}
print(myfamily2)





#
# 2.字典拷贝
#
pdict1={ "M":{"b":90909, "a":6666}, "A":191, "B":"love0.0", "C":999, "D":"you^_^" }

# copy()拷贝
pdict2=pdict1.copy()
pdict1["M"]["b"]=9999999
print(pdict2)

# 构造函数拷贝
pdict2=dict(pdict1)
pdict1["M"]["b"]="ppppp"
print(pdict2)

# deepcopy()拷贝
import copy
del pdict2
pdict2=copy.deepcopy(pdict1)
pdict1["M"]["b"]="xxxxx"
print(pdict2)





#
# 3.字典操作
#
# python Dictionary API:
'''
clear()			删除字典中的所有元素(python 2.7也有, 不同于list)
copy()			返回字典的副本(python 2.7也有, 不同于list)

fromkeys()	返回拥有指定key和值的字典
get()				返回指定key的值
setdefault()返回指定key的值(相当于: 不存在则创建,存在则返回value)
items()			返回包含每个'键值对'的元组的列表
keys()			返回包含字典key的列表
values()		返回字典中所有值的列表
pop()				删除拥有指定key的元素
popitem()		删除最后插入的'键值对'
update()		使用指定的'键值对'字典进行更新
'''
pdict={ "A":191, "B":"love", "C":999, "D":"you" }

# 根据key 值访问(读)
print(pdict["A"])

# 根据key 值访问(写)
pdict["A"]=555

# 新增'键值对'
pdict["E"]=222

# 更新多个'键值对'(存在则修改, 不存在则自动创建) [相当于两个dict 合并, 遇到相同的'键值对'则合并, 保留update()传入的那一份]
pdict.update({"E":191000, "Q":"love", "R":999})
print(pdict)

# 根据key 值, 获取value 值(读)
print(pdict.get("A"))

# 根据key 值, 设置value 值(读写, 不存在则创建, 存在则返回value)
print(pdict.setdefault("B",6666))
print(pdict.setdefault("E",7777))



# 判断字典中, 是否存在指定的某个key(判断首选)
if(pdict.get("F") is None):
	print("pdict donest have a key named 'F'")

# 判断字典中, 是否存在指定的某个key(循环判断, 一般不用, 耗性能!!)
if("A" in pdict):
	print("pdict has a key named 'A'")



# 遍历字典-所有key(性能较好)
for x in pdict:
	print(x)

# 遍历字典-所有value(性能较好)
for x in pdict:
	print(pdict[x])

# 遍历字典-所有key-value(性能较好)
for x in pdict:
	print(x,pdict[x])

# 使用keys() 返回'字典所有的key'进行遍历(性能较差, 需要先创建list)
for x in pdict.keys():
	print(x)

# 使用values() 返回'字典所有的value'进行遍历(性能较差, 需要先创建list)
for x in pdict.values():
	print(x)

# 使用items() 返回'字典所有的键和值'进行遍历(性能较差, 需要先创建list)
for x,y in pdict.items():
	print(x,y)



# 获取字典中'键-值对'的数量(字典长度)
print(len(pdict))



# 根据指定key 删除'键值对'(删除不存在的'键值对', 会报错; 删除前, 必须确定元素存在)
if("E" in pdict):
	pdict.pop("E")
print(pdict)

# del 删除指定的'键值对'
if("D" in pdict):
	del pdict["D"]
print(pdict)

# 根据指定key 删除(性能最优)
if(pdict.get("A") is None):
	pdict.pop("A")

# 删除字典中最后插入的一对键值(在3.7之前的版本中, 删除随机项目, 没有参数, 不能输入参数)
pdict.popitem()
print(pdict)



# clear() 清除字典中所有'键-值对'
pdict.clear()
print(pdict)

# del 删除字典
del pdict







