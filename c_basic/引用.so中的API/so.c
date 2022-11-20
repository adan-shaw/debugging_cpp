//编译:
//		gcc -g3 -ldl ./so.c -o x 



//安装mariadb/mysql client
//		apt-get install mariadb-client



#include <stdio.h>
#include "so.h"
#include <mysql/mysql.h>



//函数指针需要实话, 不能只声明函数指针的类型
typedef struct {
	const char* (*mysql_error)(MYSQL *mysql);
	MYSQL* (*mysql_init)(MYSQL *mysql);
	MYSQL* (*mysql_real_connect)(MYSQL *mysql, const char *host,const char *user,const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long clientflag);
	int (*mysql_real_query)(MYSQL *mysql, const char *q,unsigned long length);
	int (*mysql_query)(MYSQL*connection,const char*query);
	void (*mysql_close)(MYSQL *sock);
	MYSQL_RES* (*mysql_store_result)(MYSQL *mysql);
	MYSQL_RES* (*mysql_use_result)(MYSQL *mysql);
	void (*mysql_free_result)(MYSQL_RES *result);
	my_ulonglong (*mysql_num_rows)(MYSQL_RES *res);
	unsigned int (*mysql_num_fields)(MYSQL_RES *res);
	MYSQL_ROW (*mysql_fetch_row)(MYSQL_RES *result);
	MYSQL_ROW_OFFSET (*mysql_row_seek)(MYSQL_RES *result,MYSQL_ROW_OFFSET offset);
	int (*mysql_next_result)(MYSQL *mysql);
} API_list;



//so 共享链接库的路径
const char* path_mysql_so = "/usr/lib/x86_64-linux-gnu/libmysqlclient.so";



//从已装载的so 库中, 提取API(init .so共享链接库)
int init_so_API(void* pso, API_list* pmysql_api){
	const char* perr_so = NULL;

	dlerror();						//每次操作.so 共享链接库前, 都要清除一下错误记录器;
	pmysql_api->mysql_error = (const char* (*)(MYSQL *mysql))dlsym(pso, "mysql_error");//装载一个API
	perr_so = dlerror();	//查看错误字符串
	if(perr_so != NULL) {printf("dlsym() mysql_error fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_init = (MYSQL* (*)(MYSQL *mysql))dlsym(pso, "mysql_init");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_init fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_real_connect = (MYSQL* (*)(MYSQL *mysql, const char *host,const char *user,const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long clientflag))dlsym(pso, "mysql_real_connect");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_real_connect fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_real_query = (int (*)(MYSQL *mysql, const char *q,unsigned long length))dlsym(pso, "mysql_real_query");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_real_query fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_close = (void (*)(MYSQL *sock))dlsym(pso, "mysql_close");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_close fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_store_result = (MYSQL_RES* (*)(MYSQL *mysql))dlsym(pso, "mysql_store_result");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_store_result fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_use_result = (MYSQL_RES* (*)(MYSQL *mysql))dlsym(pso, "mysql_use_result");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_use_result fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_free_result = (void (*)(MYSQL_RES *result))dlsym(pso, "mysql_free_result");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_free_result fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_num_rows = (my_ulonglong (*)(MYSQL_RES *res))dlsym(pso, "mysql_num_rows");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_num_rows fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_num_fields = (unsigned int (*)(MYSQL_RES *res))dlsym(pso, "mysql_num_fields");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_num_fields fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_fetch_row = (MYSQL_ROW (*)(MYSQL_RES *result))dlsym(pso, "mysql_fetch_row");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_fetch_row fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_row_seek = (MYSQL_ROW_OFFSET (*)(MYSQL_RES *result,MYSQL_ROW_OFFSET offset))dlsym(pso, "mysql_row_seek");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_row_seek fail: %s", perr_so);return -1;}

	dlerror();
	pmysql_api->mysql_next_result = (int (*)(MYSQL *mysql))dlsym(pso, "mysql_next_result");
	perr_so = dlerror();
	if(perr_so != NULL) {printf("dlsym() mysql_next_result fail: %s", perr_so);return -1;}

	return 0;
}



int main(void){
	API_list mysql_api;

	void* pso = open_soEx(path_mysql_so);
	if(pso == NULL){
		perror("init_soEx()");
		return -1;
	}

	if(init_so_API(pso, &mysql_api) == -1){
		printf("init_so_API() failed\n");
		return -1;
	}

	close_soEx(pso);
	return 0;
}

