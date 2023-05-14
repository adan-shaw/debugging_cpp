#coding=utf-8



# array(字符,整数,浮点数):
'''
	python array 与c 语言的数组类似, 支持字符,整数,浮点数, 也有大量操作api;
	array 与string 可以轻松互转;

	array 不是python 内置模块, 需要先import 导入array;
'''
import array



#
# 1.创建array 实体
#
'''
array 常用的数据类型:
	类型码		C 类型						Python 类型		以字节表示的最小尺寸		推荐
		'b'		signed char			int							1
		'B'		unsigned char		int							1(无符号字符)
		'u'		Py_UNICODE			Unicode 字符			2									*
		'h'		signed short		int							2
		'H'		unsigned short	int							2
		'i'		signed int			int							2
		'I'		unsigned int		long						2
		'l'		signed long			int							4									*
		'L'		unsigned long		long						4(无符号整形)
		'f'		float						float						4									*
		'd'		double					float						8

	Python2默认使用编码ascii, 不支持中文;
	Python3默认使用编码utf-8, 支持中文, 其英文编码部分和ascii相同;
	在array 容器中, 兼容两者比较困难;
	建议直接用str() 强转为字符串来处理;

	无论你做多大努力, 用python array 来处理字符串, 都是极度困难的, 也没有必要!!
	明明有str() 不用, 非得用array 来处理字符串, 几个意思? 自己添乱?

	因此, array 基本上用来对接c/c++ 的一位数组;
	二维数组, 请用numpy;
	n维数组, 请用numpy;
'''

# 初始化字符array(等价于字符串, 基本不用, 废弃!! python2 打印array时, 会多带一个'u' 表示输出字符串是utf-8 字符)
arr_str=array.array('u', u"hello world")
print(arr_str)

# 将容器的内容[全部元素], 转换为可打印, 可传递的字符串
ptmp=str(arr_str)
print(ptmp)



# 初始化数字array(必须从list/tuple 中强转导入, 最低消耗了)
arr_int=array.array('l', [9,8,7,6,5,4,3,2,1,0])
arr_uint=array.array('L', (9,8,7,6,5,4,3,2,1,0))
arr_float=array.array('f', [9,8,7,6,5,4,3,2,1,0])
print(arr_int)
print(arr_uint)
print(arr_float)


# 初始化一个空array
tmp=array.array('b')
tmp=array.array('u')
tmp=array.array('l')
tmp=array.array('L')
tmp=array.array('f')





#
# 2.Sequences序列通用操作
#
'''
所有Sequences序列的通用的操作:
	下标索引(indexing) [ 下标从0 开始!! ]
	下标分片(sliceing) [ 切片也可以用作区间索引,区间访问,但性能一般; 先拷贝,再索引,性能还能好到哪里去? 但也比逻辑索引强; ]
	迭代(iteration)
	运算符'+'加号重载(adding)
	运算符'*'乘号重载(multiplying)
'''

# 下标索引(indexing)
print(arr_int[0])
print(arr_int[1])
print(arr_int[2])
print(arr_int[-1])		# 倒数第一位


# 下标切片(sliceing)
print(arr_int[0:4])	# 第0-4个字符
print(arr_int[5:8])	# 第5-8个字符
print(arr_int[4:])	# 截取第4个字符, 到字符串结束为止! 
print(arr_int[-5:])	# 截取倒数第5位, 到字符串结束为止!
print(arr_int[-1:])	# 截取最后一个字符



# 迭代(iteration)
# 直接对array 进行迭代, 性能最好
for x in arr_int:
	print(x)

# 先将array 强转为list, 再迭代, 没必要!(废弃, 语法错误, 禁止这样用)
'''
for x in range(arr_int):
	print(x)
'''

# 迭代找子字符串(废弃, 没必要, 直接用str.find()/str.index() 更好) [array数组也有array.index()]
if 9 in arr_int:
	print("9 in arr_int: ", arr_int)

if arr_int.index(9):
	print("array.index(9) == True: ", arr_int[arr_int.find(9)])



# 运算符'+'加号重载(adding)
tmp=arr_int+arr_int
print(tmp)

# 不同类型, 即便是signed int 与unsigned int 也不能相加!! 否则报错;
'''
tmp=arr_int+arr_uint
print(tmp)
'''



# 运算符'*'乘号重载(multiplying)
print(arr_int*3)
# 不能是两个字符串相乘, 两个array 相乘, 没意义; 要向量相乘, 请用numpy
#print(arr_int*arr_int)



# 获取数组的长度
print(len(arr_int))





#
# 3.array 其他API
#
# 插入元素(在匹配位置之前插入)
arr_uint.insert(-1,9)
arr_uint.insert(0,9)
print(arr_uint)

# 查找元素, 返回'首次出现'的下标, 找不到直接报错!!
print(arr_uint.index(9))
#print(arr_uint.index(99))

# 根据下标索引, 返回弹出array 中的元素, 默认参数: -1 (即剔除最后一位, 剔除其它位置, array 需要整体向前移动)
arr_uint.pop(-1)
arr_uint.pop(1)
print(arr_uint)
print(arr_uint.pop(1))

# 根据值索引, 移除array 中'首次出现'的元素;
arr_uint.remove(3)
print(arr_uint)

# 反转数组的顺序
arr_uint.reverse()
print(arr_uint)

# 统计某个元素在数组中的出现次数
print(arr_uint.count(9))








#
# 4.array 录入/导出数据(基本上废弃)
#
'''
	录入前, 需要先创建array 空容器(最好是空容器, 防止数据被污染);
	导出时, 可以随意导出, 但默认不会影响导入端的数据, 除非你重定向, 直接导出覆盖导入端的数据;
'''
# 从一个已打开的file 中录入数据到array; 一byte一byte地读取数据到array, n 是截断保险位(单位byte字节);(弃用)
'''
fd=open("./array数组.py",'rb')
tmp=array.array('b')
tmp.fromfile(fd, 32)					# file录入数据到array
print(tmp)
#tmp.tofile(fd)								# array导出数据到file
fd.close()
'''

# 从list 中录入数据到array
tmp=array.array('l')
tmp.fromlist([1,2,3,4,5])			# list录入数据到array
print(tmp)
print(tmp.tolist())						# array导出数据到list

# 从string 中录入数据到array
tmp=array.array('b')
tmp.fromstring("idiot")				# string录入数据到array
print(tmp)
print(tmp.tostring())					# array导出数据到string

# 从unicode string 中录入数据到array
tmp=array.array('u')
tmp.fromunicode(u'idiot2')		# unicode string录入数据到array
print(tmp)
print(tmp.tounicode())				# array导出数据到unicode string


