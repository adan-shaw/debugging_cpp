//编译:
//		g++ SimpleIni.h ini_config.h ini_config.cpp -g3 -o x 



#include "ini_config.h"



bool ini_config_read_from_file(G_val_t* pg_val){
	CSimpleIniA m_ini(true, true, true);
	CSimpleIniA::TNamesDepend sections;																//容器:节点
	CSimpleIniA::TNamesDepend keys;																		//容器:键
	CSimpleIniA::TNamesDepend values;																	//容器:值
	CSimpleIniA::TNamesDepend::const_iterator i;											//节点迭代器
	CSimpleIniA::TNamesDepend::const_iterator k;
	CSimpleIniA::TNamesDepend::const_iterator v;

	if(eopen_inifile(&m_ini, "./config_test.ini") < 0){
		printf("myIni::eopen_inifile(%d) failed\n", "./target_data_demo.ini");
		return false;
	}


	get_all_sections(&m_ini,&sections);																//获取所有的sections
	for(i = sections.begin(); i != sections.end(); ++i){							//遍历所有的sections
		if(strstr(i->pItem,"server_info")!= NULL){											//section: server_info 
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){							//获取该section下所有的key
				for(k = keys.begin(); k != keys.end(); ++k){								//遍历该section所有的key
					if(strstr(k->pItem,"srv_ip")!= NULL){											//key: srv_ip
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){	//获取key下所有的value
							v = values.begin();																		//只取第一个value
							pg_val->srv_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"srv_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->srv_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"test_count")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->test_count = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"test_err_max")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->test_err_max = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"thread_count")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->pth_io_count = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"logger_path")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->log_path = v->pItem;
						}
					}
				}
			}
		}


		if(strstr(i->pItem,"redis_info")!= NULL){
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){
				for(k = keys.begin(); k != keys.end(); ++k){
					if(strstr(k->pItem,"redis_ip")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"redis_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"redis_dbkey")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->redis_dbkey = atoi(v->pItem);
						}
					}
				}
			}
		}


		if(strstr(i->pItem,"mysql_info")!= NULL){
			if(get_section_all_keys(&m_ini,i->pItem,&keys)){
				for(k = keys.begin(); k != keys.end(); ++k){
					if(strstr(k->pItem,"mysql_ip")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql_ip = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_port")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql_port = atoi(v->pItem);
						}
					}
					if(strstr(k->pItem,"mysql_user")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql_user = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_pw")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql_pw = v->pItem;
						}
					}
					if(strstr(k->pItem,"mysql_dbname")!= NULL){
						if(get_key_all_val(&m_ini,i->pItem,k->pItem,&values)){
							v = values.begin();
							pg_val->mysql_dbname = v->pItem;
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
	G_val_t g_val;
	if(!ini_config_read_from_file(&g_val))
		printf("ini_config_read_from_file() failed !!\n");
	return 0;
}
