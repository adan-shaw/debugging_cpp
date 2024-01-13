//编译:
//		gcc -g3 -lssl -lcrypto aes.c -o x



//注意点1: key_str buf 长度, 应为: AES_BLOCK_SIZE*n 倍, 不足补'\n'
//注意点2: 密文缓冲区buf, 长度应为:AES_BLOCK_SIZE*n 倍, 不足补04

//key 结构体: AES_KEY key;

//aes ebc 模式(仍然是循环加密, 解密的ebc 模式! 每次'加密/解密'一个AES_BLOCK_SIZE 长度):
/*
	AES_set_encrypt_key()		//设置aes ebc 加密key
	AES_encrypt()						//执行aes ebc 加密
	AES_set_decrypt_key()		//设置aes ebc 解密key
	AES_decrypt()						//执行aes ebc 解密
*/

//aes cbc 模式
/*
	AES_set_encrypt_key()		//设置aes cbc 加密key
	AES_cbc_encrypt()				//执行aes cbc 加密
	AES_set_decrypt_key()		//设置aes cbc 解密key
	AES_cbc_decrypt()				//执行aes cbc 解密
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <openssl/aes.h>



//#define AES_KEY_LEN (AES_BLOCK_SIZE*8)							//128 bit aes
//#define AES_KEY_LEN (AES_BLOCK_SIZE*12)							//192 bit aes
#define AES_KEY_LEN (AES_BLOCK_SIZE*16)								//192 bit aes

#define AES_KEY_STR "Email:adan_shaw@qq.com"					//'共知密钥'
#define AES_ENCRYPT_BUF_SIZE (AES_KEY_LEN*10)					//'共知-约定的加密数据长度'(每段默认长度为: AES_KEY_LEN)
//测试数据
//#define TEST_DATA "11111111111222222222222222333333333333444444444445555555555 \
	asdfkajsdhnfgjkasdrjglksdfjglksdfjglkdfjglksdfngodfg !!!"
#define TEST_DATA "111111111每段默认长度为11222222222222222333333333333444444444445555555555 \
	asdfkaj每段默认长度为sdhnfgjkasdrjglksdfjglksdfjglkdfjglksdfngodfg !!!"

#define AES_BITS (128)
#define MSG_LEN (128)



//******************************
//1.ebc 版本(一个函数, 包含加密和解密)
//******************************
void aes_ebc(void);


//***
//2.cbc 版本
//***
//2.1 cbc 加密
bool aes_encrypt(char* in, char* key, char* out);
//2.2 cbc 解密
bool aes_decrypt(char* in, char* key, char* out);
//2.3 cbc 测试
void aes_cbc(void);



int main(void){
	aes_ebc();
	aes_cbc();
	//evp_aes();
	return 0;
}



//******************************
//1.ebc 版本(一个函数, 包含加密和解密)
//******************************
void aes_ebc(void){
	unsigned char key_str[AES_KEY_LEN];
	unsigned char *data=(unsigned char*)malloc(AES_ENCRYPT_BUF_SIZE);		//原数据buf
	unsigned char *encrypt=(unsigned char*)malloc(AES_ENCRYPT_BUF_SIZE);//密文buf
	unsigned char *decrypt=(unsigned char*)malloc(AES_ENCRYPT_BUF_SIZE);//解密文buf

	int len = 0, tmp = 0;
	AES_KEY key;

	memset((void *)key_str,'\0',AES_KEY_LEN);						//init'加密钥匙'的内容
	memset((void *)data,'\0',AES_ENCRYPT_BUF_SIZE);			//init'原始数据'的内容
	memset((void *)encrypt,04,AES_ENCRYPT_BUF_SIZE);		//init'密文buf'
	memset((void *)decrypt,'\0',AES_ENCRYPT_BUF_SIZE);	//init'解密文buf'

	//我的篡改
	strncpy((char*)key_str,AES_KEY_STR,AES_KEY_LEN);		//init'共知密钥'
	strncpy((char*)data,TEST_DATA,AES_ENCRYPT_BUF_SIZE);//init原文数据

	//打印原文数据
	printf("data:\n%s\n",data);
	printf("TEST_DATA len = %d\n",sizeof(TEST_DATA));
	printf("data len: %d <==> %d\n\n",strlen((char*)data), AES_ENCRYPT_BUF_SIZE);

	//1.加密部分
	//*********************************************************
	//初始化aes 加密钥匙key, 并明确aes 密钥的长度=AES_BLOCK_SIZE*8=AES_ENCRYPT_BUF_SIZE.
	AES_set_encrypt_key(key_str, AES_KEY_LEN, &key);

	//循环加密, 每次只能加密AES_BLOCK_SIZE长度的数据! 所以: 定义,原数据buf or 密文buf or 解密文buf, 都应该是AES_BLOCK_SIZE*n倍
	len = 0;
	tmp = strlen((char*)data);
	//printf("data len=%d\n",tmp);
	//while(len < AES_ENCRYPT_BUF_SIZE){
	while(len < tmp){
		AES_encrypt(data+len, encrypt+len, &key);
		len += AES_BLOCK_SIZE;
	}

	//打印密文
	printf("encrypt: \n");
	for(tmp = 0; tmp < AES_ENCRYPT_BUF_SIZE; tmp++){
		printf("%.2x ", encrypt[tmp]);
		if((tmp+1) % 32 == 0){
			printf("\n");
		}
	}
	printf("encrypt text:\n%s\n\n",encrypt);

	//打印加密钥匙
	printf("加密钥匙:\n%s\n\n\n\n",key_str);
	//*********************************************************


	//2.解密部分
	//*********************************************************
	//重置数据, 防止作弊
	memset((void *)&key, '\0', sizeof(key));						//重置key
	memset((void *)key_str, '\0', AES_KEY_LEN);					//重置'共知钥匙字符串'
	strncpy((char*)key_str, AES_KEY_STR, AES_KEY_LEN);	//初始化'共知密钥'

	//初始化aes 解密key; 并明确aes 密钥的长度 = AES_BLOCK_SIZE*8.
	AES_set_decrypt_key(key_str, AES_KEY_LEN, &key);

	//循环解密
	len = 0;
	tmp = strlen((char*)encrypt);
	printf("encrypt len=%d\n",tmp);
	while(len < AES_ENCRYPT_BUF_SIZE){
		AES_decrypt(encrypt+len, decrypt+len, &key);
		len += AES_BLOCK_SIZE;
	}

	//解密后与原数据是否一致
	if(!memcmp(decrypt, data, strlen((char*)data)))
		printf("test success\n\n");
	else
		printf("test failed\n\n");

	printf("decrypt:\n%s\n\n",decrypt);									//打印解密文
	printf("解密钥匙:\n%s\n\n\n\n",key_str);							//打印解密钥匙
	//*********************************************************

	free(data);
	free(encrypt);
	free(decrypt);

	return ;
}





//***
//2.cbc 版本
//***
//2.1 cbc 加密
bool aes_encrypt(char* in, char* key, char* out){
	AES_KEY aes;
	unsigned char iv[AES_BLOCK_SIZE];										//加密的初始化向量
	int len=strlen(in);

	for(int i=0; i<AES_BLOCK_SIZE; ++i)
		iv[i]=0;																					//iv一般设置为全0,可以设置其他,但是加密解密要一样就行

	//设置aes encrypt 对称加密钥匙key
	if(AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
		return false;

	//执行cbc 加密AES_cbc_encrypt() + AES_ENCRYPT
	/*
		这里的len长度是char*in的长度,但如果char*in中间包含'\0'字符的话
		那么就只会加密前面'\0'前面的一段(遇到'\0'结束字符串,只加密一串字符串),
		所以,这个len可以作为参数传进来, 记录char*in的长度.
		至于解密也是一个道理,光以'\0'来判断字符串长度,确有不妥.
	*/
	AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_ENCRYPT);
	return true;
}



