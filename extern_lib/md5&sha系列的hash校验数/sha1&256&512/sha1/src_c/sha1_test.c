//编译:
//		gcc -g3 ./sha1.h sha1_test.c -o x 

//用法(usage):
//	# 两次计算结果一致, 即sha256正确
//	./x ./x
//	./x ./x

#include <stdio.h>
#include <stdlib.h>
#include "sha1.h"



//压力测试
void stress_testing(void);



int main(void){
	const char* data = "ssss";			//测试数据1(错误, 不能传入常量字符串)
	char data2[] = "ssss";					//测试数据2
	unsigned char data3[] = "ssss";	//测试数据3
	unsigned char sha1_result[20];	//sha1 计算结果
	int i;
	unsigned int str_len;



	memset(sha1_result,0,20);
	//使用MD5()之前,必须保证'字符串长度的准确性'! 经过截断改造后, MD5()不会为你'计算字符串长度'
	str_len = strnlen(data,64);
	SHA1((unsigned char*)data,str_len,sha1_result);	//计算sha1 校验码
	printf("1.请求sha1的数据:%s\n计算出来的sha1校验码(const char*):",data);
	for(i=0;i<20;i++)
		printf("%02x",sha1_result[i]);
	printf("\n\n");


	memset(sha1_result,0,20);
	str_len = strnlen(data2,64);
	SHA1((unsigned char*)data2,str_len,sha1_result);//计算sha1 校验码
	printf("2.请求sha1的数据:%s\n计算出来的sha1校验码(char*):",data2);
	for(i=0;i<20;i++)
		printf("%02x",sha1_result[i]);
	printf("\n\n");


	memset(sha1_result,0,20);
	str_len = strnlen((const char*)data3,64);
	SHA1(data3,str_len,sha1_result);								//计算sha1 校验码
	printf("3.请求sha1的数据:%s\n计算出来的sha1校验码(unsigned char*):",data3);
	for(i=0;i<20;i++)
		printf("%02x",sha1_result[i]);
	printf("\n\n");



	//stress_testing();//压力测试

	return 0;
}





//压力测试(忽略, 只要看计算速度, 不用管准确性的)
void stress_testing(void){
	char data2[4096] = "ssss";
	char data2_x = '0';
	int data2_pos = 4;
	unsigned char data3[128] = "ssss";
	char data3_x = '0';
	int data3_pos = 4;
	unsigned char sha1_result[20];//sha1 计算结果
	int i,k;
	unsigned int str_len;



	for(k = 0;k < 9999;k++){
		memset(sha1_result,0,20);
		str_len = strnlen(data2,4096);
		//str_len = strlen(data2);
		SHA1((unsigned char*)data2,str_len,sha1_result);//测试1
		//打印结果
		printf("x1-%d.请求sha1的数据:%s\nstr_len=%d\n计算出来的sha1校验码:",\
				k,data2,str_len);
		for(i=0;i<20;i++)
			printf("%02x",sha1_result[i]);
		printf("\n\n");

		if(data2_pos >= 4096){
			data2_pos = 0;
			memset(data2,0,4096);
		}
		data2[data2_pos++] = data2_x;
		data2[data2_pos] = '\0';
		data2_x+=1;//字符变动
	}

	for(k = 0;k < 9999;k++){
		memset(sha1_result,0,20);
		str_len = strnlen((const char*)data3,128);
		//str_len = strlen((const char*)data3);
		SHA1((unsigned char*)data3,str_len,sha1_result);//测试2
		//打印结果
		printf("x2-%d.请求sha1的数据:%s\nstr_len=%d\n计算出来的sha1校验码:",\
				k,data3,str_len);
		for(i=0;i<20;i++)
			printf("%02x",sha1_result[i]);
		printf("\n\n");

		if(data3_pos >= 128){
			data3_pos = 0;
			memset(data3,0,128);
		}
		data3[data3_pos++] = data3_x;
		data3[data3_pos] = '\0';
		data3_x+=1;//字符变动
	}
}

