//cjson 简介
/*
	目前cjson 都是DaveGamble 搞出来的, 所以不用怀疑兼容性.
	https://github.com/DaveGamble/cJSON

	头文件路径: 
		$install_path/include/cjson/cJSON.h
		#include <cjson/cJSON.h>

		cjson 是一个纯c 项目, 原本需要使用extern "C"{}, 才能被c++ 兼容;
		但cJSON.h头文件已附带了extern "C"{}, 因此c++不需要再次包含extern "C"{}, 而可以直接使用;

	链接库路径:
		$install_path/lib/*

	cjson 编译选项: -l cjson
*/



//cjson 数据类型: 
#define cJSON_False 0						//false
#define cJSON_True 1						//true
#define cJSON_NULL 2						//null
#define cJSON_Number 3					//整形/浮点
#define cJSON_String 4					//字符串
#define cJSON_Array 5						//数组(数字数组/字符串数组)
#define cJSON_Object 6					//cjson 根
#define cJSON_IsReference 256		//判断当前变量是否为引用(c语言中符号&)
#define cJSON_StringIsConst 512	//判断字符串是否为常量字符串

//ps: 
//	cjson 是一个malloc 创建的数据链表, 必然性出发ptmalloc 内存管理器的重用, 
//	频繁使用cjson, 最好更换一下进程内存管理器, jemalloc;

//cjson 节点结构体: 
typedef struct cJSON {
  struct cJSON *next,*prev;  //next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem 
  struct cJSON *child;       //An array or object item will have a child pointer pointing to a chain of the items in the array/object;
  int type;                  //The type of the item, as above;
  char *valuestring;         //The item's string, if type==cJSON_String 
  int valueint;              //The item's number, if type==cJSON_Number 
  double valuedouble;        //The item's number, if type==cJSON_Number 
  char *string;              //The item's name string, if this item is the child of, or is in the list of subitems of an object;
} cJSON;

//cjson 节点创建时使用的内存申请API 函数指针(malloc/free/realloc), 可更换为jemalloc 的API
typedef struct cJSON_Hooks {
  void *(*malloc_fn)(size_t sz);
  void (*free_fn)(void *ptr);
} cJSON_Hooks;



//快速函数宏(Macros for creating things quickly), 没有性能优化, 但可以减少代码行数;
#define cJSON_AddNullToObject(object,name)  cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define cJSON_AddTrueToObject(object,name)  cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object,name)  cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define cJSON_AddBoolToObject(object,name,b)  cJSON_AddItemToObject(object, name, cJSON_CreateBool(b))
#define cJSON_AddNumberToObject(object,name,n)  cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object,name,s)  cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

//When assigning an integer value, it needs to be propagated to valuedouble too
#define cJSON_SetIntValue(object,val)  ((object)?(object)->valueint=(object)->valuedouble=(val):(val))
#define cJSON_SetNumberValue(object,val)  ((object)?(object)->valueint=(object)->valuedouble=(val):(val))



//cjson 节点元素类型判断API:
(cJSON_bool) cJSON_IsInvalid(const cJSON* const item);
(cJSON_bool) cJSON_IsFalse(const cJSON* const item);
(cJSON_bool) cJSON_IsTrue(const cJSON* const item);
(cJSON_bool) cJSON_IsBool(const cJSON* const item);
(cJSON_bool) cJSON_IsNull(const cJSON* const item);
(cJSON_bool) cJSON_IsNumber(const cJSON* const item);
(cJSON_bool) cJSON_IsString(const cJSON* const item);
(cJSON_bool) cJSON_IsArray(const cJSON* const item);
(cJSON_bool) cJSON_IsObject(const cJSON* const item);
(cJSON_bool) cJSON_IsRaw(const cJSON* const item);



//
//cjson 函数定义声明(需要-L 添加动态链接库, 需要-l 添加链接库编译选项)
//
//更换cjson 默认使用的malloc/free/realloc, 可以切换为jemalloc;
extern void cJSON_InitHooks(cJSON_Hooks* hooks);

//压缩cjson 字符串(如果手动输入json 报文, 有空格, 则可调用这个函数进行压缩)
extern void cJSON_Minify(char *json);

//解析json 字符串, 得到一个cjson链表的根(解析后会自动调用cJSON_Delete()删除该cjson链表)
extern cJSON *cJSON_Parse(const char *value);

//解析json 字符串, 得到一个cjson链表的根[自动检查版, 发现null元素则自动返回return_parse_end 字符串]
extern cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

//cJSON_Parse() 解析json 字符串失败, 打印错误的原因;
extern const char *cJSON_GetErrorPtr(void);

//将cjson 节点转换为字符串(字符串指针需要手动释放)
extern char  *cJSON_Print(cJSON *item);
//将cjson 链表转换为字符串(字符串指针需要手动释放)
extern char  *cJSON_PrintUnformatted(cJSON *item);
//将cjson 链表转换为字符串(字符串指针需要手动释放), 截断操作prebuffer=截断的最大长度max, fmt=字符串是否需要重新格式化(0=不用格式化,1=需要格式化)
extern char *cJSON_PrintBuffered(cJSON *item,int prebuffer,int fmt);

//删除cjson链表
extern void  cJSON_Delete(cJSON *c);

//根据cjson 数组节点名, 获取'数组节点'的长度
extern int  cJSON_GetArraySize(cJSON *array);
//根据cjson 数组节点名和数组中的元素pos下标, 取出'数组节点'中的元素
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);
//根据cjson节点名, 取出'子节点元素'
extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

