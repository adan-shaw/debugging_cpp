//编译:
//		gcc mysql_demo.c -g3 -lmysqlclient -o x 



//需要先开启局域网访问: vi /etc/mysql/mariadb.conf.d/50-server.cnf
//			#bind-address            = 127.0.0.1
//			bind-address            = *

//			systemctl restart mysqld
//			systemctl status mysqld
//			# 不能自动输入密码, 需要手动输入密码
//			mysql -h 192.168.56.101 -u adan -p



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>



//mysql server info
const char* g_host_name = "192.168.56.101";
const char* g_user_name = "adan";
const char* g_password = "123";
const char* g_db_name = "db4adan_srv";

const char* sql_test = "select * from db4adan_srv";



int main(void){
	int tmp;

	//初始化mysql 链接引擎(不需要填写任何信息)
	MYSQL *mysql_conn = mysql_init(NULL);
	if(NULL == mysql_conn){
		printf("mysql_init() failed\n");
		return -1;
	}

	//连接mysql数据库
	mysql_conn = mysql_real_connect(mysql_conn, g_host_name, g_user_name, g_password, g_db_name, 0, NULL, 0);
	if(NULL == mysql_conn){
		printf("mysql_real_connect() failed\n");
		return -1;
	}

	//执行SQL语句
	tmp = mysql_query(mysql_conn, sql_test);
	if(0 != tmp){
		printf("mysql_query() failed\n");
		return -1;
	}

	//关闭连接
	mysql_close(mysql_conn);
	return 0;
}
