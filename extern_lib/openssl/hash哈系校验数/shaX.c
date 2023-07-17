//编译: 散列算法(签名算法)(哈系算法)
//		gcc -g3 -lcrypto shaX.c -o x



//用法:
/*
	sha系列校验码计算, 是不可能反向的!!
	sha系列校验算法, 不是一种数据加密手段, 不能用作数据加密!!

	如果你对sha系列算法反向破解, 就算能成功, 但是没有意义.
	sha系列校验算法的作用, 基本只能保证数据完整性, 保证数据没有被篡改过, 而不是对数据进行加密!!

	对方仍然可以修改你的数据,
	然后又重新算一个新的'sha校验码'粘帖上去,
	覆盖你原来的'sha校验码', 这样是可以做到篡改.

	但如果对'sha校验码'也进行加密, 那么就安全很多了.
	加密了校验码, 到了'对端'再进行解密, 得到校验码,
	再对'收到的数据'重新计算sha校验码,比对是否一致,一致则没有被篡改.



	md5同理, sha系列, md5系列, 都是哈系类, 散列算法, 也叫签名算法.
	所以, sha系列校验算法, 并不需要反向操作.
	只需对比出'数据传输前',和'传输后'计算出来的sha校验码是否一致, 即可知道数据是否被篡改.

	为了防止校验码被篡改, 从而数据可能被改写, 你可以对'sha校验码'进行加密!!
*/



#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>//包含sha1 224 256 384 512
#include <openssl/crypto.h>//OPENSSL_cleanse()



//测试数据
//const char *str_test_data = "hello world";
//const char *str_test_data2 = "hello world2";
//const char *str_test_data3 = "hello world3";

//测试数据-中文(不测试宽字符)
const char *str_test_data = "中文hello world";
const char *str_test_data2 = "中文hello world2";
const char *str_test_data3 = "中文hello world3";

//测试二进制块数据
const int byte_data = 0b1001;



//1.打印哈系阵列
void printHash(unsigned char *md, int len);
//2.sha1
void mySHA1(void);
//3.sha224
void mySHA224(void);
//4.sha256
void myHash256(void);
//5.sha384
void mySHA384(void);
//6.sha512
void mySHA512(void);



int main(void){
	mySHA1();
	mySHA224();
	myHash256();
	mySHA384();
	mySHA512();
	return 0;
}



//1.打印哈系阵列
void printHash(unsigned char *md, int len){
	int i;
	for(i = 0; i < len; i++)
		printf("%02x", md[i]);
	printf("\n");
	return;
}



//2.sha1
void mySHA1(void){
	SHA_CTX c;
	unsigned char md[SHA_DIGEST_LENGTH];
	printf("mySHA1()\n");

	//1.简单用法(适合单串数据)--直接执行SHA1 校验计算
	SHA1((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, SHA_DIGEST_LENGTH);//打印结果

	//2.复杂用法(适合多串数据, 全部累加在一起, 计算出一个校验和.)
	memset(&c, 0, sizeof(SHA_CTX));
	memset(&md, 0, sizeof(md));

	SHA1_Init(&c);
	SHA1_Update(&c, str_test_data, strlen(str_test_data));//多串数据
	SHA1_Update(&c, str_test_data2, strlen(str_test_data2));
	SHA1_Update(&c, str_test_data3, strlen(str_test_data3));
	SHA1_Update(&c, (void*)&byte_data, 4);//测试二进制块数据进行sha1
	SHA1_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, SHA_DIGEST_LENGTH);
	printf("\n\n");
	return;
}



//3.sha224
void mySHA224(void){
	SHA256_CTX c;
	unsigned char md[SHA224_DIGEST_LENGTH];
	printf("mySHA224()\n");

	//1.简单用法(适合单串数据)
	SHA224((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, SHA224_DIGEST_LENGTH);

	//2.复杂用法(适合多串数据)
	memset(&c, 0, sizeof(SHA_CTX));
	memset(&md, 0, sizeof(md));

	SHA224_Init(&c);
	SHA224_Update(&c, str_test_data, strlen(str_test_data));
	SHA224_Update(&c, str_test_data2, strlen(str_test_data2));
	SHA224_Update(&c, str_test_data3, strlen(str_test_data3));
	SHA224_Update(&c, (void*)&byte_data, 4);
	SHA224_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, SHA224_DIGEST_LENGTH);
	printf("\n\n");
	return;
}



//4.sha256
void myHash256(void){
	SHA256_CTX c;
	unsigned char md[SHA256_DIGEST_LENGTH];
	printf("myHash256()\n");

	//1.简单用法(适合单串数据)
	SHA256((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, SHA256_DIGEST_LENGTH);

	//2.复杂用法(适合多串数据)
	SHA256_Init(&c);
	SHA256_Update(&c, str_test_data, strlen(str_test_data));
	SHA256_Update(&c, str_test_data2, strlen(str_test_data2));
	SHA256_Update(&c, str_test_data3, strlen(str_test_data3));
	SHA256_Update(&c, (void*)&byte_data, 4);
	SHA256_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, SHA256_DIGEST_LENGTH);
	printf("\n\n");
	return;
}



//5.sha384
void mySHA384(void){
	SHA512_CTX c;
	unsigned char md[SHA384_DIGEST_LENGTH];
	printf("mySHA384()\n");

	//1.简单用法(适合单串数据)
	SHA384((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, SHA384_DIGEST_LENGTH);

	//2.复杂用法(适合多串数据)
	SHA384_Init(&c);
	SHA384_Update(&c, str_test_data, strlen(str_test_data));
	SHA384_Update(&c, str_test_data2, strlen(str_test_data2));
	SHA384_Update(&c, str_test_data3, strlen(str_test_data3));
	SHA384_Update(&c, (void*)&byte_data, 4);
	SHA384_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, SHA384_DIGEST_LENGTH);
	printf("\n\n");
	return;
}



//6.sha512
void mySHA512(void){
	SHA512_CTX c;
	unsigned char md[SHA512_DIGEST_LENGTH];
	printf("mySHA512()\n");

	//1.简单用法(适合单串数据)
	SHA512((unsigned char *)str_test_data, strlen(str_test_data), md);
	printHash(md, SHA512_DIGEST_LENGTH);

	//2.复杂用法(适合多串数据)
	SHA512_Init(&c);
	SHA512_Update(&c, str_test_data, strlen(str_test_data));
	SHA512_Update(&c, str_test_data2, strlen(str_test_data2));
	SHA512_Update(&c, str_test_data3, strlen(str_test_data3));
	SHA512_Update(&c, (void*)&byte_data, 4);
	SHA512_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));

	printHash(md, SHA512_DIGEST_LENGTH);
	printf("\n\n");
	return;
}
