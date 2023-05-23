#coding=utf-8

import pymysql

# 打开数据库连接
conn = pymysql.connect(host='localhost', user='root', password='root', database='test')
conn.select_db('test')

# 获取游标
cur = conn.cursor()

# 修改前查询所有数据
cur.execute("select * from user;")
print('修改前的数据为:')
for res in cur.fetchall():
	print(res)

print('*' * 40)
# 更新表中第1条数据
cur.execute("update user set name='aaa'")

# 修改后查询所有数据
cur.execute("select * from user;")
print('修改后的数据为:')
for res in cur.fetchall():
	print(res)
print('*' * 40)

# 回滚事务
conn.rollback()
cur.execute("select * from user;")
print('回滚事务后的数据为:')
for res in cur.fetchall():
	print(res)

cur.close()

conn.close()
print('sql执行成功')
