#coding=UTF-8

import psycopg2


# 打印数据函数
def ShowData(str, data):
	arr = []
	for item in data:
		arr.append(item)
	print(str, "\n", arr)


# 打开数据库连接
db = psycopg2.connect(host='192.168.64.132', port='5432', user='root', password='123456', database='test')

# 创建游标对象
cursor = db.cursor()

retrieveSql = 'SELECT * FROM "Person" ORDER BY "ID";'

# 删除前的数据
cursor.execute(retrieveSql)
ShowData("删除前的数据:", cursor.fetchall())

# 执行单条数据删除
cursor.execute('DELETE FROM "Person" WHERE "ID" = 1;')

# 删除单条数据后的数据
cursor.execute(retrieveSql)
ShowData("删除单条数据后的数据:", cursor.fetchall())

# 执行批量删除
sql = 'DELETE FROM "Person" WHERE "ID" = %s;'
cursor.executemany(sql, [('2'), ('3')])

# 批量删除后的的数据
cursor.execute(retrieveSql)
ShowData("删除单条数据后的数据:", cursor.fetchall())

# 清空表, 并重置自增主键计数
sql = 'TRUNCATE TABLE "Person" RESTART IDENTITY'
cursor.execute(sql)

# 提交事务
db.commit()

# 释放游标及数据库连接
cursor.close()
db.close()
