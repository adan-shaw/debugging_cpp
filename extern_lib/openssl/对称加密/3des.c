//编译:
//		gcc -g3 -lssl -lcrypto 3des.c -o x



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>



char *smart_point;



//1.key'1 to 3'函数
/*
	如果填充3 个char buf[8], 实在太麻烦了, 因此这里固定一种模式, 由1 to 3.
	如果被人发现了, 大不了修改1 to 3 函数咯, 方便比较重要.
	务必保证key size = 8, 否则内存溢出。
*/
void key1_to3(char* in_out1, char* out2, char* out3);

//2.打印密文
void print_encrypted(char* data);

//3.打印明文
void print_text(char* data);

//4.3des ebc 加密
/*
	(注意: strlen(key_str),意味着只能传入一串字符串,而且长度不能超出8 字节.)
	char* data,char* ret,这两个参数的缓冲区长度,应当一致!!等于:int buf_len
*/
void des_ebc3_encrypt(char* key_str,char* data,char* ret,int buf_len);

//5.3des ebc 解密
void des_ebc3_decrypt(char* key_str,char* data,char* ret,int buf_len);

//6.3des cbc 加密(char* key_str长度不限, char* ivec必须为8字节)
void des_cbc3_encrypt(char* key_str, char* ivec, char* data, char* ret, int buf_len);

//7.3des cbc 解密
void des_cbc3_decrypt(char* key_str, char* ivec, char* data, char* ret, int buf_len);



//8.3des ebc encrypt 测试
void des_ebc3_encrypt_test(void);
//9.3des ebc decrypt 测试
void des_ebc3_decrypt_test(void);
//10.3des ebc encrypt 测试
void des_cbc3_encrypt_test(void);
//11.3des ebc decrypt 测试
void des_cbc3_decrypt_test(void);



int main(void){
	printf("1.des ebc 测试:\n");
	des_ebc3_encrypt_test();
	des_ebc3_decrypt_test();

	printf("2.des cbc 测试:\n");
	des_cbc3_encrypt_test();
	des_cbc3_decrypt_test();
	return 0;
}



//1.key'1 to 3'函数
/*
	如果填充3 个char buf[8], 实在太麻烦了, 因此这里固定一种模式, 由1 to 3.
	如果被人发现了, 大不了修改1 to 3 函数咯, 方便比较重要.
	务必保证key size = 8, 否则内存溢出。
*/
void key1_to3(char* in_out1, char* out2, char* out3){
	char tmp[4] = "";
	char tmp2[4] = "";
	char tmp3[4] = "669";
	char tmp4[5] = "adan";

	memset(out2,'\0',sizeof(out2));
	memset(out3,'\0',sizeof(out3));

	memcpy(in_out1, &tmp, 4);
	memcpy(in_out1+4, &tmp2, 4);

	memcpy(out2, tmp, 4);
	memcpy(out2, tmp3, 4);

	memcpy(out3, tmp4, 4);
	memcpy(out3, tmp2, 4);

	return;
}



//2.打印密文
void print_encrypted(char* data){
	int i;
	printf("print_encrypted:\n");
	for(i = 0; i < strlen(data); i++)
	 printf("%02x", data[i]);
	printf("\n\n");
	return;
}



//3.打印明文
void print_text(char* data){
	printf("print_text:\n%s\n\n", data);
	return;
}



