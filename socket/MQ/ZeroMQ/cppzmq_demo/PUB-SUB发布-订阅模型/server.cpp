#include <iostream>
#include <unistd.h>
//必须手动安装cppzmq 库到/usr/local
#include <zmq.hpp>
#include <zmq_addon.hpp>



int main(void){
	std::string str2cli = "hello client!";
	zmq::message_t msg;
	zmq::context_t zmq_context(1);									//获取zmq上下文
	zmq::socket_t zmq_socket(zmq_context, ZMQ_PUB);	//创建ZMQ_PUB模式的socket

	zmq_socket.bind("tcp://*:6666");								//绑定端口号
	msg.rebuild(str2cli.data(), str2cli.size());

	while(1)
	{
		zmq_socket.send(msg, zmq::send_flags::dontwait);
		sleep(1);
	}
	return 0;
}

