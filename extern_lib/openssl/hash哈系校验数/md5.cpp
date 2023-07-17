//编译: 散列算法(签名算法)(哈系算法)
//		g++ -lcrypto -g3 md5.cpp -o x



#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>//OPENSSL_cleanse()



//const char *str_test_data = "hello world";		//测试字符串数据
const char *str_test_data = "测试数据-中文 md5";	//测试字符串数据-中文
const int byte_data = 0b1001;										//测试二进制块数据



//1.打印函数
void printHash(unsigned char *md, int len){
	int i;
	for(i = 0; i < len; i++)
		printf("%02x", md[i]);
	printf("\n");
	return;
}



//2.md5
void myMD5(void){
	MD5_CTX c;
	unsigned char md[MD5_DIGEST_LENGTH];
	printf("myHash1()\n");

	//1.简单用法-单串快捷方法
	MD5((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, MD5_DIGEST_LENGTH);

	//2.复杂用法(适合多串数据)
	MD5_Init(&c);
	MD5_Update(&c, str_test_data, strlen(str_test_data));//多串叠加计算.
	MD5_Update(&c, str_test_data, strlen(str_test_data));
	MD5_Update(&c, (void*)&byte_data, 4);//测试二进制块数据进行md5 
	MD5_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, MD5_DIGEST_LENGTH);
	printf("\n\n");
	return;
}



int main(void){
	myMD5();
	return 0;
}




