#coding=utf8

import pymysql

# 打开数据库连接
db = pymysql.connect(host='localhost', user='root', password='root', database='test')

# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 增加单条数据
sql = "INSERT INTO user (Name,Age) VALUES ('Tom',18)"
# 使用 execute()  方法执行 SQL 查询
cursor.execute(sql)

# 关闭数据库连接
db.close()
