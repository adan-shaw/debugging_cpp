//编译:
//		g++ ./boost_asio_tcp_sync_cli.cpp -o tcp_sync_cli -l pthread -l boost_system



#include<iostream>
#include"boost_asioEx.h"

#define tcp_sync_srv_port (8884)
const char* pdata4tcp = "hello async/sync tcp server!!";

void test_tcp_sync_srv(void){
	char buf[0xFF];
	io_context io;
	tcp::socket sfd(io);

	sfd.connect(tcp::endpoint(address::from_string("127.0.0.1"), tcp_sync_srv_port));

	try{
		while(1){
			memcpy(buf, pdata4tcp, 0xFF);
			sfd.send(buffer(buf), 0);

			memset(buf, 0, 0xFF);
			sfd.receive(buffer(buf), 0);
			std::cout << buf << std::endl;
		}
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

	sfd.close();
}

int main(void){
	test_tcp_sync_srv();
	return 0;
}
