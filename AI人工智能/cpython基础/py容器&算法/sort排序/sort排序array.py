#coding=utf-8



import array



# 初始化数字array(必须从list/tuple 中强转导入, 最低消耗了)
arr_int=array.array('l', [9,8,7,6,5,4,3,2,1,0])
arr_uint=array.array('L', (9,8,7,6,5,4,3,2,1,0))
arr_float=array.array('f', [9,8,7,6,5,4,3,2,1,0])



#
# 反序
#
# 自我反序API
arr_int.reverse()
arr_uint.reverse()
arr_float.reverse()

# 切片复制反序
print(arr_int[::-1])
print(arr_uint[::-1])
print(arr_float[::-1])

# reversed() 反序函数(废物API, 得到一个reversed object, 还要做强转, 一般不这么用)
print(reversed(arr_int))
print(reversed(arr_uint))
print(reversed(arr_float))





#
# 排序
#
# 自我反序API(无, 略)



# sorted() 强制排序
print(sorted(arr_int,reverse=True))
print(sorted(arr_uint))
print(sorted(arr_float,reverse=True))

a = ['A','Q','a','b','d','c','B','A']
# 默认根据ascii 码大小进行排序
print(sorted(a))

# 忽略大小写, 直接按照'英文字母顺序'进行排序
print(sorted(a,key = str.lower))

# ps:
# 	一维数组, 一般都是统一元素, 排序的时候很简单, 基本没有多少特殊用法, 这里不再详细叙述;

