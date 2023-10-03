#coding=UTF-8

import sqlite3


# 打开数据库
conn = sqlite3.connect('test.db')

# 获取该数据库的游标
cursor = conn.cursor()

# 执行单条数据插入, 并返回操作行数
singleInsert = cursor.execute("INSERT INTO User (Name, Age) VALUES ('张三', 18)")
print(f"插入了{singleInsert.rowcount}条数据")

# 执行批量插入, 并返回操作行数
sql = "INSERT INTO User (Name, Age) VALUES (?, ?)"
batchInsert = cursor.executemany(sql, [('李四', 19), ('王五', 22)])
print(f"批量插入了{batchInsert.rowcount}条数据")

# 关闭Cursor:
cursor.close()

# 提交事务:
conn.commit()

# 关闭Connection:
conn.close()
