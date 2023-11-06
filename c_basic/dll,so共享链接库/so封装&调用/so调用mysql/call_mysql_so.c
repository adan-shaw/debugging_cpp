//编译:
//		gcc -g3 -ldl ./call_mysql_so.c -o x 



//安装mariadb/mysql client 引擎:
//		apt-get install mariadb-client



#include <stdio.h>
#include "call_mysql_so.h"
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
void init_so_API(void* pso, API_list* papi){
	const char* perr_so = NULL;

	//dlerror();
	papi->mysql_error = (const char* (*)(MYSQL *mysql))dlsym(pso, "mysql_error");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_init = (MYSQL* (*)(MYSQL *mysql))dlsym(pso, "mysql_init");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_real_connect = (MYSQL* (*)(MYSQL *mysql, const char *host,const char *user,const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long clientflag))dlsym(pso, "mysql_real_connect");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_real_query = (int (*)(MYSQL *mysql, const char *q,unsigned long length))dlsym(pso, "mysql_real_query");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_close = (void (*)(MYSQL *sock))dlsym(pso, "mysql_close");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_store_result = (MYSQL_RES* (*)(MYSQL *mysql))dlsym(pso, "mysql_store_result");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_use_result = (MYSQL_RES* (*)(MYSQL *mysql))dlsym(pso, "mysql_use_result");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_free_result = (void (*)(MYSQL_RES *result))dlsym(pso, "mysql_free_result");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_num_rows = (my_ulonglong (*)(MYSQL_RES *res))dlsym(pso, "mysql_num_rows");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_num_fields = (unsigned int (*)(MYSQL_RES *res))dlsym(pso, "mysql_num_fields");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_fetch_row = (MYSQL_ROW (*)(MYSQL_RES *result))dlsym(pso, "mysql_fetch_row");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_row_seek = (MYSQL_ROW_OFFSET (*)(MYSQL_RES *result,MYSQL_ROW_OFFSET offset))dlsym(pso, "mysql_row_seek");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	//dlerror();
	papi->mysql_next_result = (int (*)(MYSQL *mysql))dlsym(pso, "mysql_next_result");
	if((perr_so = dlerror()) != NULL){printf("dlsym() failed: %s\n", perr_so);}

	return;
}



int main(void){
	API_list mysql_api;
	void* pvoid = NULL;

	if(!open_soEx(pvoid, path_mysql_so)){
		perror("open_soEx()");
		return -1;
	}

	init_so_API(pvoid, &mysql_api);

	close_soEx(pvoid);
	return 0;
}


