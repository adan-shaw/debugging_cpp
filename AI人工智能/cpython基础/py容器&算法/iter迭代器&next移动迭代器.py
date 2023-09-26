#coding=utf8



# 迭代器iter(), next()移动迭代器, len()获取容器当前的容量
'''
	next()移动迭代器时, 一旦越界, 就会崩溃;
	所以next()移动迭代器时, 需要先使用len() 获取容器边界;

	总结:
		iter() + next() + len(), 适合用while 迭代遍历, 性能一般, 但比较麻烦;
		没什么必要, 尽量不要这样迭代遍历容器;
'''





# 迭代enumerate枚举对象
enum_box = ['Spring', 'Summer', 'Fall', 'Winter']
p = enumerate(enum_box)
p = iter(p)
l = len(enum_box)

while(l > 0):
	print(next(p))
	l-=1
	# next() 一旦迭代越界, 就会报错



# 迭代'字符串容器'(相当于迭代一维数组)
s = 'abcd'
p = iter(s)
l = len(s)
while(l > 0):
	print(next(p))
	l-=1





# 迭代list(这种迭代方式, 效率十分低, 这种情况下, 根本没有必要用iter(), 直接用list 迭代更好)
plist = [1, 2, 3]
for i in iter(plist):
	print(i)

# 迭代tuple
ptuple=(1,2,3,4,"love","you","bbbbb",1,1,1,1,2,"","love",0)
for i in iter(ptuple):
	print(i)

# 迭代set
pset={9,1,2,5,7,3,2,6,23,"U","fuk",111,999}
for i in iter(pset):
	print(i)

# 迭代dict
pdict={ "M":{"b":90909, "a":6666},"A":191, "B":"love0.0", "C":999, "D":"you^_^" }
iter_tmp=iter(pdict)
# iter() 索引字典, 会触发二次索引, 效率十分差!!
print(iter_tmp.next(), pdict[iter_tmp.next()])



