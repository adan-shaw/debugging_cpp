#coding=utf-8



#
# range(): 返回一个纯数字的list
#
# (默认起始=0,终止=10,默认步进=1)
print(range(10))

# (起始,终止,默认步进=1)
print(range(1,10))

# (起始,终止,步进)
print(range(1,10,3))





#
# memoryview() 返回'二进制数据'数组
#
v = memoryview(b'abcefg')
print(v[1])  # b
print(v[-1]) # g
