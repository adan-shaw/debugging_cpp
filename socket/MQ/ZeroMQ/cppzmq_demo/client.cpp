#include <iostream>
#include <string>
//必须手动安装cppzmq 库到/usr/local
#include <zmq.hpp>
#include <zmq_addon.hpp>



int main(void){
	const char topic[] = "333";
	zmq::message_t msg;
	zmq::context_t zmq_context(1);									//获取zmq上下文
	zmq::socket_t zmq_socket(zmq_context,ZMQ_SUB);	//创建ZMQ_PUB模式的socket

	zmq_socket.connect("tcp://localhost:6666");			//通过一个socket创建一个对外连接
	zmq_socket.set(zmq::sockopt::subscribe, zmq::const_buffer(topic, strlen(topic)));//设置过滤条件

	while(1)
	{
		zmq_socket.recv(msg, zmq::recv_flags::none);	//接收消息
		std::cout << msg.to_string() << std::endl;
	}
	return 0;
}

