//编译:
//		gcc ./json解析from文件.c -g3 -I /home/tarball/cjson/include/ -L /home/tarball/cjson/lib/ -l cjson -o x 

//运行:
//		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/cjson/lib/" ./x

//debug:
//		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/tarball/cjson/lib/" gdb ./x



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>



#define HOST_COUNT (2)
#define GROUP_COUNT (2)
#define ARR_MAX (16)
#define IP_MAX (32)
#define GROUP_NAME_MAX (128)



typedef struct {
	char ip[IP_MAX];
	unsigned short port;
}Host_t;

typedef struct {
	char isWork;
	int iArrNo[ARR_MAX];
	int groupNo;
	char groupName[GROUP_NAME_MAX];
	Host_t host[HOST_COUNT];//内嵌复杂数组(最多两层,尽量不要出现3层内嵌,否则数据结构太复杂,不好处理)
}Group_t;



//初始化测试数据
Group_t g_Groups[GROUP_COUNT]={
	{1,{0,1,2},10,"Group1",{{"10.10.10.101",5060},{"10.10.10.102",5080}}},
	{0,{9,8,7},20,"Group2",{{"20.20.20.201",6060},{"20.20.20.202",6080}}}
};



//快速将整个文件的数据读入内存, 返回一个mmap 映射-字符串指针, 回调返回文件实际总长(可以防止文件中出现'\0'字符)
char* mmap_fileEx_r(const char* filepath, unsigned int* len){
	int fd;
	char* pbuf;

	fd = open(filepath, O_RDONLY);
	if(fd == -1){
		perror("open()");
		return NULL;
	}

	//读取文件长度
	*len = lseek(fd, 0, SEEK_END);

	//建立映射(mmap 为只读时, 推荐使用: MAP_PRIVATE)
	pbuf = (char*)mmap(NULL, *len, PROT_READ, MAP_PRIVATE, fd, 0);
	if(pbuf == MAP_FAILED){
		perror("mmap");
		close(fd);
		return NULL;
	}
	close(fd);

	return pbuf;
}



//将pgroup数组, 转换成'cJSON结构体', 再导出json字符串
void Get_Group_t_Str(Group_t *pgroup, char **pjson){
	cJSON *pRoot,*pNode,*pSon,*pSon_tmp,*ptmp;
	int tmp,pos;

	pRoot = cJSON_CreateObject();
	pNode = cJSON_CreateArray();
	for(pos = 0;pos < GROUP_COUNT;pos++){
		pSon = cJSON_CreateObject();
		cJSON_AddBoolToObject(pSon,"isWork",pgroup[pos].isWork);
		ptmp = cJSON_CreateArray();
		cJSON_AddItemToArray(ptmp,cJSON_CreateIntArray(pgroup[pos].iArrNo,ARR_MAX));
		cJSON_AddItemToObject(pSon,"iArrNo",ptmp);
		cJSON_AddNumberToObject(pSon, "groupNo", pgroup[pos].groupNo);
		cJSON_AddStringToObject(pSon, "groupName", pgroup[pos].groupName);

		pSon_tmp = cJSON_CreateArray();
		for(tmp = 0;tmp < HOST_COUNT;tmp++){
			ptmp = cJSON_CreateObject();
			cJSON_AddStringToObject(ptmp, "ip", pgroup[pos].host[tmp].ip);
			cJSON_AddNumberToObject(ptmp, "port", pgroup[pos].host[tmp].port);
			cJSON_AddItemToArray(pSon_tmp, ptmp);
		}
		cJSON_AddItemToObject(pSon, "host", pSon_tmp);

		cJSON_AddItemToArray(pNode, pSon);
	}
	cJSON_AddItemToObject(pRoot, "root", pNode);

	*pjson = cJSON_PrintUnformatted(pRoot);
	cJSON_Delete(pRoot);
	return;
}



//解析json字符串, 得到'cJSON结构体', 再利用'cJSON结构体'填充pgroup数组结构体
int Set_Group_t(Group_t *pgroup, char *pjson){
	cJSON *pRoot,*pRootArr,*pNode,*pSon,*ptmp;
	int rootArrLen,arrLen,pos,tmp;

	pRoot = cJSON_Parse(pjson);
	if(pRoot == NULL){
		printf("cJSON_Parse() Err:%s\n", cJSON_GetErrorPtr());
		return 0;
	}
	else{
		pRootArr = cJSON_GetObjectItem(pRoot, "root");
		rootArrLen = cJSON_GetArraySize(pRootArr);
		for(pos = 0;pos < rootArrLen;pos++){
			pNode = cJSON_GetArrayItem(pRootArr, pos);

			pgroup[pos].isWork = cJSON_GetObjectItem(pNode, "isWork")->valueint;
			ptmp = cJSON_GetObjectItem(pNode, "iArrNo");
			arrLen = cJSON_GetArraySize(ptmp);
			for(tmp = 0;tmp < arrLen;tmp++)
				pgroup[pos].iArrNo[tmp]=cJSON_GetArrayItem(ptmp,tmp)->valueint;
			pgroup[pos].groupNo = cJSON_GetObjectItem(pNode, "groupNo")->valueint;
			strncpy(pgroup[pos].groupName, cJSON_GetObjectItem(pNode, "groupName")->valuestring,GROUP_NAME_MAX);

			pSon = cJSON_GetObjectItem(pNode,"host");
			arrLen = cJSON_GetArraySize(pSon);
			for(tmp = 0;tmp < arrLen;tmp++){
				ptmp = cJSON_GetArrayItem(pSon, tmp);
				pgroup[pos].host[tmp].port=cJSON_GetObjectItem(ptmp,"port")->valueint;
				strncpy(pgroup[pos].host[tmp].ip,cJSON_GetObjectItem(ptmp,"ip")->valuestring,IP_MAX);
			}
		}
		cJSON_Delete(pRoot);
		return 1;
	}
}



int main(void){
	const char* pfilepath = "./data.json";
	char* pr;
	unsigned int pr_len = 0;
	char* pjson = NULL;
	int pjson_len;

	pr = mmap_fileEx_r(pfilepath, &pr_len);
	if(!pr)
		return -1;

	printf("./data.json len=%d\n, %s\n",pr_len,pr);

	//压缩json字符串[失败, 不可压缩mmap指针]
	/*
		cJSON_Minify(pr);
		printf("压缩后: ./data.json len=%d\n, %s\n",strnlen(pr,pr_len),pr);
	*/

	//篡改全局变量数据
	g_Groups[0].groupNo = 0;
	g_Groups[1].groupNo = 0;
	strncpy(g_Groups[1].groupName,"baby you light up my world like nobody else!!",GROUP_NAME_MAX);
	strncpy(g_Groups[0].groupName,"baby you light up my world like nobody else!!(again)",GROUP_NAME_MAX);

	//测试是否可以根据json 字符串, 还原来的值, 如果可以, 证明还原也成功
	Set_Group_t(g_Groups,pr);

	//重新生成一次json 字符串, 看前后的字符串是否一致, 一致则成功.
	Get_Group_t_Str(g_Groups,&pjson);
	pjson_len = strlen(pjson);
	printf("还原后: g_Groups string len=%d\n",pjson_len);
	printf("还原后: g_Groups string:\n%s\n\n",pjson);

	munmap(pr,pr_len);
	return 0;
}







