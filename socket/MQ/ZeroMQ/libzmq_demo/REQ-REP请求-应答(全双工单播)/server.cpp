#include<iostream>
#include<zmq.h>
#include<Windows.h>

using namespace std;

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

		Sleep(1000);

		bytes = zmq_send(socket, sendBuf, strlen(sendBuf) + 1, 0);
		printf("[Server] ---<%d>--- sendMessage: %s   size = %d bytes\n", i, sendBuf, bytes);
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	system("pause");
	return 0;
}

