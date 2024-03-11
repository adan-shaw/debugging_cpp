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
	int sfd_sctp, sfd_sctp_cli, tmp, nRead;
	socklen_t len;
	struct sctp_status status;//获取客户端sfd_sctp_cli 套接字属性
	sctp_assoc_t associd = 0;
	struct sctp_sndrcvinfo sinfo;



	sfd_sctp = socket (AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (sfd_sctp < 0)
	{
		perror("socket()");
		return -1;
	}

	//设置sctp 专用控制
	initmsg.sinit_max_instreams = x_sinit_num_ostream;
	initmsg.sinit_num_ostreams = x_sinit_max_instream;
	initmsg.sinit_max_attempts = x_sinit_max_attempts;
	initmsg.sinit_max_init_timeo = x_sinit_max_init_timeo;

	if (setsockopt (sfd_sctp, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof (struct sctp_initmsg)) == -1)
	{
		perror("setsockopt()");
		close(sfd_sctp);
		return -1;
	}

	//bind sctp socket
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr (ip);
	serv_addr.sin_port = htons (port);

	tmp = 1;//只有一个addr sctp_bindx(), tmp = 1
	tmp = sctp_bindx (sfd_sctp, (struct sockaddr *) &serv_addr, tmp, SCTP_BINDX_ADD_ADDR);
	if (tmp == -1)
	{
		perror("sctp_bindx()");
		close(sfd_sctp);
		return -1;
	}

	//启动sctp 监听
	if (listen (sfd_sctp, listen_queue_max) == -1)
	{
		perror("listen()");
		close(sfd_sctp);
		return -1;
	}

	//等待client 链接
	for (;;)
	{
		//接受客户端accept
		len = sizeof (addr_cli);
		sfd_sctp_cli = accept (sfd_sctp, (struct sockaddr *) &addr_cli, &len);
		if (sfd_sctp_cli == -1)
		{
			perror("accept()");
			close(sfd_sctp);
			return -1;
		}

		memset (&status, 0, sizeof (status));
		status.sstat_assoc_id = associd;
		len = sizeof (status);
		if (getsockopt (sfd_sctp_cli, IPPROTO_SCTP, SCTP_STATUS, &status, &len) == -1)
		{
			perror("getsockopt()");
			close(sfd_sctp);
			return -1;
		}
		//打印套接字属性
		printf ("input stream:%d, output stream:%d\n", status.sstat_instrms, status.sstat_outstrms);

		//接收数据
		for (;;)
		{
			len = sizeof (&sinfo, sizeof (sinfo));
			nRead = sctp_recvmsg (sfd_sctp_cli, buf, buf_max, (struct sockaddr *) &addr_cli, &len, &sinfo, &tmp);
			if (nRead <= 0)
			{
				perror("sctp_recvmsg()");
				printf ("sctp_recvmsg(): sfd_sctp_cli=%d read all the data already, QUIT!!\n", sfd_sctp_cli);
				close (sfd_sctp_cli);
				break;
			}
			else
				printf ("server sctp_recvmsg() %d bytes data on channel %d:\n %s\n", nRead, sinfo.sinfo_stream, buf);
		}
	}

	return 0;
}
