//编译:
//		g++ ./boost_asio_udp_sync_cli.cpp -o udp_sync_cli -l pthread -l boost_system



#include<iostream>
#include"boost_asioEx.h"

#define udp_sync_srv_port (7777)
const char* pdata4udp = "hello async/sync udp server!!";

void test_udp_sync_srv(void){
	char buf[0xFF];
	io_context io;
	udp::socket sfd(io);
	udp::endpoint srvPoint(address::from_string("127.0.0.1"), udp_sync_srv_port);

	sfd.open(udp::v4());

	try{
		while(1){
			memcpy(buf, pdata4udp, 0xFF);
			sfd.send_to(buffer(buf), srvPoint, 0);

			memset(buf, 0, 0xFF);
			sfd.receive_from(buffer(buf), srvPoint, 0);
			std::cout << buf << std::endl;
		}
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

	sfd.close();
}

int main(void){
	test_udp_sync_srv();
	return 0;
}
