//client(sink)

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
	void *zmq_text, *zmq_sock;
	int tmp, bytes;

	zmq_text = zmq_ctx_new();
	assert(zmq_text != NULL);

	zmq_sock = zmq_socket(zmq_text, ZMQ_PULL);
	assert(zmq_sock != NULL);

	tmp = zmq_bind(zmq_sock, "tcp://*:5555");
	assert(tmp == 0);

	while (1)
	{
		sleep(1);
		bytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r), 0);
		if (bytes <= 0)
		{
			printf ("zmq_recv(): %s\n", zmq_strerror (errno));
			return -1;
		}
		else
			printf("[sink] from server content == %s\n", buf_r);
	}

	return 0;
}

