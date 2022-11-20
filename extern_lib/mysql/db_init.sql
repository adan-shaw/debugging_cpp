-- 切换mysql数据库
use mysql;

-- 创建用户 
create user adan identified by '123';

-- 创建 DB
create database db4adan_srv;

-- 授权adan 操作db4adan_srv
grant all privileges on db4adan_srv.* to 'adan'@'localhost' identified by '123';

-- 给用户adan 授权执行存储过程
grant execute on configdb.* to 'adan'@'%' identified by password '123';

-- 授权adan 访问mysql.proc 表
grant all privileges on mysql.proc to adan@localhost identified by '123';
