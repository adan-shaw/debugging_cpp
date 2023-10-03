#coding=UTF-8

import psycopg2


# 打印数据函数
def ShowData(str, data):
	arr = []
	for item in data:
		arr.append(item)
	print(str, arr)


# 打开数据库连接
db = psycopg2.connect(host='192.168.64.132',
  port='5432',
  user='root',
  password='123456')

# 设置数据库连接打开自动提交模式
db.autocommit = True

# 创建游标对象
cursor = db.cursor()

# 查看现有数据库
sql = 'SELECT datname FROM pg_database;'
cursor.execute(sql)
ShowData("现有数据库:", cursor.fetchall())

# 创建test数据库
sql = "CREATE DATABASE test;"
cursor.execute(sql)

# 查看新数据库创建完成后的数据库列表
sql = 'SELECT datname FROM pg_database;'
cursor.execute(sql)
ShowData("创建完成后的数据库:", cursor.fetchall())

# 释放游标及数据库连接
cursor.close()
db.close()
