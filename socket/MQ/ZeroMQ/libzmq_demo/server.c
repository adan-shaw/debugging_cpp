#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <zmq.h>

//只接收10 条消息, 就自动退出, 只做测试
static unsigned short msg_count = 0;

int main(void){
	char szMsg[128] = {0};
	const char* addr_info = "tcp://*:8888";//使用tcp协议, 目标IP/PORT为0.0.0.0:8888
	int tmp;
	void * pCtx = NULL;
	void * pSock = NULL;

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

	//设置接收超时(2s)
	tmp = 2000;// millisecond 毫秒
	zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &tmp, sizeof(tmp));

	//绑定地址 tcp://0.0.0.0:8888
	if(zmq_bind(pSock, addr_info) < 0){
		printf ("zmq_bind(): %s\n", zmq_strerror (errno));
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	else
		printf("bind at: %s\n", addr_info);

	//循环接收消息(间隔1s)
	while(1){
		if(zmq_recv(pSock, szMsg, sizeof(szMsg), 0) < 0){
			printf ("zmq_recv(): %s\n", zmq_strerror (errno));
			//break;//zmq_recv()超时会报错: Resource temporarily unavailable (zmq 是默认异步的, 不能用死等的老式socket 方式对待)
			continue;//接收可能会超时
		}
		else{
			printf("server received msg: %s\n", szMsg);
			msg_count++;
		}
		if(msg_count > 10)
			break;
		sleep(1);
	}

	zmq_close(pSock);
	zmq_ctx_destroy(pCtx);
	return 0;
}
