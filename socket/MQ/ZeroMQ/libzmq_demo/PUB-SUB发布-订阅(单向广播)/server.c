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
#include <string.h>
#include <zmq.h>
#include <assert.h>

int main(void)
{
	char buf_s[128] = {0};
	int tmp, count, sentBytes;
	void *zmq_text, *zmq_sock;

	zmq_text = zmq_ctx_new();
	zmq_sock = zmq_socket(zmq_text, ZMQ_PUB);

	tmp = zmq_bind(zmq_sock, "tcp://*:6666");
	assert(tmp == 0);
	tmp = zmq_bind(zmq_sock, "tcp://*:5555");//绑定另一个端口5555
	assert(tmp == 0);

	count = 0;//消息计数器
	while (1)
	{
		sleep(1);
		sprintf(buf_s, "[Server]: server = %d", count++);
		sentBytes = zmq_send(zmq_sock, buf_s, strlen(buf_s), 0);
		if (sentBytes > 0)
		{
			printf("[Server]: %s\n", buf_s);
		}
	}

	zmq_close(zmq_sock);
	zmq_ctx_destroy(zmq_text);
	return 0;
}

