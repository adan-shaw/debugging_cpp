//编译:
//		gcc ./SSL_key.c -o x -lssl -lcrypto



//自动生成私钥公钥
//		openssl genrsa -out ./private_key.pem 2048 
//		openssl rsa -in ./private_key.pem -pubout -out ./public_key.pem
//校验私钥
//		openssl rsa -noout -modulus -in ./private_key.pem | openssl md5
//校验公钥
//		openssl rsa -noout -modulus -in ./public_key.pem -pubin | openssl md5



//测试:
//		sudo ./x



#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>



//用于读取PEM 格式的钥匙文件(可以是公钥is_pri_pub=0, 可以是私钥is_pri_pub=1)
RSA *rsa_read_key (const char *path_rsa_key, bool is_pri_pub)
{
	RSA *rsa;
	FILE *file = fopen (path_rsa_key, "r");
	if (file == NULL)
	{
		perror ("Error opening private key file");
		return NULL;
	}

	if(is_pri_pub)
		rsa = PEM_read_RSAPrivateKey (file, NULL, NULL, NULL);	//两种RSA 报头通用, 很奇怪!!(私钥API 通用, 公钥API 有特殊约束, 呵呵)
	else
		//rsa = PEM_read_RSAPublicKey (file, NULL, NULL, NULL);	//注意, RSA 公钥报头不一样, 读取RSA 公钥的API 也不一样, 前后不兼容, 比较无语!!
		rsa = PEM_read_RSA_PUBKEY (file, NULL, NULL, NULL);
	if (!rsa)
	{
		ERR_print_errors_fp (stderr);
		fclose (file);
		return NULL;
	}

	fclose (file);
	return rsa;
}



//对消息进行RSA签名
bool rsa_sign_msg (const char *message, RSA *rsa_priv_key, unsigned char *sign, long unsigned int *signLen)
{
	EVP_PKEY *pkey = EVP_PKEY_new ();
	EVP_PKEY_set1_RSA (pkey, rsa_priv_key);

	EVP_MD_CTX *ctx = EVP_MD_CTX_new ();
	if (!ctx)
	{
		perror ("Error creating context");
		EVP_PKEY_free (pkey);
		return false;
	}

	if (EVP_DigestSignInit (ctx, NULL, EVP_sha256 (), NULL, pkey) != 1)
	{
		perror ("Error initializing sign context");
		EVP_MD_CTX_free (ctx);
		EVP_PKEY_free (pkey);
		return false;
	}

	if (EVP_DigestSignUpdate (ctx, message, strlen (message)) != 1)
	{
		perror ("Error updating sign context");
		EVP_MD_CTX_free (ctx);
		EVP_PKEY_free (pkey);
		return false;
	}

	if (EVP_DigestSignFinal (ctx, sign, signLen) != 1)
	{
		perror ("Error finalizing sign context");
		EVP_MD_CTX_free (ctx);
		EVP_PKEY_free (pkey);
		return false;
	}

	EVP_MD_CTX_free (ctx);
	EVP_PKEY_free (pkey);
	return true;
}



//验证RSA签名
bool verifySignature (const char *message, RSA *rsa_pub_key, const unsigned char *sign, long unsigned int *signLen)
{
	int tmp;
	EVP_PKEY *pkey = EVP_PKEY_new ();
	EVP_PKEY_set1_RSA (pkey, rsa_pub_key);

	EVP_MD_CTX *ctx = EVP_MD_CTX_new ();
	if (!ctx)
	{
		perror ("Error creating context");
		EVP_PKEY_free (pkey);
		return false;
	}

	if (EVP_DigestVerifyInit (ctx, NULL, EVP_sha256 (), NULL, pkey) != 1)
	{
		perror ("Error initializing verify context");
		EVP_MD_CTX_free (ctx);
		EVP_PKEY_free (pkey);
		return false;
	}

	if (EVP_DigestVerifyUpdate (ctx, message, strlen (message)) != 1)
	{
		perror ("Error updating verify context");
		EVP_MD_CTX_free (ctx);
		EVP_PKEY_free (pkey);
		return false;
	}

	tmp = EVP_DigestVerifyFinal (ctx, sign, *signLen);
	EVP_MD_CTX_free (ctx);
	EVP_PKEY_free (pkey);

	return (tmp == 1);
}



// test demo
int main (void)
{
	const char *path_rsa_key = "./private_key.pem", *rsa_path_pub_key = "./public_key.pem";
	RSA *rsa_priv_key, *rsa_pub_key;
	const char *message = "Hello, RSA!";//计算签名
	unsigned char sign[2048];//2048是RSA密钥长度
	long unsigned int signLen;

	//读取私钥
	rsa_priv_key = rsa_read_key (path_rsa_key, 1);
	if (!rsa_priv_key)
	{
		printf ("Error loading private key\n");
		return 1;
	}

	//对消息进行RSA签名
	if (rsa_sign_msg (message, rsa_priv_key, sign, &signLen))
	{
		printf ("Signature created successfully\n");
	}
	else
	{
		printf ("Error creating signature\n");
		RSA_free (rsa_priv_key);
		return -1;
	}

	//读取公钥
	rsa_pub_key = rsa_read_key (rsa_path_pub_key, 0);
	if (!rsa_pub_key)
	{
		printf ("Error loading public key\n");
		RSA_free (rsa_priv_key);
		return -1;
	}

	//验证签名
	if (verifySignature (message, rsa_pub_key, sign, &signLen))
		printf ("Signature verified successfully\n");
	else
		printf ("Error verifying signature\n");

	//释放资源
	RSA_free (rsa_priv_key);
	RSA_free (rsa_pub_key);
	return 0;
}



