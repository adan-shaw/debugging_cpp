#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>



#define MAXBUF (1023)



//显示pem 证书的内容
void ShowCerts(SSL * ssl)
{
	char *line;
	X509 *cert = SSL_get_peer_certificate(ssl);										//获取证书内容
	if (cert != NULL) {
		printf("* 数字证书内容 *\n");

		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);//获取证书内容->证书本体
		printf("证书: %s\n", line);
		free(line);

		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);	//获取证书内容->颁发者
		printf("颁发者: %s\n", line);
		free(line);

		X509_free(cert);																						//释放证书内容
	}
	else
		printf("无证书信息！\n");
}



//SSL 库初始化(client/server 通用的固定顺序, 使用SSL 库前都必须先初始化, 只需要初始化一次)
static void init_ssl(void){
	SSL_library_init();						//SSL_Init();
	OpenSSL_add_all_algorithms();	//载入所有 SSL 算法
	SSL_load_error_strings();			//载入所有 SSL 错误消息
}

//关闭SSL + CTX + sfd 的固定顺序
static void kill_ALL(SSL_CTX *ctx, int sfd, SSL *ssl){
	SSL_shutdown(ssl);						//关闭SSL 连接
	SSL_free(ssl);								//释放SSL
	close(sfd);										//关闭socket
	SSL_CTX_free(ctx);						//释放CTX
}

//创建SSL 时失败, 关闭CTX + sfd 的固定顺序
static void kill_CTX(SSL_CTX *ctx, int sfd){
	close(sfd);										//关闭socket
	SSL_CTX_free(ctx);						//释放CTX
}

//运行时, SSL io 错误时, 关闭单个SSL 的固定顺序
static void kill_SSL(int sfd, SSL *ssl){
	SSL_shutdown(ssl);						//关闭SSL 连接
	SSL_free(ssl);								//释放SSL
	close(sfd);										//关闭socket
}

//根据ip+port, 创建一个普通的tcp connected sfd
static int get_connected_sfd(char* ip, uint16_t port){
	struct sockaddr_in dest;
	int sfd = -1;

	//bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = port;
	if (inet_aton(ip, (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
		perror("inet_aton()");
		return -1;
	}

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		perror("socket()");
		return -1;
	}

	if (connect(sfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
		perror("connect()");
		close(sfd);
		return -1;
	}
	else{
		printf("socket connected to server %s:%d\n", ip, port);
		return sfd;
	}
}



int main(int argc, char **argv)
{
	int sfd, len;
	char buffer[MAXBUF + 1];
	SSL_CTX *ctx;
	SSL *ssl;

	if (argc != 3) {
		printf("参数格式错误！正确用法如下：\n/t/t%s IP地址 端口\n/t比如:/t%s 127.0.0.1 9213\n", argv[0], argv[0]);
		return -1;
	}

	//初始化SSL 库的关键宏
	init_ssl();

	//以SSL V2和V3 标准兼容方式产生一个SSL_CTX, 即SSL Content Text; 也可以用SSLv2_server_method() 或SSLv3_server_method() 单独表示V2 或V3标准
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);//打印SSL 错误到stdout
		close(sfd);
		return -1;
	}

	//根据ip+port, 创建一个普通的tcp connected sfd
	sfd = get_connected_sfd(argv[1], htons(atoi(argv[2])));
	if (sfd == -1) {
		printf("get_connected_sfd() failed!!");
		return -1;
	}

	//基于ctx 产生一个新的SSL
	ssl = SSL_new(ctx);

	//把SSL 绑定到sfd 中(SSL 需要指定sfd 后, 才能正常使用)
	SSL_set_fd(ssl, sfd);

	//建立SSL 连接
	if (SSL_connect(ssl) == -1){
		perror("SSL_connect()");
		kill_ALL(ctx,sfd,ssl);//关闭SSL + CTX + sfd 的固定顺序
		return -1;
	}
	else{
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
	}

	//接收对方发过来的消息, 最多接收MAXBUF 个字节(阻塞)
	bzero(buffer, MAXBUF + 1);
	len = SSL_read(ssl, buffer, MAXBUF);
	if (len > 0)
		printf("接收消息成功:'%s',共%d个字节的数据\n",buffer, len);
	else
	{
		printf("消息接收失败！错误代码是%d,错误信息是'%s'\n",errno, strerror(errno));
		kill_ALL(ctx,sfd,ssl);
		return -1;
	}

	//发消息给服务器(阻塞)
	bzero(buffer, MAXBUF + 1);
	strcpy(buffer, "hello server, i'am client -_-");
	len = SSL_write(ssl, buffer, strlen(buffer));
	if (len < 0)
		printf("消息'%s'发送失败！错误代码是%d,错误信息是'%s'\n", buffer, errno, strerror(errno));
	else
		printf("消息'%s'发送成功,共发送了%d个字节！\n", buffer, len);

	kill_ALL(ctx,sfd,ssl);
	return 0;
}
