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
//using ip::raw;不存在!!
using ip::address;

