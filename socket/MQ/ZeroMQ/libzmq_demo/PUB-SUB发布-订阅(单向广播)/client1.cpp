//客户端1: 接收服务端 6666端口的消息

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
#include <errno.h>
#include <assert.h>

//客户端接收线程1
void Recv1(void *arg)
{
	char buf_r[128] = { 0 };
	int recvBytes;
	void *zmq_sock = arg;

	while(1)
	{
		//sleep(1);
		recvBytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r) - 1, 0);
		if (recvBytes > 0)
		{
			printf("[Client1]: Recv1--------------:%s\n", buf_r);
		}
		else{
			printf ("zmq_recv(): %s\n", zmq_strerror (errno));
			break;
		}
	}
	return;
}

//客户端接收线程2
void Recv2(void *arg)
{
	char buf_r[128] = { 0 };
	int recvBytes;
	void *zmq_sock = arg;

	while (1)
	{
		//sleep(1);
		recvBytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r) - 1, 0);
		if (recvBytes > 0)
		{
			printf("[Client1]: Recv2--------------:%s\n", buf_r);
		}
		else{
			printf ("zmq_recv(): %s\n", zmq_strerror (errno));
			break;
		}
	}
	return;
}

int main(void)
{
	void *zmq_text, *zmq_sock;
	int tmp;
	std::thread *pth1 = NULL, *pth2 = NULL;

	zmq_text = zmq_ctx_new();
	assert(zmq_text != NULL);

	zmq_sock = zmq_socket(zmq_text,ZMQ_SUB);
	assert(zmq_sock != NULL);

	tmp = zmq_connect(zmq_sock, "tcp://localhost:6666");
	assert(tmp == 0);

	tmp = zmq_setsockopt(zmq_sock, ZMQ_SUBSCRIBE, "[Server]", 8);
	assert(tmp == 0);

	pth1 = new std::thread(Recv1,zmq_sock);
	pth2 = new std::thread(Recv2,zmq_sock);

	pth1->join();
	pth2->join();

	delete pth1;
	delete pth2;

	zmq_close(zmq_sock);
	zmq_ctx_destroy(zmq_text);
	return 0;
}



