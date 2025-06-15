#include <stdio.h>
#include <string.h>
#include <zmq.h>

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

int main(void)
{
	void *zmq_text, *zmq_sock;
	int count, num, bytes;
	char buf_r[16] = { 0 }, *buf_s = "hello cli";
	
	zmq_text = zmq_ctx_new();
	zmq_sock = zmq_socket(zmq_text, ZMQ_REP);

	zmq_bind(zmq_sock, "tcp://*:6000");

	count = 1;//消息计数
	while (1)
	{
		bytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r), 0);
		//buf_r[bytes] = '\0';
		printf("[Server] ---<%d>--- recvMessage: %s   size = %d bytes\n", count++, buf_r, bytes);

		sleep(1);//发送消息的间隔

		bytes = zmq_send(zmq_sock, buf_s, strlen(buf_s) + 1, 0);
		printf("[Server] ---<%d>--- sendMessage: %s   size = %d bytes\n", count, buf_s, bytes);
	}

	zmq_close(zmq_sock);
	zmq_ctx_destroy(zmq_text);
	return 0;
}

