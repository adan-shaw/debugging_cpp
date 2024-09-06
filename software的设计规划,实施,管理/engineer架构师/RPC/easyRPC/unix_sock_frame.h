//'客户端&服务端'约定的unix socket文件名
#define UNIX_TCP_DOMAIN "/tmp/UNIX_TCP.domain"

//由于unix socket几乎不用等待,这个值可以很小,8/16 都可以
#define UNIX_TCP_LISTEN_MAX (8)

//usock_tcp 消息结构体(不需要校验数,不会丢包的; 不需要指明帧的长度,尽量设计成最简单的单帧数据包,永恒单帧)
struct usock_tcp_frame{
	char API[64];			//函数名最长64bit
	char data[1024];	//通过序列化, 把参数做间隔, 或者是RPC 程序的一大设计难点(因为参数个数不确定, 不定长的参数数据, 很麻烦) [这里演示, 参数只有一串字符串]
};

typedef struct usock_tcp_frame utcp_frame_t;



//'客户端&服务端'约定的unix socket文件名
#define UNIX_UDP_DOMAIN "/tmp/UNIX_UDP.domain"

//usock_tcp 消息结构体
struct usock_udp_frame{
	char API[64];			//函数名最长64bit
	char data[1024];	//通过序列化, 把参数做间隔, 或者是RPC 程序的一大设计难点(因为参数个数不确定, 不定长的参数数据, 很麻烦) [这里演示, 参数只有一串字符串]
};

typedef struct usock_udp_frame uudp_frame_t;



