#coding=UTF-8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456', database='TestDB')

# 创建游标对象,并设置返回数据的类型为字典
cursor = db.cursor(as_dict=True)

# 获取全部记录
cursor.execute("SELECT * FROM Persons")
allData = cursor.fetchall()
print("直接获取全部记录:")
for item in allData:
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 获取前N条记录
cursor.execute("SELECT * FROM Persons")
manyData = cursor.fetchmany(2)
print("获取部分结果:")
for item in manyData:
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 一次读取一条结果, 循环获取所有记录
cursor.execute("SELECT * FROM Persons")
print("一次读取一条结果, 循环获取所有记录:")
while True:
	singleData = cursor.fetchone()
	if singleData is None:
		break
	singleData["Name"] = singleData["Name"].encode("latin1").decode("gbk")
	print(singleData)

# 关闭游标
cursor.close()

# 提交事务
db.commit()

# 关闭数据库连接
db.close()
