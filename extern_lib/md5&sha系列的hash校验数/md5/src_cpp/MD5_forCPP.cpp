//编译:
//		g++ -g3 -o x ./MD5_forCPP.cpp



//md5 class{} 用例测试
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MD5_forCPP.h"



//压力测试
void stress_testing(void);



int main(void){
	const char* data = "ssss";				//测试数据1(错误, 不能传入常量字符串)
	char data2[] = "ssss";						//测试数据2
	unsigned char data3[] = "ssss";		//测试数据3
	unsigned char md5_result[16];			//md5 计算结果
	md5 m_md5;												//md5类实体
	int i;
	unsigned int str_len;



	memset(md5_result,0,16);
	str_len = strnlen(data,64);//使用MD5()之前,必须保证'字符串长度的准确性'!(即使出错, 也要在strnlen()中出错, MD5()不再为你'计算字符串长度')
	m_md5.MD5((unsigned char*)data,str_len,md5_result);//测试1
	//打印结果
	printf("1.请求md5的数据:%s\n计算出来的md5校验码:",data);
	for(i=0;i<16;i++)
		printf("%02x",md5_result[i]);
	printf("\n\n");


	memset(md5_result,0,16);
	str_len = strnlen(data2,64);
	m_md5.MD5((unsigned char*)data2,str_len,md5_result);//测试2
	//打印结果
	printf("2.请求md5的数据:%s\n计算出来的md5校验码:",data2);
	for(i=0;i<16;i++)
		printf("%02x",md5_result[i]);
	printf("\n\n");


	memset(md5_result,0,16);
	str_len = strnlen((const char*)data3,64);
	m_md5.MD5(data3,str_len,md5_result);//测试3
	//打印结果
	printf("3.请求md5的数据:%s\n计算出来的md5校验码:",data3);
	for(i=0;i<16;i++)
		printf("%02x",md5_result[i]);
	printf("\n\n");



	stress_testing();//压力测试

	exit(0);
}



//压力测试
void stress_testing(void){
	char data2[4096] = "ssss";
	char data2_x = '0';
	int data2_pos = 4;
	unsigned char data3[128] = "ssss";
	char data3_x = '0';
	int data3_pos = 4;
	unsigned char md5_result[16];//md5 计算结果
	md5 m_md5;//md5类实体
	int i,k;
	unsigned int str_len;



	for(k = 0;k < 9999;k++){
		memset(md5_result,0,16);
		str_len = strnlen(data2,4096);
		//str_len = strlen(data2);
		m_md5.MD5((unsigned char*)data2,str_len,md5_result);//测试1
		//打印结果
		printf("x1-%d.请求md5的数据:%s\nstr_len=%d\n计算出来的md5校验码:",k,data2,str_len);
		for(i=0;i<16;i++)
			printf("%02x",md5_result[i]);
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
		memset(md5_result,0,16);
		str_len = strnlen((const char*)data3,128);
		//str_len = strlen((const char*)data3);
		m_md5.MD5((unsigned char*)data3,str_len,md5_result);//测试2
		//打印结果
		printf("x2-%d.请求md5的数据:%s\nstr_len=%d\n计算出来的md5校验码:",k,data3,str_len);
		for(i=0;i<16;i++)
			printf("%02x",md5_result[i]);
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
