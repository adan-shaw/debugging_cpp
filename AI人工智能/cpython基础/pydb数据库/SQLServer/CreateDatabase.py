#coding=utf-8

import pymssql

# 打开数据库连接
db = pymssql.connect(server='localhost', user='test', password='123456')

# 创建游标对象,并设置返回数据的类型为字典
cursor = db.cursor(as_dict=True)

# 设置立即操作
db.autocommit(True)

# 查看现有数据库
sql = "SELECT * FROM SYSDATABASES"
cursor.execute(sql)
print("现有数据库:")
while True:
	msg = cursor.fetchone()
	if msg is None:
		break
	print(msg['name'])

# 创建数据库
sql = '''
IF NOT EXISTS(SELECT * FROM SYSDATABASES WHERE NAME = 'TestDB')
CREATE DATABASE TestDB;
'''
cursor.execute(sql)

# 查看现有数据库
sql = "SELECT * FROM SYSDATABASES"
cursor.execute(sql)
print("创建后的数据库:")
while True:
	msg = cursor.fetchone()
	if msg is None:
		break
	print(msg['name'])

# 关闭游标与数据库连接
cursor.close()
db.close()
