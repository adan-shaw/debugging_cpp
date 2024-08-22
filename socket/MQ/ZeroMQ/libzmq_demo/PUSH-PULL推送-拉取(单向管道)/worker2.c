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
#include <zmq.h>
#include <errno.h>
#include <assert.h>


int main(void)
{
	char *buf_r[64] = { 0 };
	void *zmq_text, *sender, *recver;
	int tmp, count, bytes;

	zmq_text = zmq_ctx_new();
	assert(zmq_text != NULL);

	recver = zmq_socket(zmq_text, ZMQ_PULL);//ZMQ_PULL = zmq_recv() only
	assert(recver != NULL);

	sender = zmq_socket(zmq_text, ZMQ_PUSH);//ZMQ_PUSH = zmq_send() only
	assert(sender != NULL);

	tmp = zmq_connect(recver, "tcp://localhost:6666");//注意端口号, 也必须对齐info, ZMQ_PUSH/ZMQ_PULL
	assert(tmp == 0);

	tmp = zmq_connect(sender, "tcp://localhost:5555");
	assert(tmp == 0);

	while (1)
	{
		sleep(1);
		bytes = zmq_recv(recver, buf_r, sizeof(buf_r), 0);
		if (bytes <= 0)
		{
			printf ("zmq_recv(): %s\n", zmq_strerror (errno));
			return -1;
		}
		else
			printf("[worker2 received]: %s\n", buf_r);

		bytes = zmq_send(sender, buf_r, sizeof(buf_r), 0);
		if (bytes <= 0)
		{
			printf ("zmq_send(): %s\n", zmq_strerror (errno));
			return -1;
		}
		else
			printf("[worker2 sended]: %s \n", buf_r);
	}

	return 0;
}

