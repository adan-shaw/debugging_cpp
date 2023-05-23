#coding=utf-8

import sqlite3

# 若无该数据库, 则创建Sqlite数据库并打开
# 若有, 则直接打开数据库
conn = sqlite3.connect('test.db')

# 获取该数据库的游标
cursor = conn.cursor()

# 执行一条SQL语句, 创建user表:
cursor.execute('CREATE TABLE User (ID INTEGER Primary KEY, Name TEXT, Age INTEGER)')

# 关闭Cursor:
cursor.close()

# 提交事务:
conn.commit()

# 关闭Connection:
conn.close()
