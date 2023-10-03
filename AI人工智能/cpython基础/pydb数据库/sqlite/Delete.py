#coding=UTF-8

import sqlite3


# 打开数据库
conn = sqlite3.connect('test.db')

# 获取该数据库的游标
cursor = conn.cursor()

cursor.execute("SELECT * FROM User")
print("删除前的数据:")
print(cursor.fetchall())

# 执行单条数据修改, 并返回操作行数
singleDelete = cursor.execute("DELETE FROM User WHERE ID = 1")
print(f"删除了{singleDelete.rowcount}条数据")

cursor.execute("SELECT * FROM User")
print("删除单条数据后的数据:")
print(cursor.fetchall())

# 执行批量删除, 并返回操作行数
sql = "DELETE FROM User WHERE ID = ?"
batchDelete = cursor.executemany(sql, [('2'), ('3')])
print(f"批量删除了{batchDelete.rowcount}条数据")

cursor.execute("SELECT * FROM User")
print("批量删除后的的数据:")
print(cursor.fetchall())

# 关闭Cursor:
cursor.close()

# 提交事务:
conn.commit()

# 关闭Connection:
conn.close()
