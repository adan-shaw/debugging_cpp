'''更新单条数据'''
#coding=utf-8

import pymysql

# 打开数据库连接
conn = pymysql.connect(host='localhost', user='root', password='root', database='test')
conn.select_db('test')

# 获取游标
cur = conn.cursor()

# 更新一条数据
update = cur.execute("update user set age=100 where name='kongsh'")
print('修改后受影响的行数为:', update)

# 查询一条数据
cur.execute('select * from user where name="kongsh";')
print(cur.fetchone())
cur.close()
conn.commit()
conn.close()
print('sql执行成功')
