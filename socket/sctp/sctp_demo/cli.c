//编译
//		gcc -g3 ./cli.c -lsctp -o cli

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>

#define buf_max (1024)

#define x_sinit_num_ostream (4096)		// 单一个sctp socket 绑定下 4096 个客户端回应
#define x_sinit_max_instream (4096)		// 4096 个客户端链接
#define x_sinit_max_attempts (2)			//重传2 次失败, 就表示不可达, 不再传送
#define x_sinit_max_init_timeo (2000)	//重传超时, 毫秒单位

const char *ip = "127.0.0.1";
const int port = 2013;



int main (void)
{
	char buf[buf_max];
	struct sctp_initmsg initmsg;
	struct sockaddr_in serv_addr;
	socklen_t len;
	struct sctp_status status;//获取客户端sctp_cfd 套接字属性
	sctp_assoc_t associd = 0;
	struct sctp_sndrcvinfo sinfo;



	int sctp_cfd = socket (AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (sctp_cfd < 0)
	{
		perror("socket()");
		return -1;
	}

	//设置sctp 专用控制
	initmsg.sinit_max_instreams = x_sinit_num_ostream;
	initmsg.sinit_num_ostreams = x_sinit_max_instream;
	initmsg.sinit_max_attempts = x_sinit_max_attempts;
	initmsg.sinit_max_init_timeo = x_sinit_max_init_timeo;

	if (setsockopt (sctp_cfd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof (initmsg)) == -1)
	{
		perror("setsockopt()");
		close(sctp_cfd);
		return -1;
	}

	//连接sctp 服务器
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr (ip);
	serv_addr.sin_port = htons (port);

	if (sctp_connectx (sctp_cfd, (struct sockaddr *) &serv_addr, 1, &associd) < 0)
	{
		perror("sctp_connectx()");
		close(sctp_cfd);
		return -1;
	}

	//反向获取client sctp_cfd 属性
	status.sstat_assoc_id = associd;
	len = sizeof (status);
	if (getsockopt (sctp_cfd, IPPROTO_SCTP, SCTP_STATUS, &status, &len) == -1)
	{
		perror("getsockopt()");
		close(sctp_cfd);
		return -1;
	}
	//打印套接字属性
	printf ("input stream:%d, output stream:%d\n", status.sstat_instrms, status.sstat_outstrms);

	//发送数据到服务器
	bzero (&sinfo, sizeof (sinfo));
	sinfo.sinfo_stream = 123;//随机数据?
	snprintf (buf, buf_max, "hi server!!");

	//执行发送
	sctp_send (sctp_cfd, buf, strlen (buf), &sinfo, 0);
	sinfo.sinfo_flags = SCTP_EOF;
	sctp_send (sctp_cfd, NULL, 0, &sinfo, 0);

	close (sctp_cfd);
	return 0;
}
