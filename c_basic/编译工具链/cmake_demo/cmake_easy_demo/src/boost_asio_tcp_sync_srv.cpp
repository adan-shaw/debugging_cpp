//编译:
//		g++ ./boost_asio_tcp_sync_srv.cpp -o tcp_sync_srv -l pthread -l boost_system



#include<iostream>
#include"boost_asioEx.h"

#define tcp_sync_srv_port (7777)

int main(void){
	char buf[0xFF];
	io_context io;
	tcp::socket sfd(io);
	tcp::acceptor listener(io, tcp::endpoint(tcp::v4(), tcp_sync_srv_port));
	std::cout << "[0.0.0.0:" << tcp_sync_srv_port << "]server start:" << std::endl;

	listener.accept(sfd);
	std::cout << "client:" << sfd.remote_endpoint().address() << std::endl;

	try{
		while(1){
			sfd.receive(buffer(buf), 0);
			sfd.send(buffer(buf), 0);//数据原路返回, 不做修改
		}
	}
	catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

	sfd.close();
	return 0;
}
