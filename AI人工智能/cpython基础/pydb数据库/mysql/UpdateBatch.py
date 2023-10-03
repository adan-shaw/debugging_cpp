'''更新多条数据'''
#coding=UTF-8

import pymysql
# 打开数据库连接
conn = pymysql.connect(host='localhost', user='root', password='root', database='test')
conn.select_db('test')
# 获取游标
cur = conn.cursor()

# 更新前查询所有数据
cur.execute("select * from user where name in ('kongsh','wen');")
print('更新前的数据为:')
for res in cur.fetchall():
	print(res)

print('*' * 40)
# 更新2条数据
sql = "update user set age=%s where name=%s"
update = cur.executemany(sql, [(15, 'kongsh'), (18, 'wen')])

# 更新2条数据后查询所有数据
cur.execute("select * from user where name in ('kongsh','wen');")
print('更新后的数据为:')
for res in cur.fetchall():
	print(res)

cur.close()
conn.commit()
conn.close()
print('sql执行成功')
