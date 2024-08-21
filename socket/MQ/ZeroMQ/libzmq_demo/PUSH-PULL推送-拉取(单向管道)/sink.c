//client（sink）

#include <stdio.h>
#include <zmq.h>
#include <assert.h>


int main(void)
{
	void *context = zmq_ctx_new();
	assert(context != NULL);

	void *socket = zmq_socket(context, ZMQ_PULL);
	assert(socket != NULL);

	int ret = zmq_bind(socket, "tcp://*:5555");
	assert(ret == 0);

	while (1)
	{
		char *resBuf[64] = { 0 };
		int bytes = zmq_recv(socket, resBuf, sizeof(resBuf), 0);
		if (bytes <= 0)
		{
			return -1;
		}
		printf("[Client] from server content == %s\n", resBuf);
	}

	return 0;
}

