#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<zmq.h>
#include<assert.h>

using namespace std;

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
		Sleep(10);
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

