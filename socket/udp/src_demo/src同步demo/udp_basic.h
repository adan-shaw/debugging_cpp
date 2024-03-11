//
// AF_INET 协议簇only
//



//创建一个udp binder
int udp_binder(const char* srv_ip, unsigned short srv_port);



//udp connect(): 成功返回0, 失败返回-1
int udp_connect(int sfd, const char* targer_ip, unsigned short targer_port);



//由于udp acceptEx() 异步, 太复杂了, 这里不做演示实现了, 需要用到的时候, 请自己重新写一个;



//udp accept(): 返回一个struct udp_accept_ret{} 结构体(里面有各种udp server 所需的数据)
struct udp_accept_ret {
	struct sockaddr_in addr_peer;	//返回值: 对端ip 信息
	unsigned char *pbuf;					//返回值(但需要先初始化buf 缓冲区): 第一次recvfrom() 的数据
	int data_len;									//返回值: 第一次recvfrom() 的数据len
	int pbuf_len;									//输入值: buf 缓冲区len
	int sfd_new;									//返回值: udp server 对接的sfd(相当于accept() 之后拷贝的sfd)
};//用结构体来表达, 更便捷明了(函数参数太多, 不好操作)

int udp_accept(int sfd_li, struct udp_accept_ret *p, unsigned short udp_srv_bind_port);

//用结构体来表达, 更便捷明了(函数参数太多, 不好操作)
int udp_accept_old(int sfd_li, unsigned short udp_srv_bind_port, struct sockaddr_in *addr_peer_ret, unsigned char *pbuf_ret, int *data_len_ret, int pbuf_len);
