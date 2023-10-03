#coding=UTF-8

import sqlite3


# 打开数据库
conn = sqlite3.connect('test.db')

# 获取该数据库的游标
cursor = conn.cursor()

cursor.execute("SELECT * FROM User")
print("修改前的数据:")
print(cursor.fetchall())

# 执行单条数据修改, 并返回操作行数
singleUpdate = cursor.execute("UPDATE User SET Age = 20 WHERE Name = '张三'")
print(f"修改了{singleUpdate.rowcount}条数据")

cursor.execute("SELECT * FROM User")
print("修改单条数据后的数据:")
print(cursor.fetchall())

# 执行批量修改, 并返回操作行数
sql = "UPDATE User SET Age = ? WHERE Name = ?"
batchUpdate = cursor.executemany(sql, [(25, '李四'), (35, '王五')])
print(f"批量修改了{batchUpdate.rowcount}条数据")

cursor.execute("SELECT * FROM User")
print("批量修改后的的数据:")
print(cursor.fetchall())

# 关闭Cursor:
cursor.close()

# 提交事务:
conn.commit()

# 关闭Connection:
conn.close()
