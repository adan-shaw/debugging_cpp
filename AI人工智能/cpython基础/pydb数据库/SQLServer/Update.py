#coding=utf-8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456', database='TestDB')

# 创建游标对象,并设置返回数据的类型为字典
cursor = db.cursor(as_dict=True)

cursor.execute("SELECT * FROM Persons")
print("修改前的数据:")
cursor.execute("SELECT * FROM Persons")
for item in cursor.fetchall():
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 执行单条数据修改
cursor.execute("UPDATE Persons SET Age = 20 WHERE Name = '张三'")

print("修改单条数据后的数据:")
cursor.execute("SELECT * FROM Persons")
for item in cursor.fetchall():
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 执行批量修改
sql = "UPDATE Persons SET Age = %s WHERE Name = %s"
batchUpdate = cursor.executemany(sql, [(25, '李四'), (35, '王五')])

print("批量修改后的数据:")
cursor.execute("SELECT * FROM Persons")
for item in cursor.fetchall():
	item["Name"] = item["Name"].encode("latin1").decode("gbk")
	print(item)

# 关闭游标
cursor.close()

# 提交事务
db.commit()

# 关闭数据库连接
db.close()
