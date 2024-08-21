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
#include <assert.h>



int main(void)
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context,ZMQ_PUB);
	int ret = zmq_bind(socket, "tcp://*:6666");
	assert(ret == 0);
	ret = zmq_bind(socket, "tcp://*:5555");//绑定另一个端口5555
	assert(ret == 0);

	int i = 0;//消息计数器
	while (1)
	{
		sleep(10);
		char sendBuf[1024] = {0};
		sprintf(sendBuf, "[Server]: server = %d", i++);
		int sentBytes = zmq_send(socket, sendBuf, strlen(sendBuf), 0);
		if (sentBytes > 0)
		{
			printf("%s\n", sendBuf);
		}
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	system("pause");
	return 0;
}