//2.2 cbc 解密
bool aes_decrypt(char* in, char* key, char* out){
	AES_KEY aes;
	unsigned char iv[AES_BLOCK_SIZE];//加密的初始化向量
	int len=strlen(in);

	for(int i=0; i<AES_BLOCK_SIZE; ++i)
		iv[i]=0;																					//iv一般设置为全0,可以设置其他,但是加密解密要一样就行

	//设置cbc 解密钥匙
	if(AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
		return false;

	//执行cbc解密AES_cbc_encrypt() + AES_DECRYPT
	AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_DECRYPT);
	return true;
}



//2.3 cbc 测试
void aes_cbc(void){
	char sourceStringTemp[MSG_LEN];
	char dstStringTemp[MSG_LEN];
	char key[AES_BLOCK_SIZE];
	int i;

	memset((char*)sourceStringTemp, 0 ,MSG_LEN);
	memset((char*)dstStringTemp, 0 ,MSG_LEN);
	strncpy((char*)sourceStringTemp,"123456789 123456789 123456789 12a",sizeof(sourceStringTemp));
	//strcpy((char*)sourceStringTemp, argv[1]);

	for(i = 0; i < 16; i++)															//可自由设置密钥
		key[i] = 32 + i;


	//1.加密测试
	if(!aes_encrypt(sourceStringTemp,key,dstStringTemp)){
		printf("encrypt error\n");
		return ;
	}
	printf("密文长度aes_encrypt() len=%d:",strlen((char*)dstStringTemp));
	for(i= 0;dstStringTemp[i];i+=1)											//打印密文
		printf("%x",(unsigned char)dstStringTemp[i]);


	//2.解密测试
	memset((char*)sourceStringTemp, 0 ,MSG_LEN);
	if(!aes_decrypt(dstStringTemp,key,sourceStringTemp)){
		printf("decrypt error\n");
		return ;
	}
	printf("\n原文长度aes_decrypt() len=%d:",strlen((char*)sourceStringTemp));
	printf("%s\n",sourceStringTemp);										//打印密文
	for(i= 0;sourceStringTemp[i];i+=1)									//打印密文2
		printf("%x",(unsigned char)sourceStringTemp[i]);

	printf("\n\n");
	return ;
}





