#include "simpleini/SimpleIni.h"
#include <string>



//CSimpleIniA() 构造函数初始化说明:
/*
	根据文件路径装载一个ini file
	CSimpleIniA(bool a_bIsUtf8, bool a_bUseMultiKey, bool a_bUseMultiLine)
	参数说明:
		a_bIsUtf8:				文件编码格式(一般是utf-8)
		a_bUseMultiKey:		表示是否存在多键的情况?默认为true
		a_bUseMultiLine:	表示是否存在多行的情况?默认为true

	注意:
		初始化时, CSimpleIniA() 是空类, 未载入任何文件, 所以初始化构造函数一定成功, 除非内存不足;
		CSimpleIniA() 一般都是一次性使用, 不需要用new 这么麻烦, 直接local 局部变量更方便, 一般只装载一次即可;
*/

//出错返回说明:
/*
	返回值不是整形, 不是bool, 而是一个enum 联合体
	enum SI_Error {
		SI_OK       =  0,   //!< No error
		SI_UPDATED  =  1,   //!< An existing value was updated
		SI_INSERTED =  2,   //!< A new value was inserted

		// note: test for any error with (retval < 0)
		SI_FAIL     = -1,   //!< Generic failure
		SI_NOMEM    = -2,   //!< Out of memory error
		SI_FILE     = -3    //!< File error (see errno for detail error)
	};
*/



//装载一个ini file, 从file中读取ini 数据[load from a data file]
inline bool eopen_inifile(CSimpleIniA* pini, const char *file_path){
	return pini->LoadFile(file_path)<0 ? false : true;
}
//从一串string中, 装载ini 数据[load from a string](文件中不能有'\0'字符串结束符号, 还要预先预读整个文件的内容到string中)
inline bool eopen_inistring(CSimpleIniA* pini, std::string *str_data){
	return pini->LoadData(str_data->c_str(), str_data->size())<0 ? false : true;
}
//将ini数据保存到string缓冲区中[save the ini data to a string]
inline bool save_inidata2string(CSimpleIniA* pini, std::string* strData){
	return pini->Save(*strData)<0 ? false : true;
}
//将ini数据保存到file中[save the ini data to a file]
inline bool save_inidata2file(CSimpleIniA* pini, const char* file_path){
	return pini->SaveFile(file_path)<0 ? false : true;
}


//获取CSimpleIniA中'所有的节点'[get all sections]
inline void get_all_sections(CSimpleIniA* pini, CSimpleIniA::TNamesDepend* psections){
	pini->GetAllSections(*psections);
}
//获取一个节点下'所有的键'[get all keys in a section]
inline bool get_section_all_keys(CSimpleIniA* pini, const char* psection, CSimpleIniA::TNamesDepend* pkeys){
	return pini->GetAllKeys(psection, *pkeys);
}
//获取'一个键下面的一个值'[get the value of a key]
inline bool get_a_key_val(CSimpleIniA* pini, const char* psection, const char* key, char* buf, int buf_len){
	const char* tmp = pini->GetValue(psection, key, NULL);
	if(tmp == NULL)
		return false;
	else{
		strncpy(buf, tmp, buf_len);
		return true;
	}
}
//获取节点下的一个key的所有value(n个值), 用CSimpleIniA::TNamesDepend* pvals值表返回数据[get all values of a key with multiple values]
inline bool get_key_all_val(CSimpleIniA* pini, const char* psection, const char* key, CSimpleIniA::TNamesDepend* pvals){
	if(pini->GetAllValues(psection, key, *pvals)){
		//sort the values into the original load order
		#if defined(_MSC_VER) && _MSC_VER <= 1200
			pvals->sort();//STL of VC6 doesn't allow me to specify my own comparator for list::sort()
		#else
			pvals->sort(CSimpleIniA::Entry::LoadOrder());
		#endif
		return true;
	}
	else
		return false;
}


//添加一个空节点到CSimpleIniA 类中, 没有指定'key键'和'value值', 少用.[adding a new section]
inline bool add_section(CSimpleIniA* pini, const char* psection){
	return pini->SetValue(psection, NULL, NULL)<0 ? false : true;
}
//添加一个新节点到CSimpleIniA 类中, 如果节点不存在, 则自动创建节点, 并填充输入的值[adding a new section]
//(ps: 添加key, 每个key 必然带一个值, 尽量不要为空值)
inline bool add_section_key_val(CSimpleIniA* pini, const char* psection, const char* pkey, const char* pval){
	return pini->SetValue(psection, pkey, pval)<0 ? false : true;
}


//删除一个节点下面的一个key 数据, 的一个value 的数据!
inline bool del_a_val(CSimpleIniA* pini, const char* psection, const char* pkey, const char* pval){
	return pini->DeleteValue(psection, pkey, pval, true);//(第3参数: true=删除整个key, false=删除key的一个val)
}
//删除一个节点下面的一个key 数据
inline bool del_a_key(CSimpleIniA* pini, const char* psection, const char* pkey){
	return pini->Delete(psection, pkey, true);//(第3参数: true=删除整个sec, false=删除sec的一个key)
}
//[删除一个section]deleting an entire section and all keys in it.
inline bool del_a_section(CSimpleIniA* pini, const char* psection){
	return pini->Delete(psection, NULL);
}
//清除所有section
inline void reset_all(CSimpleIniA* pini){
	pini->Reset();
}

//检查
inline bool check_empty(CSimpleIniA* pini){
	return pini->IsEmpty();
}


//遍历打印所有数据
void print_all(CSimpleIniA* pini){
	CSimpleIniA::TNamesDepend sections;					//容器:节点
	CSimpleIniA::TNamesDepend keys;							//容器:键
	CSimpleIniA::TNamesDepend values;						//容器:值
	CSimpleIniA::TNamesDepend::const_iterator i;//节点迭代器
	CSimpleIniA::TNamesDepend::const_iterator k;
	CSimpleIniA::TNamesDepend::const_iterator v;
	int sec_count = 0;
	int key_count = 0;
	//int val_count = 0;//simpleini 不支持一键多值, 这个val_count 统计, 毫无意义

	get_all_sections(pini,&sections);															//获取所有的sections
	for(i = sections.begin(); i != sections.end(); ++i){					//遍历所有的sections
		if(get_section_all_keys(pini,i->pItem,&keys)){							//无论该节点是否与上面的目标节点名匹配, 统一遍历打印'已经存在的节点'
			for(k = keys.begin(); k != keys.end(); ++k){							//遍历所有的key
				printf("* %d [ %s, (", key_count, k->pItem);							//打印key名
				//val_count = 0;
				if(get_key_all_val(pini,i->pItem,k->pItem,&values)){		//获取key下所有的value
					for(v = values.begin(); v != values.end(); ++v){			//遍历所有的value
						printf("%s", v->pItem);														//打印value
						//val_count++;																			//统计value个数
					}
					printf(") ]\n");
				}
				key_count++;																						//统计key个数
			}
		}
		sec_count++;																								//统计section个数
		printf("\n\n");																							//每个节点产生一个换行
	}

	//打印统计数据
	printf("\n\nsec_count = %d,key_count = %d\n",sec_count, key_count);
	return;
}