//4.3des ebc 加密
/*
	(注意: strlen(key_str),意味着只能传入一串字符串,而且长度不能超出8 字节.)
	char* data,char* ret,这两个参数的缓冲区长度,应当一致!!等于:int buf_len
*/
void des_ebc3_encrypt(char* key_str,char* data,char* ret,int buf_len){
	DES_cblock key, key2, key3, data_tmp_in, data_tmp_out;
	DES_key_schedule schedule, schedule2, schedule3;
	int i = strlen(key_str) + 1;
	int datalen = strlen(data);
	char *key_str2;
	char *key_str3;

	//key 长度检查
	if(i > 8){
		printf("des_ebc3_encrypt() fail, 'char* key_str' length > 8\n");
		return;
	}
	//缓冲区长度检查(必须为8的倍数)
	if(buf_len%8 != 0){
		printf("des_ebc3_encrypt() fail, 'int buf_len' length % 8 != 0\n");
		return;
	}
	//数据填补后, 数据长度'是否大于'缓冲区长度??
	datalen = datalen + 1;										//补齐'\0' 结束字符串
	datalen = (datalen%8 == 0) ? datalen : datalen + 8;
	if(datalen > buf_len){
		printf("des_ebc3_encrypt() fail, datalen > buf_len\n");
		return;
	}

	key_str2 = (char*)malloc(8);
	key_str3 = (char*)malloc(8);
	memset(ret,'\0',buf_len);									//如果这里崩溃了, 也容易发现
	memset(key,'\0',sizeof(key));
	memset(key2,'\0',sizeof(key2));
	memset(key3,'\0',sizeof(key3));

	//1 to 3
	key1_to3(key_str,key_str2,key_str3);

	strncpy((char*)&key,key_str,i);
	strncpy((char*)&key2,key_str2,i);
	strncpy((char*)&key3,key_str3,i);
	DES_set_key_unchecked(&key, &schedule);		//将'des 密钥'转换成schedule
	DES_set_key_unchecked(&key2, &schedule2);
	DES_set_key_unchecked(&key3, &schedule3);

	//执行ebc 加密
	for(i = 0;i < datalen/8;i++){
		memcpy(data_tmp_in,data+i*8,8);
		DES_ecb3_encrypt(&data_tmp_in, &data_tmp_out,  	&schedule, &schedule2, &schedule3, DES_ENCRYPT);
		memcpy(ret+i*8,data_tmp_out,8);
	}

	free(key_str2);
	free(key_str3);
	return;
}



//5.3des ebc 解密
void des_ebc3_decrypt(char* key_str,char* data,char* ret,int buf_len){
	DES_cblock key, key2, key3, data_tmp_in, data_tmp_out;
	DES_key_schedule schedule, schedule2, schedule3;
	int i = strlen(key_str) + 1;
	int datalen = strlen(data);
	char *key_str2;
	char *key_str3;

	//key 长度检查
	if(i > 8){
		printf("des_ebc3_decrypt() fail, 'char* key_str' length > 8\n");
		return;
	}
	//缓冲区长度检查(必须为8的倍数)
	if(buf_len%8 != 0){
		printf("des_ebc3_decrypt() fail, 'char* data' length % 8 != 0\n");
		return;
	}
	//数据填补后, 数据长度'是否大于'缓冲区长度??
	datalen = datalen + 1;//补齐'\0' 结束字符串
	datalen = (datalen%8 == 0) ? datalen : datalen + (8-datalen%8);
	if(datalen > buf_len){
		printf("des_ebc3_decrypt() fail, datalen > buf_len\n");
		return;
	}

	key_str2 = (char*)malloc(8);
	key_str3 = (char*)malloc(8);
	memset(ret,'\0',buf_len);									//如果这里崩溃了, 也容易发现
	memset(key,'\0',sizeof(key));
	memset(key2,'\0',sizeof(key2));
	memset(key3,'\0',sizeof(key3));

	//1 to 3
	key1_to3(key_str,key_str2,key_str3);

	strncpy((char*)&key,key_str,i);
	strncpy((char*)&key2,key_str2,i);
	strncpy((char*)&key3,key_str3,i);
	DES_set_key_unchecked(&key, &schedule);		//将'des 密钥'转换成schedule
	DES_set_key_unchecked(&key2, &schedule2);
	DES_set_key_unchecked(&key3, &schedule3);

	//执行ebc 解密
	for(i = 0;i < datalen/8;i++){
		memcpy(data_tmp_in,data+i*8,8);
		DES_ecb3_encrypt(&data_tmp_in, &data_tmp_out,  	&schedule, &schedule2, &schedule3, DES_DECRYPT);
		memcpy(ret+i*8,data_tmp_out,8);
	}

	free(key_str2);
	free(key_str3);
	return;
}



