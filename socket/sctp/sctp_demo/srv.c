//编译
//		gcc -g3 ./srv.c -lsctp -o srv

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>

#define listen_queue_max (32)
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
	struct sockaddr_in serv_addr, addr_cli;
	int sctp_sfd, sctp_cfd, tmp, nRead;
	socklen_t len;
	struct sctp_status status;//获取客户端sctp_cfd 套接字属性
	sctp_assoc_t associd = 0;
	struct sctp_sndrcvinfo sinfo;



	sctp_sfd = socket (AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (sctp_sfd < 0)
	{
		perror("socket()");
		return -1;
	}

	//设置sctp 专用控制
	initmsg.sinit_max_instreams = x_sinit_num_ostream;
	initmsg.sinit_num_ostreams = x_sinit_max_instream;
	initmsg.sinit_max_attempts = x_sinit_max_attempts;
	initmsg.sinit_max_init_timeo = x_sinit_max_init_timeo;

	tmp = setsockopt (sctp_sfd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof (initmsg));
	if (tmp == -1)
	{
		perror("setsockopt()");
		close(sctp_sfd);
		return -1;
	}

	//bind sctp socket
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr (ip);
	serv_addr.sin_port = htons (port);

	tmp = 1;//只有一个addr, tmp = 1
	tmp = sctp_bindx (sctp_sfd, (struct sockaddr *) &serv_addr, tmp, SCTP_BINDX_ADD_ADDR);
	if (tmp == -1)
	{
		perror("sctp_bindx()");
		close(sctp_sfd);
		return -1;
	}

	//启动sctp 监听
	if (listen (sctp_sfd, listen_queue_max) == -1)
	{
		perror("listen()");
		close(sctp_sfd);
		return -1;
	}

	//等待client 链接
	for (;;)
	{
		//接受客户端accept
		len = sizeof (addr_cli);
		sctp_cfd = accept (sctp_sfd, (struct sockaddr *) &addr_cli, &len);
		if (sctp_cfd == -1)
		{
			perror("accept()");
			close(sctp_sfd);
			return -1;
		}

		memset (&status, 0, sizeof (status));
		status.sstat_assoc_id = associd;
		len = sizeof (status);
		if (getsockopt (sctp_cfd, IPPROTO_SCTP, SCTP_STATUS, &status, &len) == -1)
		{
			perror("getsockopt()");
			close(sctp_sfd);
			return -1;
		}
		//打印套接字属性
		printf ("input stream:%d, output stream:%d\n", status.sstat_instrms, status.sstat_outstrms);

		//接收数据
		for (;;)
		{
			len = sizeof (&sinfo, sizeof (sinfo));
			nRead = sctp_recvmsg (sctp_cfd, buf, buf_max, (struct sockaddr *) &addr_cli, &len, &sinfo, &tmp);
			if (nRead <= 0)
			{
				perror("sctp_recvmsg()");
				printf ("sctp_recvmsg(): sctp_cfd=%d read all the data already, QUIT!!\n", sctp_cfd);
				close (sctp_cfd);
				break;
			}
			else
				printf ("server sctp_recvmsg() %d bytes data on channel %d:\n %s\n", nRead, sinfo.sinfo_stream, buf);
		}
	}

	return 0;
}
