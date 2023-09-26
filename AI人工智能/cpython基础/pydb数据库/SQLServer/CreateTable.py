#coding=utf8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456', database='TestDB')

# 创建一个游标对象
cursor = db.cursor()

# 查看现有表
sql = "SELECT NAME FROM TESTDB.SYS.TABLES"
msg = cursor.execute(sql)
print(cursor.fetchall())

# 创建Person表
sql = """
USE TestDB
CREATE TABLE [Persons](
ID INT PRIMARY KEY IDENTITY(1, 1),
Name VARCHAR(50),
Age INT,
)
"""
cursor.execute(sql)

# 查看现有表
sql = "SELECT NAME FROM TESTDB.SYS.TABLES"
cursor.execute(sql)
print(cursor.fetchall())

# 提交事务
db.commit()

# 关闭游标与数据库连接
cursor.close()
db.close()
