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
#include <errno.h>
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
		//sleep(1);//接收是阻塞的, 不需要sleep(), 发布端才需要sleep(); zmq_recv() 第四个参数是阻塞参数, 和socket 一样的;
		recvBytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r) - 1, 0);
		if (recvBytes > 0)
		{
			printf("[Client2]: Recv3--------------:%s\n", buf_r);
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
	char sendBuf[128] = {0};
	int tmp;
	void *zmq_text, *zmq_sock;
	std::thread *pth3 = NULL;//posix 线程类, 一旦构造成对象, 就会自动启动posix 线程, 比较麻烦
													 //(可以用std::barrier 来协调多线程同步启动, 也可以用指针粗略做同步启动)

	zmq_text = zmq_ctx_new();
	zmq_sock = zmq_socket(zmq_text, ZMQ_SUB);

	tmp = zmq_connect(zmq_sock, "tcp://localhost:5555");
	assert(tmp == 0);

	pth3 = new std::thread(Recv3, zmq_sock);//调用重新构造 = 启动posix 线程
	pth3->join();
	delete pth3;

	zmq_close(zmq_sock);
	zmq_ctx_destroy(zmq_text);
	return 0;
}




