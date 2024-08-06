#include <zmq.hpp>
#include <iostream>

int main(void){
	zmq::message_t request;
	zmq::message_t reply(5);
	zmq::context_t context(1);
	zmq::socket_t socket(context, zmq::socket_type::rep);
	socket.bind("tcp://*:5555");

	while (true) {
		socket.recv(request, zmq::recv_flags::none);

		std::cout << "Received request: " << request.to_string() << std::endl;

		memcpy(reply.data(), "Reply", 5);
		socket.send(reply, zmq::send_flags::none);
	}

	return 0;
}
