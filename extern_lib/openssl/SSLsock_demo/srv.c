#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXBUF (4096)

//SSL 库初始化(client/server 通用的固定顺序, 使用SSL 库前都必须先初始化, 只需要初始化一次)
void init_ssl (void)
{
	SSL_library_init ();					//SSL_Init();
	OpenSSL_add_all_algorithms ();//载入所有 SSL 算法
	SSL_load_error_strings ();		//载入所有 SSL 错误消息
}

//根据ip+port, 创建一个普通的tcp listened sfd
int get_listened_sfd (char *ip, uint16_t port, int li_wait_sfd_max)
{
	struct sockaddr_in addr;
	int sfd = -1;

	//bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons (port);
	if (inet_aton (ip, (struct in_addr *) &addr.sin_addr.s_addr) == 0)
	{
		perror ("inet_aton()");
		addr.sin_addr.s_addr = INADDR_ANY;	//错误则强制使用0.0.0.0 混杂地址
	}

	if ((sfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("socket()");
		return -1;
	}

	if (bind (sfd, (struct sockaddr *) &addr, sizeof (struct sockaddr)) == -1)
	{
		perror ("bind()");
		close (sfd);
		return -1;
	}

	listen (sfd, li_wait_sfd_max);
	printf ("begin listen on %s:%d(li_wait_sfd_max=%d)\n", ip, port, li_wait_sfd_max);
	return sfd;
}

int main (int argc, char **argv)
{
	int sfd, sfd_new;
	socklen_t len;
	struct sockaddr_in addr_cli;
	unsigned int port, li_wait_sfd_max;
	char buf[MAXBUF + 1];
	SSL_CTX *ctx;
	SSL *ssl;
	char *ip, *path_ca, *path_key;

	if (argc != 6)
	{
		printf ("参数格式错误!正确用法如下:\n/t/t%s IP地址 端口 监听wait_sfd_max 证书path 私钥path\n/t比如:/t%s 127.0.0.1 9213 64 ca.pem priv.key\n", argv[0], argv[0]);
		return -1;
	}

	//ip 预处理
	ip = argv[1];
	//port 预处理
	if (argv[2])
		port = atoi (argv[2]);
	else
		port = 9213;
	//li_wait_sfd_max 预处理
	if (argv[3])
		li_wait_sfd_max = atoi (argv[3]);
	else
		li_wait_sfd_max = 64;
	//预处理证书path
	path_ca = argv[4];
	//预处理私钥path
	path_key = argv[5];

	//SSL 库初始化
	init_ssl ();

	//以SSLV2 和V3 标准兼容方式产生一个SSL_CTX , 即SSL Content Text; 也可以用SSLv2_server_method() 或SSLv3_server_method() 单独表示V2 或V3 标准
	ctx = SSL_CTX_new (SSLv23_server_method ());
	if (ctx == NULL)
	{
		ERR_print_errors_fp (stdout);//打印SSL 错误到stdout
		return -1;
	}

	//载入CA 数字证书
	if (SSL_CTX_use_certificate_file (ctx, path_ca, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp (stdout);
		SSL_CTX_free (ctx);
		return -1;
	}

	//载入私钥
	if (SSL_CTX_use_PrivateKey_file (ctx, path_key, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp (stdout);
		SSL_CTX_free (ctx);
		return -1;
	}

	//检查用户私钥是否正确
	if (!SSL_CTX_check_private_key (ctx))
	{
		ERR_print_errors_fp (stdout);
		SSL_CTX_free (ctx);
		return -1;
	}

	//根据ip+port, 创建一个普通的tcp listened sfd
	sfd = get_listened_sfd (ip, port, li_wait_sfd_max);
	if (sfd == -1)
	{
		printf ("get_listened_sfd() failed!!");
		SSL_CTX_free (ctx);
		return -1;
	}

	while (1)
	{
		len = sizeof (struct sockaddr);
		//等待客户端连上来
		if ((sfd_new = accept (sfd, (struct sockaddr *) &addr_cli, &len)) == -1)
		{
			perror ("accept");
			goto pos_error;
		}
		else
			printf ("server: got connection from %s, port %d, socket %d\n", inet_ntoa (addr_cli.sin_addr), ntohs (addr_cli.sin_port), sfd_new);

		//基于ctx 产生一个新的SSL 通道
		ssl = SSL_new (ctx);

		//SSL 通道绑定sfd_new, 模式=读写模式
		SSL_set_fd (ssl, sfd_new);
		//SSL 通道绑定sfd_new, 模式=只读模式
		//SSL_set_rfd (ssl, sfd_new);
		//SSL 通道绑定sfd_new, 模式=只写模式
		//SSL_set_wfd (ssl, sfd_new);

		//建立 SSL 连接
		if (SSL_accept (ssl) == -1)
		{
			perror ("accept");
			goto pos_error;
		}

		//发消息给客户端
		//bzero (buf, MAXBUF + 1);
		strcpy (buf, "server->client: hello world");
		len = SSL_write (ssl, buf, strlen (buf));
		if (len <= 0)
		{
			printf ("srv: 消息'%s'发送失败! 错误代码是%d, 错误信息是'%s'\n", buf, errno, strerror (errno));
			goto pos_finish;
		}
		else
			printf ("srv: 消息'%s'发送成功, 共发送了%d个字节!!\n", buf, len);

		//接收客户端的消息
		bzero (buf, MAXBUF + 1);
		len = SSL_read (ssl, buf, MAXBUF);
		if (len > 0)
			printf ("srv: 接收消息成功:'%s', 共%d个字节的数据\n", buf, len);
		else
			printf ("srv: 消息接收失败! 错误代码是%d, 错误信息是'%s'\n", errno, strerror (errno));

	pos_finish:
		SSL_shutdown (ssl);					//关闭SSL 连接
		SSL_free (ssl);							//释放SSL
		close (sfd_new);						//关闭socket
	}
pos_error:
	close (sfd);									//关闭socket
	SSL_CTX_free (ctx);						//释放CTX
	return 0;
}
