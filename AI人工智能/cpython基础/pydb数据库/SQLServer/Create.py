#coding=UTF-8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456', database='TestDB')

# 创建游标对象,并设置返回数据的类型为字典
cursor = db.cursor(as_dict=True)

# 插入单条数据
cursor.execute("INSERT INTO Persons(Name, Age) VALUES ('张三',18 )")

print("插入单条数据后的数据")
cursor.execute("SELECT * FROM Persons")
for item in cursor.fetchall():
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 批量插入数据
sql = "INSERT INTO Persons(Name, Age) VALUES (%s,%s)"
cursor.executemany(sql, [('李四', 20), ('王五', 10)])

print("批量插入后的数据")
cursor.execute("SELECT * FROM Persons")
for item in cursor.fetchall():
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 提交事务
db.commit()

# 关闭游标与数据库连接
cursor.close()
db.close()
