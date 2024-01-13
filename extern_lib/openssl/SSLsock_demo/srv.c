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



#define MAXBUF (1023)



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

//根据ip+port, 创建一个普通的tcp listened sfd
static int get_listened_sfd(char* ip, uint16_t port, int li_wait_sfd_max){
	struct sockaddr_in addr;
	int sfd = -1;

	//bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	if (inet_aton(ip, (struct in_addr *) &addr.sin_addr.s_addr) == 0) {
		perror("inet_aton()");
		addr.sin_addr.s_addr = INADDR_ANY;//错误则强制使用0.0.0.0 混杂地址
	}

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket()");
		return -1;
	}

	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
		perror("bind()");
		close(sfd);
		return -1;
	}

	if (listen(sfd, li_wait_sfd_max) == -1) {
		perror("listen()");
		close(sfd);
		return -1;
	}
	else{
		printf("begin listen on %s:%d(li_wait_sfd_max=%d)\n",ip, port, li_wait_sfd_max);
		return sfd;
	}
}



int main(int argc, char **argv)
{
	int sfd, new_sfd;
	socklen_t len;
	struct sockaddr_in their_addr;
	unsigned int port, li_wait_sfd_max;
	char buf[MAXBUF + 1];
	SSL_CTX *ctx;
	SSL *ssl;
	char *path_ca, *path_key;

	if (argc != 6) {
		printf("参数格式错误！正确用法如下：\n/t/t%s IP地址 端口 监听wait_sfd_max 证书path 私钥path\n/t比如:/t%s 127.0.0.1 9213 64 ca.pem priv.key\n", argv[0], argv[0]);
		return -1;
	}
	//ip不用预处理
	//port 预处理
	if (argv[2])
		port = atoi(argv[2]);
	else
		port = 9213;
	//li_wait_sfd_max 预处理
	if (argv[3])
		li_wait_sfd_max = atoi(argv[3]);
	else
		li_wait_sfd_max = 64;
	//预处理证书path
	path_ca = argv[4];
	//预处理私钥path
	path_key = argv[5];

	//根据ip+port, 创建一个普通的tcp listened sfd
	sfd = get_listened_sfd(argv[1], port, li_wait_sfd_max);
	if (sfd == -1) {
		printf("get_listened_sfd() failed!!");
		return -1;
	}

	//初始化SSL 库的关键宏
	init_ssl();

	//以SSL V2和V3 标准兼容方式产生一个SSL_CTX, 即SSL Content Text; 也可以用SSLv2_server_method() 或SSLv3_server_method() 单独表示V2 或V3标准
	ctx = SSL_CTX_new(SSLv23_server_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);//打印SSL 错误到stdout
		close(sfd);
		return -1;
	}

	//载入用户的数字证书, 此证书用来发送给客户端. 证书里包含有公钥
	if (SSL_CTX_use_certificate_file(ctx, path_ca, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		kill_CTX(ctx,sfd);//释放CTX sfd 的固定流程
		return -1;
	}

	//载入用户私钥
	if (SSL_CTX_use_PrivateKey_file(ctx, path_key, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		kill_CTX(ctx,sfd);
		return -1;
	}

	//检查用户私钥是否正确
	if (!SSL_CTX_check_private_key(ctx)) {
		ERR_print_errors_fp(stdout);
		kill_CTX(ctx,sfd);
		return -1;
	}


	while (1) {
		len = sizeof(struct sockaddr);
		//等待客户端连上来
		if ((new_sfd = accept(sfd, (struct sockaddr *) &their_addr, &len)) == -1) {
			perror("accept()");
			kill_CTX(ctx,sfd);
			return -1;
		}
		else
			printf("server: got connection from %s, port %d, socket %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_sfd);

		ssl = SSL_new(ctx);						//基于ctx 产生一个新的 SSL
		SSL_set_fd(ssl, new_sfd);			//将连接用户的socket 加入到 SSL
		if (SSL_accept(ssl) == -1) {	//建立 SSL 连接
			perror("SSL_accept()");
			kill_SSL(new_sfd,ssl);			//ssl 已经创建, 杀死时切换为kill_SSL()
			break;
		}

		//发消息给客户端(阻塞)
		bzero(buf, MAXBUF + 1);
		strcpy(buf, "11111111111111111111111111111111111111\n");
		len = SSL_write(ssl, buf, strlen(buf));
		if (len <= 0) {
			printf("消息'%s'发送失败！错误代码是%d,错误信息是'%s'\n", buf, errno, strerror(errno));
			kill_SSL(new_sfd,ssl);
		}
		else
			printf("消息'%s'发送成功,共发送了%d个字节！\n",buf, len);

		//接收客户端的消息(阻塞)
		bzero(buf, MAXBUF + 1);
		len = SSL_read(ssl, buf, MAXBUF);
		if (len > 0)
			printf("接收消息成功:'%s',共%d个字节的数据\n",buf, len);
		else{
			printf("消息接收失败！错误代码是%d,错误信息是'%s'\n", errno, strerror(errno));
			kill_SSL(new_sfd,ssl);
		}

		//发送, 接收一轮后, 主动关闭SSLsock
		kill_SSL(new_sfd,ssl);//for test only
	}

	kill_CTX(ctx,sfd);
	return 0;
}
