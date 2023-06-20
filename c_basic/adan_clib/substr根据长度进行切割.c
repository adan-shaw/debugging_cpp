//编译:
//		gcc -g3 ./substr根据长度进行切割.c -o x


//简介:
//	c语言中没有类似substr截取子串的函数, 可以用strncpy, strncat实现



#include<stdio.h>
#include<string.h>
#include<stdlib.h>



/*
	首先获取str_src的长度, 判断str_src的长度, 减去需要截取的substr开始位置, 
	之后, 剩下的长度是否大于指定的长度len,
	如果大于, 就可以取长度为len的子串,
	否则就把从开始位置剩下的字符串全部返回.

	offset = 截取的起始位置
*/

char* substr(const char* str_src, int offset, unsigned int len){
	char *str_tmp = NULL;
	int len_total, real_len;

	len_total = strlen(str_src);											//必须保证str_src 为单串字符串, 且有'\0'结束符号!!
																										//(多串字符串, 截取结果有误; 没有'\0'结束符号, 则内存越界)
	real_len = ((len_total - offset) >= len ? len : (len_total - offset)) + 1;

	if(NULL == (str_tmp = (char*)malloc(real_len * sizeof(char)))){
		perror("malloc()");
		exit(0);
	}

	strncpy(str_tmp, str_src+offset, real_len-1);
	str_tmp[real_len-1] = '\0';
	return str_tmp;
}

void substr_test(void){
	const char str_src[] = "this is a test string!";
	char* str_tmp = substr(str_src, 8, 8);						//普通截取测试
	printf("TEST: result = %s\n", str_tmp);
	free(str_tmp);

	str_tmp = substr(str_src, 8, 0);									//空截取测试
	printf("TEST: result = %s\n", str_tmp);
	if(str_tmp[0] == '\0')
		printf("TEST: substr() 空字符串截取test okay\n");
	free(str_tmp);

	str_tmp = substr(str_src, 0, strlen(str_src));		//满员截取测试
	printf("TEST: result = %s\n", str_tmp);
	free(str_tmp);
}



//'len_cut截取长度'不能大于'len_buf缓冲区长度';
//每次执行截取之前, 最好都做一个'MIN_NUM'三目运算符, 进行比较大小, 保证strncpy()操作不会溢出
#define MIN_NUM(a,b) ( (a)<(b) ? (a) : (b) )

void substr_r(const char* str_src, int offset, char* str_buf, unsigned int len_cut){
	int len_total, real_len;

	len_total = strlen(str_src);											//必须保证str_src 为单串字符串, 且有'\0'结束符号!!
																										//(多串字符串, 截取结果有误; 没有'\0'结束符号, 则内存越界)
	real_len = ((len_total - offset) >= len_cut ? len_cut : (len_total - offset)) + 1;

	strncpy(str_buf, str_src+offset, real_len-1);
	str_buf[real_len-1] = '\0';
}

void substr_r_test(void){
	const char str_src[] = "this is a test string!";
	char str_buf[64];
	memset(str_buf,'\0',64);
	substr_r(str_src, 8, str_buf, MIN_NUM(8,64));			//普通截取测试
	printf("TEST: result = %s\n", str_buf);

	memset(str_buf,'\0',64);
	substr_r(str_src, 8, str_buf, MIN_NUM(0,64));			//空截取测试
	printf("TEST: result = %s\n", str_buf);
	if(str_buf[0] == '\0')
		printf("TEST: substr() 空字符串截取test okay\n");

	memset(str_buf,'\0',64);
	substr_r(str_src, 0, str_buf, MIN_NUM(strlen(str_src),64));
	printf("TEST: result = %s\n", str_buf);						//满员截取测试
}

int main(void){
	substr_test();
	substr_r_test();
	return 0;
}


