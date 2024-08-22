//worker1

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

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <zmq.h>
#include <errno.h>
#include <assert.h>


int main(void)
{
	char buf_s[64] = { 0 };
	void *zmq_text, *sender;
	int tmp, count, bytes;

	zmq_text = zmq_ctx_new();
	assert(zmq_text != NULL);

	sender = zmq_socket(zmq_text, ZMQ_PUSH);//ZMQ_PUSH = zmq_send() only
	assert(sender != NULL);

	tmp = zmq_connect(sender, "tcp://localhost:5555");//注意端口号, 也必须对齐info, ZMQ_PUSH/ZMQ_PULL
	assert(tmp == 0);

	count = 0;
	while (1)
	{
		sleep(1);
		sprintf(buf_s,"this if from ventilator's message-%d",count++);
		bytes = zmq_send(sender, buf_s, sizeof(buf_s), 0);
		if (bytes <= 0)
		{
			printf ("zmq_send(): %s\n", zmq_strerror (errno));
			return -1;
		}
		else
			printf("[worker1] Sended Reply Message content == %s\n", buf_s);
	}

	return 0;
}
