#coding=utf-8

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

# 获取全部记录
cursor.execute('SELECT * FROM "Person";')
ShowData("直接获取全部记录:", cursor.fetchall())

# 获取前N条记录
cursor.execute('SELECT * FROM "Person";')
ShowData("获取部分结果:", cursor.fetchmany(2))

# 一次读取一条结果, 循环获取所有记录
cursor.execute('SELECT * FROM "Person";')
print("一次读取一条结果, 循环获取所有记录:")
while True:
	singleData = cursor.fetchone()
	if singleData is None:
		break
	print(singleData)

# 提交事务
db.commit()

# 释放游标及数据库连接
cursor.close()
db.close()
