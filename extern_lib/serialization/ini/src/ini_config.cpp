//编译:
//		g++ ./simpleini/ConvertUTF.h ./simpleini/ConvertUTF.c ./simpleini/SimpleIni.h ini_config.h ini_config.cpp -g3 -o x 



//ini 仍然存在的问题list:
/*
* UTF-8 问题, 也没有统一, 没有做好兼容[done]

* val_count没有算对(没意义, simpleini 不支持一键多值)[done]

* 不兼容一键多值(可以自己实现, 参考: split字符串切割demo) [这个你得自己设置分割符号, 自己切割]

*/


//ini config 函数重定义(敏捷开发)
#include "ini_config.h"
//ini 文件对接的数据结构定义
#include "config_test.h"
//ini 文件path
#define FILEPATH_INI "./config_test.ini"

//定义一个全局变量, 用来装载ini 文件中存放的数据
struct G_val_t g_val;



bool ini_config_read_from_file(G_val_t* pg_val){
	CSimpleIniA m_ini(true, true, true);
	CSimpleIniA::TNamesDepend sections;																//容器:节点
	CSimpleIniA::TNamesDepend keys;																		//容器:键
	CSimpleIniA::TNamesDepend values;																	//容器:值
	CSimpleIniA::TNamesDepend::const_iterator i;											//节点迭代器
	CSimpleIniA::TNamesDepend::const_iterator k;
	CSimpleIniA::TNamesDepend::const_iterator v;

	if(eopen_inifile(&m_ini, FILEPATH_INI) < 0){
		printf("eopen_inifile(%s) failed!!\n", FILEPATH_INI);
		return false;
	}


	get_all_sections(&m_ini,&sections);																//获取所有的sections
	for(i = sections.begin(); i != sections.end(); ++i){							//遍历所有的sections
		if(strstr(i->pItem,"info_server")!= NULL){											//section: info_server 
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){							//获取该section下所有的key
				for(k = keys.begin(); k != keys.end(); ++k){								//遍历该section所有的key
					if(strstr(k->pItem,"srv_ip")!= NULL){											//key: srv_ip
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){	//获取key下所有的value
							v = values.begin();																		//只取第一个value
							pg_val->srv.srv_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"srv_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv.srv_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"test_count")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv.test_count = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"test_err_max")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv.test_err_max = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"thread_count")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv.pth_io_count = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"logger_path")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv.log_path = v->pItem;
						}
					}
				}
			}
		}


		if(strstr(i->pItem,"info_redis")!= NULL){
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){
				for(k = keys.begin(); k != keys.end(); ++k){
					if(strstr(k->pItem,"redis_ip")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis.redis_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"redis_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis.redis_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"redis_dbkey")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis.redis_dbkey = atoi(v->pItem);
						}
					}
				}
			}
		}


		if(strstr(i->pItem,"info_mysql")!= NULL){
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){
				for(k = keys.begin(); k != keys.end(); ++k){
					if(strstr(k->pItem,"mysql_ip")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql.mysql_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql.mysql_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"mysql_user")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql.mysql_user = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_pw")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql.mysql_pw = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_dbname")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql.mysql_dbname = v->pItem;
						}
					}
				}
			}
		}


		if(strstr(i->pItem,"multiline")!= NULL){
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){
				for(k = keys.begin(); k != keys.end(); ++k){
					if(strstr(k->pItem,"another")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mline.another = v->pItem;
						}
					}
					if(strstr(k->pItem,"city")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mline.city = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"中文")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mline.cn_text = v->pItem;
						}
					}
				}
			}
		}
	}

	//打印统计数据
	print_all(&m_ini);
	return true;
}



int main(void){
	if(!ini_config_read_from_file(&g_val))
		printf("ini_config_read_from_file() failed !!\n");
	return 0;
}
