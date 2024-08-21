#include <cstdio>
#include <unistd.h>
//必须手动安装cppzmq 库到/usr/local
#include <zmq.hpp>
#include <zmq_addon.hpp>



int main(void){
	const char str_data[] = "hello client!!";
	zmq::message_t zmq_msg(32);										//初始化32 字节缓冲区的zmq_msg
	zmq::context_t zmq_text(1);										//获取zmq上下文
	zmq::socket_t zmq_sock(zmq_text, ZMQ_PUB);		//创建PUB 发布模型的zmq_sock

	zmq_sock.bind("tcp://127.0.0.1:6666");				//绑定发布端的ip+port(tcp 协议)

	//拷贝str_data 到zmq_msg 里面[注意: 1.缓冲区不够用导致的溢出问题 2.不能用strlen(), 而是用sizeof(), 否则最后一个字符'\0'没有被拷贝]
	//memcpy(reinterpret_cast<void*>(zmq_msg.data()), str_data, sizeof(str_data));
	zmq_msg.rebuild(str_data, sizeof(str_data));	//重置zmq_msg 的缓冲区大小, 并自动填充zmq_msg 的内容为str_data(更便捷的zmq_msg 拷贝方式)

	//发布消息loop
	while(1)
	{
		zmq_sock.send(zmq_msg, zmq::send_flags::dontwait);//非阻塞, 循环发送消息
		printf("srv sended: %s\n", reinterpret_cast<char*>(zmq_msg.data()));//debugging only(强转打印字符串)
		sleep(1);																					//发送消息间隔(可随意定义)
	}
	return 0;
}

