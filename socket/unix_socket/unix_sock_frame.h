//unix socket 基础:
/*
	* unix socket 传递消息, 不支持void*内嵌;
		不同进程之间,没办法共享堆内存,别浪费心血想办法共享void*指针了;

	* 本机多进程通信, 可以忽略大小端字节序的问题, 因此可以直接传递整形or 浮点数据;
		所以, 数据结构体的定义, 可以随意一点, 
		不过指针类型or 变量实体类型传递, 禁止!!
		因为一般使用unix socket 的都是多进程之间通信, 
		不同进程之间, 不能传递指针, 
		类实体, 结构体实体, 定义的情况可能都不一样(可以通过copy 来拷贝, 但应用场景不多)

	* unix socket 约定的文件名, 多进程访问时, 并不需要加锁/加信号灯, 但需要先注册, 再访问;
*/

//unix socket using with udp protocol:
/*
	一般不需要考虑切片, 重排, 基本上单帧数据, 基本上满足大部分unix tcp socket 的应用场景;
	不要把unix socket 弄复杂化!!
	越简单越好, 反正是本机通信, 够用即可!!
	不足一帧, 也要发送足一帧, 填充到位, 不用怜惜, 节省编码复杂度更重要;
*/

//unix socket using with udp protocol:
/*
	unix socket 使用udp, 要保证接收缓冲区不会溢出, 
	也就是单帧数据不能超过'rbuf接收缓冲区'的大小(rbuf默认是8196大小);
	否则你需要修改socket默认缓冲区大小, 
	否则udp读报可能会出现缓冲区溢出丢失.
*/



//'客户端&服务端'约定的unix socket文件名
#define UNIX_TCP_DOMAIN "/tmp/UNIX_TCP.domain"

//由于unix socket几乎不用等待,这个值可以很小,8/16 都可以
#define UNIX_TCP_LISTEN_MAX (8)

//usock_tcp 消息结构体(不需要校验数,不会丢包的; 不需要指明帧的长度,尽量设计成最简单的单帧数据包,永恒单帧)
struct usock_tcp_frame{
	short int type;							//业务类型
	short int bak;
	unsigned int num;
	unsigned int num_arr[16];
	unsigned int num_arr2[8][8];
	char str[64];
};

typedef struct usock_tcp_frame utcp_frame_t;



//'客户端&服务端'约定的unix socket文件名
#define UNIX_UDP_DOMAIN "/tmp/UNIX_UDP.domain"

//usock_tcp 消息结构体
struct usock_udp_frame{
	short int type;							//业务类型
	short int bak;
	unsigned int num;
	unsigned int num_arr[16];
	unsigned int num_arr2[8][8];
	char str[64];
};

typedef struct usock_udp_frame uudp_frame_t;


