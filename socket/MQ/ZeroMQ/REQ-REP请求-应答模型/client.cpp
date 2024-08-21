#include <zmq.hpp>
#include <iostream>



int main(void){
	std::string str_data = "Hello srv";						//最大数据长度: 以REQ zmq_msg 为准, REP zmq_msg 不准超出REQ zmq_msg的数据长度
	zmq::message_t zmq_msg(str_data.size());
	memcpy(zmq_msg.data(), str_data.c_str(), str_data.size());

	zmq::context_t zmq_text(1);
	zmq::socket_t zmq_sock(zmq_text, zmq::socket_type::req);
	zmq_sock.connect("tcp://127.0.0.1:5555");

	zmq_sock.send(zmq_msg, zmq::send_flags::none);//阻塞发送

	zmq_msg.rebuild(10);													//一般情况下: 发送data 的buf 多大, 接收buf 就多大, zmq 是对齐的, 不可修改!!
	zmq_sock.recv(zmq_msg, zmq::recv_flags::none);//阻塞接收
	std::cout << "cli received REP: " << zmq_msg.to_string() << std::endl;

	return 0;
}
