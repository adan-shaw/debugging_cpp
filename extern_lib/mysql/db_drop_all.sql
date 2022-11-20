-- 切换mysql数据库
use mysql;

-- 删除数据库
drop database if exists db4adan_srv;

-- 删除用户
delete from mysql.user where user="adan" and host="localhost";
