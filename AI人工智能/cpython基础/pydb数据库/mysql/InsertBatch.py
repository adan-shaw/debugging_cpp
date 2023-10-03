#coding=UTF-8

import pymysql

# 打开数据库连接
db = pymysql.connect(host='localhost', user='root', password='root', database='test')

# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 增加多条数据
sql = "INSERT INTO user (Name,Age) VALUES (%s,%s)"
insert = cursor.executemany(sql, [('wen', 20), ('tom', 10), ('test', 30)])
print(insert)

db.close()
