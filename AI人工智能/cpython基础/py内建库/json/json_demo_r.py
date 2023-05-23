#coding=utf-8



import json
import io



# json.load和json.loads()的区别:
'''
	loads() 传的是json字符串, 而load() 传的是文件对象
	使用loads() 时需要先读取文件在使用, 而load() 则不用

	一个file, 一串json字符串, 只能恢复到一个py变量中;
	多个py变量, 请保存到多个file中, or 使用io.read() 逐行读取, 然后逐串py-json压缩字符串解析;
'''
#
# 1.从json压缩字符串恢复数据到python 变量中
#
# Py字典
pdict={"name": "小李", "sex": "男", "tel": ["8234588", "1244351"], "isonly": True}
# 转换为json字符串
jsonStr1=json.dumps(pdict)
print("jsonStr1",type(jsonStr1), jsonStr1)
# json字符串装载进变量
ppdict = json.loads(jsonStr1)
print("ppdict",type(ppdict), ppdict)

# Py集合(json 不能用set集合作为输入源)
#pset={"a", "b" , "c"}
#jsonStr2=json.dumps(pset)
#print("jsonStr2",type(jsonStr2), jsonStr2)
#ppset = json.loads(jsonStr2)
#print("ppset",type(ppset), ppset)

# Py列表
plist=[[1,2],11,12,13,14,15,16,17,18,19]
jsonStr3=json.dumps(plist)
print("jsonStr3",type(jsonStr3), jsonStr3)
pplist = json.loads(jsonStr3)
print("pplist",type(pplist), pplist)

# Py元组
ptuple=((1,2,3,4),"love","you","idiot",1,1,1,1,2,"","love",0)
jsonStr4=json.dumps(ptuple)
print("jsonStr4",type(jsonStr4), jsonStr4)
pptuple = json.loads(jsonStr4)
print("pptuple",type(pptuple), pptuple)





#
# 2.从json 展开格式的文件中, 恢复数据到python 变量中(一个file,一串json字符串,只能恢复到一个py变量中,多个py变量,请保存到多个file中)
#
#val_dict,val_list,val_tuple = json.load(open('test.json','r'))
#val_dict = json.load(open('test.json','r'))
#val_list = json.load(open('test.json','r'))
val_tuple = json.load(open('test.json','r'))
#print("val_dict",type(val_dict), val_dict)
#print("val_list",type(val_list), val_list)
print("val_tuple",type(val_tuple), val_tuple)




