#!/bin/sh

:<<!
	touch ./db_drop_all.sql
	touch ./db_drop_db4adan_srv.sql
	touch ./db_init_db4adan_srv.sql
	touch ./db_init.sql
!



# 首次登录, mysqld 没有密码, 直接操作;
# 切换mysql 管理员
#sudo -u mysql

# 以超级权限登录mysql
#mysql

# 快速创建管理员用户adan, 快速创建数据库
#source db_init.sql

# 快速销毁管理员用户adan, 快速销毁数据库
#source db_drop_all.sql





# mysql 远程登录(不能自动输入密码, 需要手动输入密码)
# 	mysql -h 192.168.56.101 -u mysql -p
:<<!
	mysql 远程连接报错:
		Enter password:
		ERROR 2003 (HY000): Can't connect to MySQL server on 'xxx.xxx.xxx.xxx' (111)

	访问权限问题, 解决办法(修改绑定地址为局域网地址):
	#vi /etc/mysql/my.cnf
	vi /etc/mysql/mariadb.conf.d/50-server.cnf
		#bind-address            = 127.0.0.1
		#bind-address            = *
		bind-address            = 192.168.56.101

	systemctl restart mysqld
	systemctl status mysqld
	mysql -h 192.168.56.101 -u adan -p
!



# 快速执行sql 语句的方法:
# 	mysql -u root -p passwd -e "your sql cmd"

# 快速导入mysql 脚本
# source <sql_file>;



mysql_bin="/usr/bin/mysql"
user="adan"
pw="123"
ip_addr="127.0.0.1"

# 快速导入脚本(必须指定密码, 才能执行的):
$mysql_bin -h $ip_addr -u $user -p $pw -e "source ./db_init_db4adan_srv.sql"

#$mysql_bin -h $ip_addr -u $user -p $pw -e "source ./db_drop_db4adan_srv.sql"
