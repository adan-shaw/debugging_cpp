#coding=utf-8



# io对象操作api:
'''
io.open()								打开file
io.close()							关闭file
io.flush()							冲刷file内部缓冲, 立刻写盘到file
io.fileno()							返回一个整型int 的file描述符fd
io.isatty()							如果file连接到一个终端设备, 则返回True(否则返回False)
io.next()								返回file下一行
io.tell()								返回file当前位置pos
io.truncate(size)				从file的首行首字符开始, 截断size 个字符(无size, 则表示从当前pos位截断)
io.read(pos)						从file当前位置pos起, 读取size 字节(未给定值or给定负值, 则读取所有)
io.readline(n)					读取整行, 一共读取n行, 包括 "\n" 字符
io.readlines()					读取所有行, 返回一个list
io.write(str)						向file写入一串string字符串
io.writelines(seque)		向file写入一个list/tuple(换行符需要自己添加, 否则每个元素都粘在一起)
io.seek(offset,whence)	修改file当前pos 位

'''



# file对象操作api:
'''
fd.readable()						判断fd流是否可读, 是则返回True(不可读返回False)
fd.writeable()					判断fd流是否可写, 是则返回True
fd.seekable()						判断fd流支持随机访问(支持修改pos 位), 则返回True
'''



import io



# t为: 文本读写
# b为: 二进制读写
fd = open('./py_io.py','rt')
fd.close()

# buffering=0: 同步io(指定不带缓冲区, 直读)
fd_sync = io.open("./py_io.py", "rb", buffering=0)
fd_sync.close()




# 获取/设置python 机默认的file 缓冲区大小(默认8192 kb)
io.DEFAULT_BUFFER_SIZE



fd = open('./py_io.py','rt')
# 修改pos 位(offset=偏移量, whence=起始位)
'''
whence:
	SEEK_SET=0 - 起始位(默认)
	SEEK_CUR=1 - 当前位
	SEEK_END=2 - 结束位
'''
print(io.SEEK_SET, io.SEEK_CUR, io.SEEK_END)
# 当前pos 位后移10 type字节
fd.seek(10, io.SEEK_SET)


# 判断fd流是否支持随机访问
if(fd.seekable()):
	print("supported for random access")

# 判断fd流是否可读
if(fd.readable()):
	print("ready to read")

# 判断fd流是否可写
if(fd.writable()):
	print("ready to write")

fd.close()



fd = open('./py_io.py','rt')
# 读取size 字节数据, -1=读取全部
fd.read(size=-1)

# 逐行读取file [消耗内存较少, 但性能较差]
line=fd.readline()
while line:
	#print(line)
	line=fd.readline()

# 逐行读取file [实现细节不明确, 慎用, 但精简]
#for line in open('./py_io.py','rt'):
	#print(line)

# pos 复位
fd.seek(0, io.SEEK_SET)
# 读取file 所有数据, 返回一个list
plist = fd.readlines()
for line in plist:
	print(line)


fd.close()





# 写数据到file
fd = open('./tmp4test','wt')
text="hello world asshole"
plist=["apple\n","orange\n","banana\n","watermelon\n"]

# 写入一个string
fd.write(text)
# 写入一个string list (需要自己添加换行符)
fd.writelines(plist)
# 截断文件
fd.truncate(32)

fd.close()

# 验证读写后的数据, 全部读出回显
fd = open('./tmp4test','rt')
plist = fd.readlines()
for line in plist:
	print(line)
fd.close()