//***
//EVP 虽然方便, 但版本不对, 改动太大. 新版本貌似不兼容EVP_CIPHER_CTX ctx; 结构
//***
/*
	EVP框架是对openssl提供的所有算法进行了封装, 
	在使用工程中只需要修改少量的代码就可以选择不同的加密算法, 在工作中通常采用这种方式. 

	在上述两个示例中, 直接使用API提供的接口, 没有使用padding, 在EVP中同样需要声明不可以使用padding方式, 
	否则即使要加密的数据长度是AES_BLOCK_SIZE的整数倍, EVP默认也会对原始数据进行追加, 导致结果不同, 
	所以在试验中通过EVP_CIPHER_CTX_set_padding(&ctx, 0)函数关闭的EVP的padding功能, 同样在解密的时候也需要进行关闭. 
*/

/*

#include <openssl/evp.h>
#include <openssl/aes.h>

int evp_aes(void){
	char key_str[EVP_MAX_KEY_LENGTH];
	char iv[EVP_MAX_IV_LENGTH];
	unsigned char *data = (unsigned char *)malloc(AES_ENCRYPT_BUF_SIZE);
	unsigned char *encrypt = (unsigned char *)malloc(AES_BLOCK_SIZE*6);
	unsigned char *decrypt = (unsigned char *)malloc(AES_BLOCK_SIZE*6);
	EVP_CIPHER_CTX ctx;
	int ret;
	int tlen = 0;
	int mlen = 0;
	int flen = 0;

	memset((void *)key_str, 'k', EVP_MAX_KEY_LENGTH);
	memset((void *)iv, 'i', EVP_MAX_IV_LENGTH);
	memset((void *)data, 'p', AES_ENCRYPT_BUF_SIZE);
	memset((void *)encrypt, 0, AES_BLOCK_SIZE*6);
	memset((void *)decrypt, 0, AES_BLOCK_SIZE*6);

	//初始化ctx
	EVP_CIPHER_CTX_init(&ctx);

	//指定加密算法及key和iv(此处IV没有用)
	ret = EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key_str, iv);
	if(ret != 1){
		printf("EVP_EncryptInit_ex failed\n");
		exit(-1);
	}

	//禁用padding功能
	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	//进行加密操作
	ret = EVP_EncryptUpdata(&ctx, encrypt, &mlen, data, AES_ENCRYPT_BUF_SIZE);
	if(ret != 1){
		printf("EVP_EncryptUpdata failed\n");
		exit(-1);
	}
	//结束加密操作
	ret = EVP_EncryptFinal_ex(&ctx, encrypt+mlen, &flen);
	if(ret != 1){
		printf("EVP_EncryptFinal_ex failed\n");
		exit(-1);
	}

	tlen = mlen + flen;

	tlen = 0;
	mlen = 0;
	flen = 0;

	EVP_CIPHER_CTX_cleanup(&ctx);
	EVP_CIPHER_CTX_init(&ctx);
	 
	ret = EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key_str, iv);
	if(ret != 1){
		printf("EVP_DecryptInit_ex failed\n");
		exit(-1);
	}

	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	ret = EVP_DecryptUpdata(&ctx, decrypt, &mlen, encrypt, AES_ENCRYPT_BUF_SIZE);
	if(ret != 1){
		printf("EVP_DecryptUpdata failed\n");
		exit(-1);
	}

	ret = EVP_DecryptFinal_ex(&ctx, decrypt+mlen, &flen);
	if(ret != 1){
		printf("EVP_DecryptFinal_ex failed\n");
		exit(-1);
	}

	//对比解密后与原数据是否一致
	if(!memcmp(decrypt, data, AES_ENCRYPT_BUF_SIZE))
		printf("test success\n");
	else
		printf("test failed\n");

	printf("encrypt: ");
	int i;
	for(i = 0; i < AES_ENCRYPT_BUF_SIZE; i ++){
		printf("%.2x ", encrypt[i]);
		if((i+1)%32 == 0){
			printf("\n");
		}
	}
	printf("\n");

	return 0;
}

*/