//6.3des cbc 加密(char* key_str长度不限, char* ivec必须为8字节)
void des_cbc3_encrypt(char* key_str, char* ivec, char* data, char* ret, int buf_len){
	DES_cblock key, key2, key3, key_ivec;
	DES_key_schedule schedule, schedule2, schedule3;
	int ivec_len = strlen(ivec) + 1;
	//int keylen = strlen(key_str);
	int datalen = strlen(data);
	char *key_str2;
	char *key_str3;

	//key 长度检查
	if(ivec_len > 8){
		printf("des_cbc3_encrypt() fail, 'char* ivec' length > 8\n");
		return;
	}
	//缓冲区长度检查(必须为8的倍数)
	if(buf_len%8 != 0){
		printf("des_cbc3_encrypt() fail, 'char* data' length % 8 != 0\n");
		return;
	}
	//数据填补后, 数据长度'是否大于'缓冲区长度??
	datalen = datalen + 1;										//补齐'\0' 结束字符串
	datalen = (datalen%8 == 0) ? datalen : datalen + (8-datalen%8);
	if(datalen > buf_len){
		printf("des_cbc3_encrypt() fail, datalen > buf_len\n");
		return;
	}
	/*
	//钥匙长度不受限制
	if(keylen%8 != 0){
		printf("des_cbc_encrypt() fail, 'char* key_str' length % 8 != 0\n");
		return;
	}
	*/
	key_str2 = (char*)malloc(8);
	key_str3 = (char*)malloc(8);
	memset(ret,'\0',buf_len);//如果这里崩溃了, 也容易发现
	memset(key,'\0',sizeof(key));
	memset(key2,'\0',sizeof(key2));
	memset(key3,'\0',sizeof(key3));
	memset(key_ivec,'\0',sizeof(key_ivec));

	DES_string_to_key(key_str, &key);					//生成钥匙
	strncpy((char*)&key_ivec, ivec, ivec_len);//生成ivec key
	DES_set_key_unchecked(&key, &schedule);		//将'des 密钥'转换成schedule

	key1_to3(key_str,key_str2,key_str3);			//1 to 3

	strncpy((char*)&key2,key_str2,ivec_len);
	strncpy((char*)&key3,key_str3,ivec_len);
	DES_set_key_unchecked(&key2, &schedule2);
	DES_set_key_unchecked(&key3, &schedule3);

	//执行cbc 加密
	DES_ede3_cbc_encrypt((unsigned char*)data, (unsigned char*)ret, \
			(long int)datalen, &schedule, &schedule2, &schedule3, &key_ivec, DES_ENCRYPT);

	free(key_str2);
	free(key_str3);
	return;
}



//7.3des cbc 解密
void des_cbc3_decrypt(char* key_str, char* ivec, char* data, char* ret, int buf_len){
	DES_cblock key, key2, key3, key_ivec;
	DES_key_schedule schedule, schedule2, schedule3;
	int ivec_len = strlen(ivec) + 1;
	//int keylen = strlen(key_str);
	int datalen = strlen(data);
	char *key_str2;
	char *key_str3;

	//key 长度检查
	if(ivec_len > 8){
		printf("des_cbc3_decrypt() fail, 'char* ivec' length > 8\n");
		return;
	}
	//缓冲区长度检查(必须为8的倍数)
	if(buf_len%8 != 0){
		printf("des_cbc3_decrypt() fail, 'char* data' length % 8 != 0\n");
		return;
	}
	//数据填补后, 数据长度'是否大于'缓冲区长度??
	datalen = datalen + 1;										//补齐'\0' 结束字符串
	datalen = (datalen%8 == 0) ? datalen : datalen + (8-datalen%8);
	if(datalen > buf_len){
		printf("des_cbc3_decrypt() fail, datalen > buf_len\n");
		return;
	}
	/*
	//钥匙长度不受限制
	if(keylen%8 != 0){
		printf("des_cbc_encrypt() fail, 'char* key_str' length % 8 != 0\n");
		return;
	}
	*/
	key_str2 = (char*)malloc(8);
	key_str3 = (char*)malloc(8);
	memset(ret,'\0',buf_len);									//如果这里崩溃了, 也容易发现
	memset(key,'\0',sizeof(key));
	memset(key2,'\0',sizeof(key2));
	memset(key3,'\0',sizeof(key3));
	memset(key_ivec,'\0',sizeof(key_ivec));


	DES_string_to_key(key_str, &key);					//生成钥匙
	strncpy((char*)&key_ivec, ivec, ivec_len);//生成ivec key
	DES_set_key_unchecked(&key, &schedule);		//将'des 密钥'转换成schedule

	key1_to3(key_str,key_str2,key_str3);			//1 to 3

	strncpy((char*)&key2,key_str2,ivec_len);
	strncpy((char*)&key3,key_str3,ivec_len);
	DES_set_key_unchecked(&key2, &schedule2);
	DES_set_key_unchecked(&key3, &schedule3);

	//执行cbc 加密
	DES_ede3_cbc_encrypt((unsigned char*)data, (unsigned char*)ret, \
			(long int)datalen, &schedule, &schedule2, &schedule3, &key_ivec, DES_DECRYPT);

	free(key_str2);
	free(key_str3);
	return;
}



