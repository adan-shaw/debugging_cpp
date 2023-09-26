#coding=utf8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456', database='TestDB')

# 创建游标对象,并设置返回数据的类型为字典
cursor = db.cursor(as_dict=True)

print("删除前的数据:")
cursor.execute("SELECT * FROM Persons")
data = cursor.fetchall()
if len(data) == 0:
	print(data)
else:
	for item in data:
		item["Name"] = item["Name"].encode("latin1").decode("gbk")
		print(item)

# 执行单条数据删除
cursor.execute("DELETE FROM Persons WHERE ID = 1")

print("删除单条数据后的数据:")
cursor.execute("SELECT * FROM Persons")
data = cursor.fetchall()
if len(data) == 0:
	print(data)
else:
	for item in data:
		item["Name"] = item["Name"].encode("latin1").decode("gbk")
		print(item)

# 执行批量删除
sql = "DELETE FROM Persons WHERE ID = %s"
cursor.executemany(sql, [('2'), ('3')])

print("批量删除后的的数据:")
cursor.execute("SELECT * FROM Persons")
data = cursor.fetchall()
if len(data) == 0:
	print(data)
else:
	for item in data:
		item["Name"] = item["Name"].encode("latin1").decode("gbk")
		print(item)

# 关闭游标
cursor.close()

# 提交事务
db.commit()

# 关闭数据库连接
db.close()
