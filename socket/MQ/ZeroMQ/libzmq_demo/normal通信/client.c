#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <zmq.h>

int main(void){
	const char* addr_info = "tcp://127.0.0.1:8888";//使用tcp协议, 目标IP/PORT为127.0.0.1:8888
	char szMsg[128] = {0};
	int tmp;
	void *pCtx = NULL, *pSock = NULL;

	//创建context
	if((pCtx = zmq_ctx_new()) == NULL){
		printf ("zmq_ctx_new(): %s\n", zmq_strerror (errno));
		return -1;
	}

	//创建socket(zmq socket目前有6中属性, 这里使用dealer方式)
	if((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL){
		printf ("zmq_socket(): %s\n", zmq_strerror (errno));
		zmq_ctx_destroy(pCtx);
		return -1;
	}

	//设置发送超时(2s)
	tmp = 2000;// millisecond 毫秒
	zmq_setsockopt(pSock, ZMQ_SNDTIMEO, &tmp, sizeof(tmp));

	//连接目标IP 127.0.0.1, 端口8888 
	if(zmq_connect(pSock, addr_info) < 0){
		printf ("zmq_connect(): %s\n", zmq_strerror (errno));
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return -1;
	}

	//循环发送消息(间隔1s) -- server 不在线, 仍然能继续发送, 很神奇(可能跟选择zmq socket 类型有关)
	tmp = 0;
	while(1){
		snprintf(szMsg, sizeof(szMsg), "hello world : %3d", tmp++);
		if(zmq_send(pSock, szMsg, sizeof(szMsg), 0) < 0){
			printf ("zmq_send(): %s\n", zmq_strerror (errno));
			break;//zmq_send()超时会报错: Resource temporarily unavailable (zmq 是默认异步的, 不能用死等的老式socket 方式对待)
			//continue;//但一般情况下, 发送不可能超时
		}
		else
			printf("client send msg: [%s]\n", szMsg);
		sleep(1);
	}

	zmq_close(pSock);
	zmq_ctx_destroy(pCtx);
	return 0;
}
