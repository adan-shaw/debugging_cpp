//客户端2: 接收5555端口

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
#include <thread>
#include <zmq.h>
#include <assert.h>

void Recv3(void *arg)//客户端接收线程3
{
	char buf_r[128] = { 0 };
	int tmp, recvBytes;
	void *zmq_sock = arg;

	tmp = zmq_setsockopt(zmq_sock, ZMQ_SUBSCRIBE, "", 0);
	assert(tmp == 0);

	while (1)
	{
		sleep(1);
		recvBytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r) - 1, 0);
		if (recvBytes > 0)
		{
			printf("[Client]: Recv3--------------:%s\n", buf_r);
		}
	}
	return;
}

int main(void)
{
	char sendBuf[128] = {0};
	int tmp;
	void *zmq_text, *zmq_sock;

	zmq_text = zmq_ctx_new();
	zmq_sock = zmq_socket(zmq_text, ZMQ_SUB);

	tmp = zmq_connect(zmq_sock, "tcp://localhost:5555");
	assert(tmp == 0);

	std::thread th3(Recv3, zmq_sock);
	th3.join();

	zmq_close(zmq_sock);
	zmq_ctx_destroy(zmq_text);
	return 0;
}




