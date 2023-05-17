-- 切换db4adan_srv数据库
use db4adan_srv;

-- 删除表
drop table if exists db4adan_srv.uinfo;
drop table if exists db4adan_srv.upicture;

-- 删除存储过程 -- 不要直接洗表, mysql.proc 或许有其它 procedure
drop procedure if exists `uinfo_clean`;
drop procedure if exists `uinfo_add`;
drop procedure if exists `uinfo_select_by_uid`;
drop procedure if exists `uinfo_change`;

drop procedure if exists `upicture_clean`;
drop procedure if exists `upicture_add`;
drop procedure if exists `upicture_select_by_uid`;
drop procedure if exists `upicture_change`;

-- 最后显示proc 有没有删干净
select name from mysql.proc;
