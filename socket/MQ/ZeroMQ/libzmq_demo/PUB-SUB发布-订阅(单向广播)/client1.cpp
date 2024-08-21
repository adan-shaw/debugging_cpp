//客户端1: 接收服务端 6666端口的消息
#include<iostream>
#include<thread>
#include<zmq.h>
#include<assert.h>
using namespace std;

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
		Sleep(1);
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
		Sleep(1);
		int recvBytes = zmq_recv(socket, recvBuf, sizeof(recvBuf)-1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv2--------------:%s\n", recvBuf);
		}
	}
}

