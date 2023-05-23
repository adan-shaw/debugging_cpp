#coding=utf-8

import pymysql

'''删除多条数据'''

# 打开数据库连接
conn = pymysql.connect(host='localhost', user='root', password='root', database='test')
conn.select_db('test')

# 获取游标
cur = conn.cursor()

# 删除前查询所有数据
cur.execute("select * from user;")
print('删除前的数据为:')
for res in cur.fetchall():
	print(res)

print('*' * 40)

# 删除2条数据
sql = "delete from user where id=%s"
cur.executemany(sql, [(3), (4)])

# 删除后查询所有数据
cur.execute("select * from user;")
print('删除后的数据为:')
for res in cur.fetchall():
	print(res)
cur.close()
conn.commit()
conn.close()
print('sql执行成功')
