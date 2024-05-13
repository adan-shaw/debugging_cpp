//编译:
//		gcc -g3 ./conn_https.c -lssl -lcrypto -o x



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

#define MAXBUF (4096)
#define HTTP_REQUEST "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"

//打印CA证书信息(提取CA证书信息)
void ShowCerts (SSL * ssl)
{
	X509 *cert;
	char *line;

	printf ("CA数字证书信息:\n");
	cert = SSL_get_peer_certificate (ssl);
	if (cert != NULL)
	{
		line = X509_NAME_oneline (X509_get_subject_name (cert), 0, 0);
		printf ("证书: %s\n", line);
		free (line);

		line = X509_NAME_oneline (X509_get_issuer_name (cert), 0, 0);
		printf ("颁发者: %s\n", line);
		free (line);

		X509_free (cert);
	}
	else
		printf ("无证书信息!\n");
	return;
}

//SSL 库初始化(client/server 通用的固定顺序, 使用SSL 库前都必须先初始化, 只需要初始化一次)
void init_ssl (void)
{
	SSL_library_init ();					//SSL_Init();
	OpenSSL_add_all_algorithms ();//载入所有 SSL 算法
	SSL_load_error_strings ();		//载入所有 SSL 错误消息
}

//根据ip+port, 创建一个普通的tcp connected sfd
int get_connected_sfd (char *ip, uint16_t port)
{
	struct sockaddr_in dest;
	int sfd = -1;

	//bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons (port);
	if (inet_aton (ip, (struct in_addr *) &dest.sin_addr.s_addr) == 0)
	{
		perror ("inet_aton()");
		return -1;
	}

	sfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
	{
		perror ("socket()");
		return -1;
	}

	if (connect (sfd, (struct sockaddr *) &dest, sizeof (dest)) == -1)
	{
		perror ("connect()");
		//printf("port = %d", port);//debugging only
		close (sfd);
		return -1;
	}
	else
	{
		printf ("socket connected to server %s:%d\n", ip, port);
		return sfd;
	}
}

int main (int argc, char **argv)
{
	int sfd, len;
	char buffer[MAXBUF + 1];
	SSL_CTX *ctx;
	SSL *ssl;

	if (argc != 3)
	{
		printf ("参数格式错误!正确用法如下：\n\t\t%s IP地址 端口\n\t比如:\t%s 127.0.0.1 443\n", argv[0], argv[0]);
		return -1;
	}

	//SSL 库初始化
	init_ssl ();

	ctx = SSL_CTX_new (SSLv23_client_method ());
	if (ctx == NULL)
	{
		ERR_print_errors_fp (stdout);//打印SSL 错误到stdout
		return -1;
	}

	//根据ip+port, 创建一个普通的tcp connected sfd
	sfd = get_connected_sfd (argv[1], atoi (argv[2]) );
	if (sfd == -1)
	{
		printf ("get_connected_sfd() failed!!");
		SSL_CTX_free (ctx);
		return -1;
	}

	//基于ctx 产生一个新的SSL 通道
	ssl = SSL_new (ctx);

	//SSL 通道绑定sfd, 模式=读写模式
	SSL_set_fd (ssl, sfd);
	//SSL 通道绑定sfd, 模式=只读模式
	//SSL_set_rfd (ssl, sfd);
	//SSL 通道绑定sfd, 模式=只写模式
	//SSL_set_wfd (ssl, sfd);

	//建立SSL 连接
	if (SSL_connect (ssl) == -1)
	{
		ERR_print_errors_fp (stderr);
		goto pos_error;
	}
	else
	{
		printf ("Connected with %s encryption\n", SSL_get_cipher (ssl));
		ShowCerts (ssl);
	}

	//发消息给服务器(需要填写http 请求命令, 详情请阅读http 协议)
	strncpy (buffer, HTTP_REQUEST, strlen (HTTP_REQUEST));
	len = SSL_write (ssl, buffer, strlen (buffer));
	if (len < 0)
	{
		printf ("cli: 消息发送失败!! 错误代码是%d, 错误信息是%s\n\n\n", errno, strerror (errno));
		goto pos_error;
	}
	else
		printf ("cli: 消息发送成功, 共发送了%d个字节!! 数据如下:\n%s\n\n\n", len, buffer);

	//接收对方发过来的消息, 最多接收 MAXBUF 个字节
	bzero (buffer, MAXBUF + 1);
	len = SSL_read (ssl, buffer, MAXBUF);
	if (len > 0)
		printf ("cli: 接收消息成功, 共%d个字节的数据!! 数据如下:\n%s\n\n\n", len, buffer);
	else
	{
		printf ("cli: 消息接收失败!! 错误代码是%d, 错误信息是%s\n\n\n", errno, strerror (errno));
		goto pos_error;
	}

pos_error:
	SSL_shutdown (ssl);						//关闭SSL 连接
	SSL_free (ssl);								//释放SSL

	close (sfd);									//关闭socket
	SSL_CTX_free (ctx);						//释放CTX
	return 0;
}
