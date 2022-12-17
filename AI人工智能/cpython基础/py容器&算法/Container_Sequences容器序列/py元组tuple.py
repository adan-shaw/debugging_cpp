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
ptuple=((1,2,3,4),"love","you","bitch",1,1,1,1,2,"","love",0)

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




