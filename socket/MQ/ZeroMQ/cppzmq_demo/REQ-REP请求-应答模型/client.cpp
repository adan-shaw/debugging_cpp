#include <zmq.hpp>
#include <iostream>

int main(void){
	zmq::message_t reply;
	std::string request_str = "Hello";
	zmq::context_t context(1);
	zmq::socket_t socket(context, zmq::socket_type::req);
	socket.connect("tcp://localhost:5555");

	zmq::message_t request(request_str.size());
	memcpy(request.data(), request_str.c_str(), request_str.size());
	socket.send(request, zmq::send_flags::none);

	socket.recv(reply, zmq::recv_flags::none);

	std::cout << "Received reply: " << reply.to_string() << std::endl;

	return 0;
}
