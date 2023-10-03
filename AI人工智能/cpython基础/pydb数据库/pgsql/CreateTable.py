#coding=UTF-8

import psycopg2


# 打印数据函数
def ShowData(str, data):
	arr = []
	for item in data:
		arr.append(item)
	print(str, arr)


# 打开数据库连接
db = psycopg2.connect(host='192.168.64.132', port='5432', user='root', password='123456', database='test')

# 创建游标对象
cursor = db.cursor()

# 查看现有表
sql = "SELECT tablename FROM PG_TABLES WHERE SCHEMANAME = 'public';"
cursor.execute(sql)
ShowData("现有表:", cursor.fetchall())

# 创建Person表
sql = """CREATE TABLE "Person"(
"ID" SERIAL PRIMARY KEY NOT NULL,
"Name" TEXT,
"Age" INT)
"""
cursor.execute(sql)

# 查看新数据库创建完成后的数据库列表
sql = "SELECT tablename FROM PG_TABLES WHERE SCHEMANAME = 'public';"
cursor.execute(sql)
ShowData("创建完成后的表:", cursor.fetchall())

# 提交事务
db.commit()

# 释放游标及数据库连接
cursor.close()
db.close()