//8.3des ebc encrypt 测试
void des_ebc3_encrypt_test(void){
	char des_ebc_key[8] = "1234567";
	//char des_ebc_data[512+8] = "hi i'm adan shaw !!";
	char des_ebc_data[512+8] = "中文!3des ebc";//中文
	char* des_ebc_ret = (char*)malloc(512+8);
	int des_ebc_buf_len = 512+8;

	//执行加密
	des_ebc3_encrypt(des_ebc_key,des_ebc_data,des_ebc_ret,des_ebc_buf_len);

	print_text(des_ebc_data);									//结束打印
	print_encrypted(des_ebc_ret);

	smart_point = des_ebc_ret;								//传递给后续测试函数-解密
	return;
}



//9.3des ebc decrypt 测试
void des_ebc3_decrypt_test(void){
	char des_ebc_key[8] = "1234567";
	//char des_ebc_data[512+8] = "hi i'm adan shaw !!";
	char* des_ebc_ret = (char*)malloc(512+8);
	int des_ebc_buf_len = 512+8;

	//执行加密
	des_ebc3_decrypt(des_ebc_key,smart_point,des_ebc_ret,des_ebc_buf_len);

	print_text(des_ebc_ret);									//结束打印

	free(smart_point);
	smart_point = NULL;
	return;
}



//10.3des ebc encrypt 测试
void des_cbc3_encrypt_test(void){
	char des_cbc_key[8] = "1234567";
	char des_cbc_ivce[8] = "9876543";
	//char des_cbc_data[512+8] = "hi i'm adan shaw !!";
	char des_cbc_data[512+8] = "中文hi i'm adan shaw !!";//中文
	char* des_cbc_ret = (char*)malloc(512+8);
	int des_cbc_buf_len = 512+8;

	//执行加密
	des_cbc3_encrypt(des_cbc_key,des_cbc_ivce, des_cbc_data,des_cbc_ret,des_cbc_buf_len);

	print_text(des_cbc_data);									//结束打印
	print_encrypted(des_cbc_ret);

	smart_point = des_cbc_ret;								//传递给后续测试函数-解密
	return;
}



//11.3des ebc decrypt 测试
void des_cbc3_decrypt_test(void){
	char des_cbc_key[8] = "1234567";
	char des_cbc_ivce[8] = "9876543";
	//char des_cbc_data[512+8] = "hi i'm adan shaw !!";
	char* des_cbc_ret = (char*)malloc(512+8);
	int des_cbc_buf_len = 512+8;

	//执行加密
	des_cbc3_decrypt(des_cbc_key,des_cbc_ivce, smart_point,des_cbc_ret,des_cbc_buf_len);

	print_text(des_cbc_ret);									//结束打印

	free(smart_point);
	smart_point = NULL;
	return;
}




