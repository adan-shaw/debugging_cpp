//编译:
//		g++ ./boost_asioEx.cpp -o x -l pthread -l boost_system

//socket 套接字的大量封装
#include<boost/asio.hpp>

//bind() 函数在boost 中有其他特殊用法, 这里不展开(比较复杂boost::bind() 的用法非常复杂, 尽量弃用这种写法)
#include<boost/bind.hpp>



// 应该可以包含大部分的boost asio 组件
using namespace boost::asio;
using ip::tcp;
using ip::udp;
using ip::icmp;
using ip::address;



//boost:asio 常见的类
/*
	# 通用协议类(需要初始化协议类型, 比较麻烦, 一般不用)
	generic::datagram_protocol
	generic::datagram_protocol::endpoint
	generic::datagram_protocol::socket
	generic::raw_protocol
	generic::raw_protocol::endpoint
	generic::raw_protocol::socket
	generic::seq_packet_protocol
	generic::seq_packet_protocol::endpoint
	generic::seq_packet_protocol::socket
	generic::stream_protocol
	generic::stream_protocol::endpoint
	generic::stream_protocol::iostream
	generic::stream_protocol::socket

	# 指定协议类型(类实体不需要二次初始化, 直接用就行, 与bsd socket API 的详细对比, 请看: ./asio网络编程.html)
	ip::address
	ip::address_v4
	ip::address_v4_iterator
	ip::address_v4_range
	ip::address_v6
	ip::address_v6_iterator
	ip::address_v6_range
	ip::icmp
	ip::icmp::endpoint
	ip::icmp::resolver
	ip::icmp::socket
	ip::network_v4
	ip::network_v6
	ip::resolver_query_base
	ip::tcp
	ip::tcp::acceptor
	ip::tcp::endpoint
	ip::tcp::iostream
	ip::tcp::resolver
	ip::tcp::socket
	ip::udp
	ip::udp::endpoint
	ip::udp::resolver
	ip::udp::socket
	socket_base
*/
