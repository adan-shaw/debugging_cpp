//编译: 
//		g++ -g3 -lm  ./fopen读取伪随机数模板.cpp -o x 



//简要说明:
//	c++ 从/dev/urandom 中读取伪随机数的模板(尽量少用, 跨平台能力不强)



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "adan_clib/debug_locating_errors.h"



using namespace std;



// 字典MAX大小(byte char)
#define RANDOM_DIST_MAX 256

// 初始化字典(随机字符ascii[33-126], 都是可见字符, 填充缓冲区)
void init_str_buf(char *pBuf, unsigned int uiLen);

// 基础测试: 读取不同类型的随机数据
void basic_test(void);





// 主测试函数
int main(void){
	char *pDist = NULL;



	printf("test1:\n");
	basic_test();



	printf("\n\n\ntest2:\n");
	pDist = (char*)malloc( sizeof(char) * RANDOM_DIST_MAX );
	if(pDist == NULL){
		MY_DEBUG("malloc()");
		return -1;
	}

	//向字典buf 填充随机字符
	init_str_buf(pDist, RANDOM_DIST_MAX);
	printf("%s\n", pDist);

	free(pDist);
	return 0;
}





// 基础测试: 读取不同类型的随机数据
void basic_test(void){
	unsigned int uiTmp;
	unsigned short usTmp;
	unsigned char ucTmp;
	char cTmp;
	FILE *pf = NULL;



	pf = fopen("/dev/urandom", "rb");
	if(pf == NULL){
		MY_DEBUG("fopen()");
		return;
	}


	fread(&uiTmp, sizeof(unsigned int), 1, pf);
	printf("%d\n", uiTmp);

	fread(&usTmp, sizeof(unsigned short), 1, pf);
	printf("%d\n", usTmp);

	fread(&ucTmp, sizeof(unsigned char), 1, pf);
	printf("%c\n", ucTmp);

	fread(&cTmp, sizeof(char), 1, pf);
	printf("%c\n", cTmp);


	assert(fclose(pf) == 0);
	return;
}





// 初始化字典(随机字符ascii[33-126], 都是可见字符, 填充缓冲区)
void init_str_buf(char *pBuf, unsigned int uiLen){
	unsigned int uiTmp, iCount;
	unsigned char ucTmp;
	FILE *pf = NULL;



	pf = fopen("/dev/urandom", "rb");
	if(pf == NULL){
		MY_DEBUG("fopen()");
		return;
	}


	for(iCount = 0; iCount < uiLen; iCount++){
		//从/dev/urandom中读取一个字节的随机数据
		fread(&ucTmp, sizeof(unsigned char), 1, pf);

		//ucTmp 强转为uiTmp
		uiTmp = ucTmp;

		//如果uiTmp大于126, 则用uiTmp - 126, 得到一个[33-126]的值;
		if(uiTmp > 126)
			//uiTmp = uiTmp - 126 + 33;
			uiTmp = uiTmp - 93;//减出来的值, 必定大于33, 使随机数分布, 平均一点.
			//妙用, 这里先执行uiTmp - 126, 无论减出来的值, 是否大于33, 下面都有判断抖底
			//但是, 这样一来, 少于33 的命中率会增加.

		//如果uiTmp少于33, 则用126 - uiTmp, 得到一个[33-126]区间的值;
		if(uiTmp < 33)
			uiTmp = 126 - uiTmp;


		//int 强转为unsigned char
		ucTmp = uiTmp;

		//unsigned char 强转为char
		pBuf[iCount] = ucTmp;
	}


	assert(fclose(pf) == 0);
	return;
}

