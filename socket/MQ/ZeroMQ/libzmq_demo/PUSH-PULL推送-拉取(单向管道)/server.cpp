//server(ventilator)

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<zmq.h>
#include<assert.h>
using namespace std;

int main(void)
{
	void *context = zmq_ctx_new();
	assert(context != NULL);

	void *sender = zmq_socket(context,ZMQ_PUSH);
	assert(sender != NULL);

	int ret = zmq_bind(sender, "tcp://*:6666");
	assert(ret == 0);

	printf("Pleas put down Enter,when the works are ready:\n");
	getchar();

	int i = 0;
	while (1)
	{
		Sleep(1000);
		char sendBuf[64] = { 0 };
		sprintf(sendBuf,"this if from ventilator's message-%d",i++);
		int bytes = zmq_send(sender, sendBuf, sizeof(sendBuf), 0);
		if (bytes <= 0)
		{
			return -1;
		}
		printf("[Server] Sended Reply Message content == %s\n", sendBuf);
	}

	return 0;
}

