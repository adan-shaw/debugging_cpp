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

#include <stdio.h>
#include <zmq.h>



int main(void)
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context, ZMQ_REP);
	zmq_bind(socket, "tcp://*:6000");

	int i = 1;//消息计数
	while (1)
	{
		char recvBuf[10] = { 0 };
		char *sendBuf = "world";

		int bytes = zmq_recv(socket, recvBuf, sizeof(recvBuf), 0);
		recvBuf[bytes] = '\0';
		printf("[Server] ---<%d>--- recvMessage: %s   size = %d bytes\n", i++, recvBuf, bytes);

		sleep(1000);

		bytes = zmq_send(socket, sendBuf, strlen(sendBuf) + 1, 0);
		printf("[Server] ---<%d>--- sendMessage: %s   size = %d bytes\n", i, sendBuf, bytes);
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	system("pause");
	return 0;
}

