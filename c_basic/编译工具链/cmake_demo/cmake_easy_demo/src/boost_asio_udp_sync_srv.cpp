//编译:
//		g++ ./boost_asio_udp_sync_srv.cpp -o udp_sync_srv -l pthread -l boost_system



#include<iostream>
#include"boost_asioEx.h"

#define udp_sync_srv_port (7777)

int main(void){
	char buf[0xFF];
	io_context io;
	udp::endpoint cliPoint;
	udp::socket sfd(io, udp::endpoint(udp::v4(), udp_sync_srv_port));
	std::cout << "[0.0.0.0:" << udp_sync_srv_port << "]server start:" << std::endl;

	try{
		while(1){
			sfd.receive_from(buffer(buf), cliPoint, 0);
			sfd.send_to(buffer(buf), cliPoint, 0);//数据原路返回, 不做修改
		}
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

	sfd.close();
	return 0;
}
