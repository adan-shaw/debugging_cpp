#coding=UTF-8

import pymysql

# 打开数据库连接
db = pymysql.connect(host='localhost', user='root', password='root', database='test')

# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 查询并循环读取单条数据
cursor.execute("select * from user;")

# 取所有数据
resTuple = cursor.fetchall()
print(type(resTuple))
print('共%d条数据' % len(resTuple))

# 关闭数据库连接
db.close()
