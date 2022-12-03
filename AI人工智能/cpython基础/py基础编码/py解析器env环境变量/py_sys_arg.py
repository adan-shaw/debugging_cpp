#coding=utf-8



import sys



# 对argv[] list 求len(), 就是argc
argc=len(sys.argv)

# 获取命令行参数: sys.argv[0] 必然为py 文件所在的路径
print(sys.argv[0])

# 打印所有arg(倒序打印)
while(argc > 0):
	argc-=1
	print(sys.argv[argc])



# 错误!! 
'''
	如果argv不存在, 则不能引用!! 否则会报错: list index out of range(列表超出索引范围)
	所以, 使用argv 之前, 一定要首先获取argc!! 以防止访问溢出
'''
