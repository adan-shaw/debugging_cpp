#coding=utf-8

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

# 修改前的数据
cursor.execute('SELECT * FROM "Person";')
ShowData("修改前的数据:", cursor.fetchall())

# 执行单条数据修改
cursor.execute('UPDATE "Person" SET "Age" = 22 WHERE "Name" = \'Tom\'')

# 修改单条数据后的数据
cursor.execute('SELECT * FROM "Person" ORDER BY "ID";')
ShowData("修改单条数据后的数据:", cursor.fetchall())

# 执行批量修改
sql = 'UPDATE "Person" SET "Age" = %s WHERE "Name" = %s'
batchUpdate = cursor.executemany(sql, [(25, 'Tim'), (18, 'Jane')])

# 批量修改后的数据
cursor.execute('SELECT * FROM "Person" ORDER BY "ID";')
ShowData("批量修改后的数据:", cursor.fetchall())

# 提交事务
db.commit()

# 释放游标及数据库连接
cursor.close()
db.close()
