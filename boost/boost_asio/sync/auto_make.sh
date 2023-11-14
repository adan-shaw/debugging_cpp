g++ -g3 ./boost_asio_tcp_sync_srv.cpp -o tcp_sync_srv -l pthread -l boost_system
g++ -g3 ./boost_asio_tcp_sync_cli.cpp -o tcp_sync_cli -l pthread -l boost_system
g++ -g3 ./boost_asio_udp_sync_srv.cpp -o udp_sync_srv -l pthread -l boost_system
g++ -g3 ./boost_asio_udp_sync_cli.cpp -o udp_sync_cli -l pthread -l boost_system
