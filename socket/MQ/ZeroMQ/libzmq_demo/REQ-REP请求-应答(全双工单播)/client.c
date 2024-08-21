#include <stdio.h>
#include <zmq.h>



int main(void)
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context, ZMQ_REQ);
	zmq_connect(socket, "tcp://localhost:6000");

	int i = 1;//消息计数
	for(int num = 0;num<10;num++)
	{
		char recvBuf[10] = { 0 }, *sendBuf = "hello";

		int bytes = zmq_send(socket, sendBuf, strlen(sendBuf) + 1, 0);
		printf("[Client] ---<%d>--- sendMessage: %s   size = %d bytes\n", i, sendBuf,bytes);

		bytes = zmq_recv(socket, recvBuf, sizeof(recvBuf), 0);
		recvBuf[bytes] = '\0';
		printf("[Client] ---<%d>--- recvMessage: %s   size = %d bytes\n", i++, recvBuf, bytes);
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	system("pause");
	return 0;
}

