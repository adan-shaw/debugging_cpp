//server(ventilator)
/*
	管道模式:
		服务器有一个req发送端, 一个rep 接收端(服务器既是req 请求端, 也是rep 结果接收端);
		客户端是req 执行端, 也是rep 结果生成端(结果发送端);

		服务器的req 请求端发出req 请求, 广播给客户端, 命令客户端去执行req;
		客户端执行完req 之后, 生成rep 执行结果, 一一单播结果给服务器的rep 端, 让服务器来进行逻辑验收(验证执行结果是否正确);
*/

//判断操作系统类型, 非目标平台, 终止编译
#if defined(_WIN32) || defined(_WIN64)
	#pragma message("Support Windows")
	#include <windows.h>
	//Sleep() 函数改名为: sleep(), 对齐unix
	#define sleep Sleep
#elif defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__) || defined(__unix) || defined(__unix__)
	#pragma message("Support Unix")
	#include <unistd.h>
#else
	#error [ Unknow System Type !! ]
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <zmq.h>
#include <errno.h>
#include <assert.h>


int main(void)
{
	char buf_s[64] = { 0 };
	void *zmq_text, *sender;
	int tmp, count, bytes;

	zmq_text = zmq_ctx_new();
	assert(zmq_text != NULL);

	sender = zmq_socket(zmq_text, ZMQ_PUSH);
	assert(sender != NULL);

	tmp = zmq_bind(sender, "tcp://*:6666");
	assert(tmp == 0);

	count = 0;
	while (1)
	{
		sleep(1);
		sprintf(buf_s,"this if from ventilator's message-%d",count++);
		bytes = zmq_send(sender, buf_s, sizeof(buf_s), 0);
		if (bytes <= 0)
		{
			printf ("zmq_send(): %s\n", zmq_strerror (errno));
			return -1;
		}
		else
			printf("[Server] Sended Reply Message content == %s\n", buf_s);
	}

	return 0;
}

