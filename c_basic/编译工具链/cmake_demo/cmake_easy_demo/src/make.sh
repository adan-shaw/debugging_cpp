# ps:
#   -l boost_system 编译选项非常奇怪!! 一定要放在 -l pthread -l boost_system -o 前面, 否则死活找不到!!

g++ -g3 ./boost_asio_tcp_sync_srv.cpp -l pthread -l boost_system -o tcp_sync_srv
g++ -g3 ./boost_asio_tcp_sync_cli.cpp -l pthread -l boost_system -o tcp_sync_cli
g++ -g3 ./boost_asio_udp_sync_srv.cpp -l pthread -l boost_system -o udp_sync_srv
g++ -g3 ./boost_asio_udp_sync_cli.cpp -l pthread -l boost_system -o udp_sync_cli
