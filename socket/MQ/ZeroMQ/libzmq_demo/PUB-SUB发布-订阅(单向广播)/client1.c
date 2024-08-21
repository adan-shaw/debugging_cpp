//客户端1: 接收服务端 6666端口的消息

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



void Recv1(void *arg);//客户端接收线程1
void Recv2(void *arg);//客户端接收线程2

int main(void)
{
	void *context = zmq_ctx_new();
	assert(context != NULL);

	void *socket = zmq_socket(context,ZMQ_SUB);
	assert(socket != NULL);

	int ret = zmq_connect(socket, "tcp://localhost:6666");
	assert(ret == 0);

	ret = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "[Server]", 8);
	assert(ret == 0);

	thread th1(Recv1,socket);
	thread th2(Recv2,socket);

	th1.join();
	th2.join();

	zmq_close(socket);
	zmq_ctx_destroy(context);

	system("pause");
	return 0;
}

void Recv1(void *arg)//客户端接收线程1
{
	void *socket = arg;

	while(1)
	{
		char recvBuf[1024] = { 0 };
		sleep(1);
		int recvBytes = zmq_recv(socket, recvBuf, sizeof(recvBuf)-1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv1--------------:%s\n", recvBuf);
		}
	}
}

void Recv2(void *arg)//客户端接收线程2
{
	void *socket = arg;

	while (1)
	{
		char recvBuf[1024] = { 0 };
		sleep(1);
		int recvBytes = zmq_recv(socket, recvBuf, sizeof(recvBuf)-1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv2--------------:%s\n", recvBuf);
		}
	}
}

