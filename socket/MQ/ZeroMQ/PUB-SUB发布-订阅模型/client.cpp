#include <cstdio>
#include <iostream>
//必须手动安装cppzmq 库到/usr/local
#include <zmq.hpp>
#include <zmq_addon.hpp>



int main(void){
	const char str_filter[] = "";								//过滤器(一般不需要设置过滤器, 所以为""空字符串也挺好)
	zmq::message_t zmq_msg;
	zmq::context_t zmq_text(1);									//获取zmq上下文
	zmq::socket_t zmq_sock(zmq_text, ZMQ_SUB);	//创建SUB 订阅模型的zmq_sock

	zmq_sock.connect("tcp://127.0.0.1:6666");		//连接到zmq_sock.bind() 的PUB 发布端ip+port(tcp 协议)

	zmq_sock.set(zmq::sockopt::subscribe, zmq::const_buffer(str_filter, strlen(str_filter)));//设置过滤器

	//订阅消息loop
	while(1)
	{
		zmq_msg.rebuild(64);																									//每次都重置缓冲区, 用来接收新的消息(会清空缓冲区data)
		zmq_sock.recv(zmq_msg, zmq::recv_flags::none);												//阻塞, 循环接收消息
		printf("cli received: %s\n", reinterpret_cast<char*>(zmq_msg.data()));//打印接收到的消息(怎么准确打印出消息, 有待琢磨琢磨)
		//std::cout << "cli received: " << zmq_msg.to_string() << std::endl;	//(打印失败, 不知为何)
	}
	return 0;
}

