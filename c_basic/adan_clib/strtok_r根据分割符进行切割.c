//编译:
//		gcc -g3 ./strtok_r根据分割符进行切割.c -o x



#include <stdio.h>
#include <string.h>


int main(void){
	const char chBuffer[] = "Can I help you";	//源字符串可以为const
	char *pchDilem = " ";											//分割符号: 空格
	char *pchStrTmpIn = NULL;									//剩余字符串指针
	char *pchTmp = chBuffer;									//当前切割出来的字符串指针

	//必须有pchTmp 指针, 否则strtok_r 不能向下移动, 会陷入死循环, 永远打印第一串字符
	//while(NULL != ( pchTmp = strtok_r( &chBuffer, pchDilem, &pchStrTmpIn) )){
	while(NULL != ( pchTmp = strtok_r( pchTmp, pchDilem, &pchStrTmpIn) )){
		printf("\n pchTmp[%s] pchStrTmpIn[%s]\n",pchTmp,pchStrTmpIn);
		pchTmp = NULL;
	}

	return 0;
}

