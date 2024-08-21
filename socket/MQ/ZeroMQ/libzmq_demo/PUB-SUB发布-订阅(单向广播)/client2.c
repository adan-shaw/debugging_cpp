//客户端2: 接收5555端口

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
#include <thread>
#include <zmq.h>
#include <assert.h>


void Recv3(void *arg);//客户端接收线程3

int main(void)
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context, ZMQ_SUB);
	int ret = zmq_connect(socket, "tcp://localhost:5555");
	assert(ret == 0);

	thread th3(Recv3, socket);

	th3.join();

	zmq_close(socket);
	zmq_ctx_destroy(context);
	system("pause");
	return 0;
}


void Recv3(void *arg)//客户端接收线程3
{
	void *socket = arg;
	int ret = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
	assert(ret == 0);

	while (1)
	{
		char recvBuf[1024] = { 0 };
		sleep(1);
		int recvBytes = zmq_recv(socket, recvBuf, sizeof(recvBuf)-1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv3--------------:%s\n", recvBuf);
		}
	}
}

