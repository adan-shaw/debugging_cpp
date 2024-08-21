#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main(void)
{
	void *zmq_text, *zmq_sock;
	int count, num, bytes;
	char buf_r[16] = { 0 }, *buf_s = "hello srv";

	zmq_text = zmq_ctx_new();									//创建'zmq_text 上下文'
	zmq_sock = zmq_socket(zmq_text, ZMQ_REQ);	//在'zmq_text 上下文'中, 创建zmq_sock

	zmq_connect(zmq_sock, "tcp://localhost:6000");

	count = 1;//消息计数
	for(num = 0;num<10;num++)
	{
		bytes = zmq_send(zmq_sock, buf_s, strlen(buf_s) + 1, 0);
		printf("[Client] ---<%d>--- sendMessage: %s   size = %d bytes\n", count, buf_s, bytes);

		bytes = zmq_recv(zmq_sock, buf_r, sizeof(buf_r), 0);
		//buf_r[bytes] = '\0';//防止字符串不终结? (这个可能没必要)
		printf("[Client] ---<%d>--- recvMessage: %s   size = %d bytes\n", count++, buf_r, bytes);
	}

	zmq_close(zmq_sock);											//关闭zmq_sock
	zmq_ctx_destroy(zmq_text);								//释放'zmq_text 上下文'
	return 0;
}

