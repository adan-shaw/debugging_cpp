#coding=UTF-8

import psycopg2

# 打开数据库连接
db = psycopg2.connect(host='192.168.64.132', port='5432', user='root', password='123456', database='test')

# 创建游标对象
cursor = db.cursor()

# 插入单条数据
sql = 'INSERT INTO "Person" ("Name", "Age") VALUES (\'%s\', %s)' % ("Tom", 18)
cursor.execute(sql)
print(f"成功插入{cursor.rowcount}条数据")

# 批量插入数据
sql = 'INSERT INTO "Person" ("Name", "Age") VALUES (%s, %s);'
cursor.executemany(sql, [('Tim', 20), ('Jane', 10)])
print(f"成功插入{cursor.rowcount}条数据")

# 提交事务
db.commit()

# 释放游标及数据库连接
cursor.close()
db.close()
