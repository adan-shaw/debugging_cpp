#include <zmq.hpp>
#include <iostream>



int main(void){
	zmq::message_t zmq_msg;
	zmq::context_t zmq_text(1);
	zmq::socket_t zmq_sock(zmq_text, zmq::socket_type::rep);
	zmq_sock.bind("tcp://127.0.0.1:5555");

	while (true) {
		zmq_msg.rebuild(128);
		zmq_sock.recv(zmq_msg, zmq::recv_flags::none);//阻塞接收
		std::cout << "srv received REQ: " << zmq_msg.to_string() << std::endl;

		memcpy(zmq_msg.data(), "Reply cli", 9);
		zmq_sock.send(zmq_msg, zmq::send_flags::none);//阻塞发送
	}

	return 0;
}
