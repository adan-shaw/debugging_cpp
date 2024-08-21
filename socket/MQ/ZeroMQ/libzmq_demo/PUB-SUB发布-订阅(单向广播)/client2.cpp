//客户端2: 接收5555端口
#include<iostream>
#include<thread>
#include<zmq.h>
#include<assert.h>
using namespace std;

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
		Sleep(1);
		int recvBytes = zmq_recv(socket, recvBuf, sizeof(recvBuf)-1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv3--------------:%s\n", recvBuf);
		}
	}
}

