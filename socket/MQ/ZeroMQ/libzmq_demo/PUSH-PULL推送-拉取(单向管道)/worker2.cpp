#include<iostream>
#include<zmq.h>
#include<assert.h>
using namespace std;

int main(void)
{
	void *context = zmq_ctx_new();
	assert(context != NULL);

	void *recver = zmq_socket(context, ZMQ_PULL);
	assert(recver != NULL);

	void *sender = zmq_socket(context, ZMQ_PUSH);
	assert(sender != NULL);

	int ret = zmq_connect(recver, "tcp://localhost:6666");
	assert(ret == 0);

	ret = zmq_connect(sender, "tcp://localhost:5555");
	assert(ret == 0);

	while (1)
	{
		char *recvBuf[64] = { 0 };
		int bytes = zmq_recv(recver, recvBuf, sizeof(recvBuf), 0);
		if (bytes <= 0)
		{
			return -1;
		}
		printf("[Recver]: %s\n", recvBuf);

		bytes = zmq_send(sender, recvBuf, sizeof(recvBuf), 0);
		if (bytes <= 0)
		{
			return -1;
		}
		printf("[Recver]: %s \n", recvBuf);
	}

	return 0;
}

