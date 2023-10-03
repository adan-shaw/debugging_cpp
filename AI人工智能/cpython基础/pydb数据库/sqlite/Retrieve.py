#coding=UTF-8

import sqlite3

# 打开数据库
conn = sqlite3.connect('test.db')

# 获取该数据库的游标
cursor = conn.cursor()

# 获取全部记录
cursor.execute("SELECT * FROM User")
allData = cursor.fetchall()
print("直接获取全部记录:")
for item in allData:
	print(item)

# 获取前N条记录
cursor.execute("SELECT * FROM User")
manyData = cursor.fetchmany(2)
print("获取部分结果:")
for item in manyData:
	print(item)

# 一次读取一条结果, 循环获取所有记录
cursor.execute("SELECT * FROM User")
print("一次读取一条结果, 循环获取所有记录:")
while True:
	singleData = cursor.fetchone()
	if singleData is None:
		break
	print(singleData)

# 关闭Cursor:
cursor.close()

# 提交事务:
conn.commit()

# 关闭Connection:
conn.close()
