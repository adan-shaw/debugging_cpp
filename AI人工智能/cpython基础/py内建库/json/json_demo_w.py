#coding=UTF-8



import json
import io



# json.dumps和json.dump的区别:
'''
	dump()不需要使用.write()方法, 可直写到file, json 自动展开, 阅读时更美观;
	dumps()需要使用.write()方法写入, json 自动压缩, 方便传送, 不方便阅读;
	如果把字典写到file里面的时候, dump()好用;
	如果不需要操作file, 或需要把内容存储到数据库和excel, 则需要使用dumps()先把字典转换成字符串, 再写入;

	
'''



#
# 1.json.dumps() + io.write()
#
# Py字典
pdict={"name": "小李", "sex": "男", "tel": ["8234588", "1244351"], "isonly": True}
print(type(pdict), pdict)
# 转换为json压缩字符串
jsonStr1=json.dumps(pdict)
print(type(jsonStr1), jsonStr1)

# Py集合(json 不能用set集合作为输入源)
#pset={"a", "b" , "c"}
#print(type(pset), pset)
#jsonStr2=json.dumps(pset)
#print(type(jsonStr2), jsonStr2)

# Py列表
plist=[[1,2],11,12,13,14,15,16,17,18,19]
print(type(plist), plist)
jsonStr3=json.dumps(plist)
print(type(jsonStr3), jsonStr3)

# Py元组
ptuple=((1,2,3,4),"love","you","idiot",1,1,1,1,2,"","love",0)
print(type(ptuple), ptuple)
jsonStr4=json.dumps(ptuple)
print(type(jsonStr4), jsonStr4)



# 打开file, 写入json压缩字符串
f=io.open("test.json", "w",encoding='UTF-8',newline=None)

f.write(jsonStr1.encode('UTF-8').decode('UTF-8'))
f.write("\n".encode('UTF-8').decode('UTF-8'))

#f.write(jsonStr2.encode('UTF-8').decode('UTF-8'))
#f.write("\n".encode('UTF-8').decode('UTF-8'))

f.write(jsonStr3.encode('UTF-8').decode('UTF-8'))
f.write("\n".encode('UTF-8').decode('UTF-8'))

f.write(jsonStr4.encode('UTF-8').decode('UTF-8'))
f.write("\n".encode('UTF-8').decode('UTF-8'))

f.flush()
f.close()





#
# 2.json.dump() 直写(json 自动展开, 阅读时更美观)
#
# json.dump()参数:
'''
	sort_keys:  是否排序
	indent:     定义缩进距离
	separators: 定义分隔符(分隔符的变量类型是一个元组)
	skipkeys:   是否允许json字串编码字典对象时,字典的key不是字符串类型
'''
# 写入json 展开格式字符串
json.dump(pdict, open('data.json', 'a'), sort_keys=True, indent=2, separators=(',',':'), skipkeys=True)

#json.dump(pset, open('data.json', 'a'), sort_keys=True, indent=2, separators=(',',':'), skipkeys=True)

json.dump(plist, open('data.json', 'a'), sort_keys=True, indent=2, separators=(',',':'), skipkeys=True)

json.dump(ptuple, open('data.json', 'a'), sort_keys=True, indent=2, separators=(',',':'), skipkeys=True)


