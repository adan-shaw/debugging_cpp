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




//recvmsg()与sendmsg()函数允许程序发送或是接收OOB附属数据;



//0.
/*
	struct in_pktinfo {
		unsigned int ipi_ifindex;			//接口索引
		struct in_addr ipi_spec_dst;	//路由目的地址
		struct in_addr ipi_addr;			//头标识目的地址
	};
	ipi_ifindex 指的是接收包的接口的唯一索引,
	ipi_spec_dst 指的是路由表记录中的目的地址,
	ipi_addr 指的是包头中的目的地址;
	如果给setsockopt传递了IP_PKTINFO,
	那么外发的包会通过在ipi_ifindex中指定的接口发送出去,同时把ipi_spec_dst设置为目的地址;
*/



//1.socket io 数据缓冲区'主要描述体'
/*
	struct msghdr {
		void						*msg_name;				//addr info
		int							msg_namelen;			//addr info len
		struct iovec		*msg_iov;					//msg 链
		__kernel_size_t	msg_iovlen;				//msg 链 len
		void						*msg_control;			//协议buf
		__kernel_size_t	msg_controllen;		//协议buf len
		unsigned				msg_flags;				//io 模式控制
	};
	msg_flags:
		MSG_EOR				当接收到记录结尾时会设置这一位,这一般对于SOCK_SEQPACKET套接口类型十分有用.
		MSG_TRUNC			这个标记位代表数据的结尾被截短,由于接收缓冲区过小不足以接收所有的数据.
		MSG_CTRUNC		这个标记位代表某些控制数据(附属数据)被截短,由于缓冲区过小.
		MSG_OOB				这个标记位代表接收了带外数据.
		MSG_ERRQUEUE	这个标记位代表没有接收到数据,可是返回一个扩展错误.

	struct iovec {
		void  *iov_base;	//数据buf 指针
		size_t iov_len;		//数据buf len
	};
*/



//2.[区分/识别]'主要描述体'的协议头部分:
/*
	struct cmsghdr {
		socklen_t	cmsg_len;			//附属数据的字节计数,这包含结构头的尺寸.这个值是由CMSG_LEN()宏计算的.
		int				cmsg_level;		//这个值代表了原始的协议级别(例如,SOL_SOCKET).
															SCM_RIGHTS 附属数据对象是一个文件描述符
															SCM_CREDENTIALS 附属数据对象是一个包含证书信息的结构
		int				cmsg_type;		//这个值代表了控制信息类型(例如,SCM_RIGHTS).
		u_char		cmsg_data[];	//这个成员并不实际存在.他用来指明实际的额外附属数据所在的位置.
	};
*/



//3.cmsg 宏
/*
	由于附属数据结构的复杂性, Linux系统提供了一系列的C宏来简化我们的工作;
	另外, 这些宏可以在不同的UNIX平台之间进行移植, 并且采取了一些措施来防止将来的改变;
	#include <sys/socket.h>
	struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *msgh);
	struct cmsghdr *CMSG_NXTHDR(struct msghdr *msgh, struct cmsghdr *cmsg);
	size_t CMSG_ALIGN(size_t length);
	size_t CMSG_SPACE(size_t length);
	size_t CMSG_LEN(size_t length);
	void *CMSG_DATA(struct cmsghdr *cmsg);

	CMSG_DATA()宏
		这个宏接受一个指向cmsghdr结构的指针. 
		返回的指针值指向跟随在头部以及填充字节之后的附属数据的第一个字节(如果存在). 
		如果指针mptr指向一个描述文件描述符的可用的附属数据信息头部,这个文件描述符可以用下面的代码来得到:
			struct cmsgptr *mptr;
			int sfd;
			...
			sfd = *(int *)CMSG_DATA(mptr);
	CMSG_FIRSTHDR()宏
		这个宏用于返回一个指向附属数据缓冲区内的第一个附属对象的struct cmsghdr指针. 
		输入值为是指向struct msghdr结构的指针(不要与struct cmsghdr相混淆). 
		这个宏会估计msghdr的成员msg_control与msg_controllen来确定在缓冲区中是否存在附属对象. 
		然后,他会计算返回的指针. 
		如果不存在附属数据对象,则返回的指针值为NULL. 
		否则,这个指针会指向存在的第一个struct cmsghdr. 
		这个宏用在一个for循环的开始处,来开始在附属数据对象中遍历. 
	CMSG_NXTHDR()宏
		这个用于返回下一个附属数据对象的struct cmsghdr指针. 这个宏会接受两个输入参数:
			指向struct msghdr结构的指针
			指向当前struct cmsghdr的指针
		如果没有下一个附属数据对象,这个宏就会返回NULL. 
*/

