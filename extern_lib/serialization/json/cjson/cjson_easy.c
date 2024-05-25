//编译:
//		gcc ./cjson_easy.c -g3 -I /home/tarball/cjson/include/ -L /home/tarball/cjson/lib/ -l cjson -o x 

//运行:
//		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/cjson/lib/" ./x

//debug:
//		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/cjson/lib/" gdb ./x



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>



#define iArr_MAX (16)
#define cStr_MAX (256)



typedef struct {
	char is_working;
	int iNum;
	double dSum;
	char cStr[cStr_MAX];
	int iArr[iArr_MAX];
}basic_t;



void basic_test(void){
	basic_t basic;
	int tmp, arr_len;
	char* pstr_json;
	cJSON *pRoot,*ptmp;

	basic.iNum = 999;
	basic.is_working = 1;
	basic.dSum = 399.33;
	strncpy(basic.cStr,"baby you light up my world like nobody else!!",cStr_MAX);
	memset(basic.iArr, 99, iArr_MAX);

	pRoot = cJSON_CreateObject();																//创建cjson根节点
	cJSON_AddBoolToObject(pRoot,"is_working",basic.is_working);	//添加bool 节点
	cJSON_AddNumberToObject(pRoot, "iNum", basic.iNum);					//添加number(整形) 节点
	cJSON_AddNumberToObject(pRoot, "dSum", basic.dSum);					//添加number(整形) 节点
	cJSON_AddStringToObject(pRoot, "cStr", basic.cStr);					//添加string 节点

	ptmp = cJSON_CreateIntArray(basic.iArr,iArr_MAX);						//创建int数组 节点(会自动根据数组进行赋值)
	cJSON_AddItemToObject(pRoot,"iArr",ptmp);										//添加int数组 节点

	pstr_json = cJSON_PrintUnformatted(pRoot);									//导出json字符串

	cJSON_Delete(pRoot);																				//释放cJSON 树
	pRoot = NULL;

	printf("打印json 字符串:\n%s\n\n",pstr_json);
	//free(pstr_json);


	//篡改basic_t x; 原来的值, 看pstr_json 字符串, 是否能将原来的值还原; 如果还原成功, 说明pstr_json 字符串解析成功;
	basic.iNum = 111;
	basic.is_working = 0;
	basic.dSum = 888.22;
	strncpy(basic.cStr,"loveed",cStr_MAX);
	memset(basic.iArr, 11, iArr_MAX);


	pRoot = cJSON_Parse(pstr_json);											//解析json 字符串
	if(pRoot == NULL){
		printf("cJSON_Parse() Err:%s\n", cJSON_GetErrorPtr());
		return;
	}
	else{
		ptmp = cJSON_GetObjectItem(pRoot, "is_working");	//解析bool 类型
		basic.is_working = ptmp->valueint;

		ptmp = cJSON_GetObjectItem(pRoot, "iNum");				//解析int 类型
		basic.iNum = ptmp->valueint;

		ptmp = cJSON_GetObjectItem(pRoot, "dSum");				//解析double 类型
		basic.dSum = ptmp->valuedouble;

		ptmp = cJSON_GetObjectItem(pRoot, "cStr");				//解析string 类型
		strncpy(basic.cStr,ptmp->valuestring,cStr_MAX);

		ptmp = cJSON_GetObjectItem(pRoot, "iArr");				//解析int 数组类型
		arr_len = cJSON_GetArraySize(ptmp);
		for(tmp=0; tmp<arr_len; tmp++)
			basic.iArr[tmp] = cJSON_GetArrayItem(ptmp,tmp)->valueint;
	}

	free(pstr_json);
	pstr_json = NULL;
	pstr_json = cJSON_PrintUnformatted(pRoot);					//导出json字符串

	cJSON_Delete(pRoot);																//释放cJSON 树
	pRoot = NULL;

	printf("打印json 字符串2:\n%s\n\n",pstr_json);
	free(pstr_json);
	return;
}



int main(void){
	basic_test();
	return 0;
}