extern cJSON *cJSON_CreateNull(void);									//创建null节点
extern cJSON *cJSON_CreateTrue(void);									//创建true节点
extern cJSON *cJSON_CreateFalse(void);								//创建false节点
extern cJSON *cJSON_CreateBool(int b);								//创建bool节点
extern cJSON *cJSON_CreateNumber(double num);					//创建number节点
extern cJSON *cJSON_CreateString(const char *string);	//创建string节点
extern cJSON *cJSON_CreateArray(void);								//创建array节点
extern cJSON *cJSON_CreateObject(void);								//创建子根节点

extern cJSON *cJSON_CreateIntArray(const int *numbers,int count);				//创建int数组节点
extern cJSON *cJSON_CreateFloatArray(const float *numbers,int count);		//创建float数组节点
extern cJSON *cJSON_CreateDoubleArray(const double *numbers,int count);	//创建double数组节点
extern cJSON *cJSON_CreateStringArray(const char **strings,int count);	//创建string数组节点

//附加后, 原cjson 节点会被直接并入cjson 链表
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);											//添加数组节点到'根节点/任意节点'中, 并为该数组节点命名
extern void cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);	//添加子根节点到'根节点/任意节点'中, 并为该子根节点命名
extern void cJSON_AddItemToObjectCS(cJSON *object,const char *string,cJSON *item);//添加子根节点到'根节点/任意节点'中, 并为该子根节点命名(当子根节点为字符串常量时的专用版)

//先拷贝一份原cjson 节点, 再将新的节点拷贝, 并入cjson 链表
extern void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
extern void  cJSON_AddItemReferenceToObject(cJSON *object,const char *string,cJSON *item);

extern cJSON *cJSON_DetachItemFromArray(cJSON *array,int which);						//根据pos下标, 从cjson 数组节点中删除元素
extern void   cJSON_DeleteItemFromArray(cJSON *array,int which);						//根据pos下标, 从cjson 数组节点中删除元素
extern cJSON *cJSON_DetachItemFromObject(cJSON *object,const char *string);	//根据节点名, 从cjson 子根节点中删除元素
extern void   cJSON_DeleteItemFromObject(cJSON *object,const char *string);	//根据节点名, 从cjson 子根节点中删除元素

extern void cJSON_InsertItemInArray(cJSON *array,int which,cJSON *newitem);							//根据pos下标, 向cjson 数组节点插入元素
extern void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);						//根据pos下标, 将cjson 数组节点的元素替换
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);	//根据节点名, 将cjson 子根节点的元素替换

extern cJSON *cJSON_Duplicate(cJSON *item,int recurse);//递归复制n个cjson节点(recurse=n), 复制操作在某些应用场景很有效, 可批量操作和申请连续内存





//
// english version(英文原版说明, 略)
//
//Supply malloc, realloc and free functions to cJSON
extern void cJSON_InitHooks(cJSON_Hooks* hooks);

//Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished;
extern cJSON *cJSON_Parse(const char *value);

//Render a cJSON entity to text for transfer/storage. Free the char* when finished;
extern char  *cJSON_Print(cJSON *item);
//Render a cJSON entity to text for transfer/storage without any formatting. Free the char* when finished;
extern char  *cJSON_PrintUnformatted(cJSON *item);
//Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted 
extern char *cJSON_PrintBuffered(cJSON *item,int prebuffer,int fmt);

//Delete a cJSON entity and all subentities;
extern void   cJSON_Delete(cJSON *c);

//Returns the number of items in an array (or object);
extern int  cJSON_GetArraySize(cJSON *array);
//Retrieve item number "item" from array "array". Returns NULL if unsuccessful;
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);
//Get item "string" from object. Case insensitive;
extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

//For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds;
extern const char *cJSON_GetErrorPtr(void);
  
//These calls create a cJSON item of the appropriate type;
extern cJSON *cJSON_CreateNull(void);
extern cJSON *cJSON_CreateTrue(void);
extern cJSON *cJSON_CreateFalse(void);
extern cJSON *cJSON_CreateBool(int b);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray(void);
extern cJSON *cJSON_CreateObject(void);

//These utilities create an Array of count items;
extern cJSON *cJSON_CreateIntArray(const int *numbers,int count);
extern cJSON *cJSON_CreateFloatArray(const float *numbers,int count);
extern cJSON *cJSON_CreateDoubleArray(const double *numbers,int count);
extern cJSON *cJSON_CreateStringArray(const char **strings,int count);

//Append item to the specified array/object;
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);
extern void  cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);
extern void  cJSON_AddItemToObjectCS(cJSON *object,const char *string,cJSON *item);  //Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object 
//Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON;
extern void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
extern void  cJSON_AddItemReferenceToObject(cJSON *object,const char *string,cJSON *item);

//Remove/Detatch items from Arrays/Objects;
extern cJSON *cJSON_DetachItemFromArray(cJSON *array,int which);
extern void   cJSON_DeleteItemFromArray(cJSON *array,int which);
extern cJSON *cJSON_DetachItemFromObject(cJSON *object,const char *string);
extern void   cJSON_DeleteItemFromObject(cJSON *object,const char *string);
  
//Update array items;
extern void cJSON_InsertItemInArray(cJSON *array,int which,cJSON *newitem);  //Shifts pre-existing items to the right;
extern void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

//Duplicate a cJSON item
/*
	Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
	need to be released. With recurse!=0, it will duplicate any children connected to the item.
	The item->next and ->prev pointers are always zero on return from Duplicate;
*/
extern cJSON *cJSON_Duplicate(cJSON *item,int recurse);

//ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed;
extern cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

extern void cJSON_Minify(char *json);
