//编译:
//		gcc -g3 ./can_send.c -o s_can



#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>



const char *if_can_name = "vcan0";		//can 网络空间接口名
const unsigned int my_canid = 0x123;

int main (void)
{
	struct sockaddr_can addr;						//can 地址info 载体
	struct canfd_frame frame;						//canfd 数据帧载体
	struct can_filter filter;						//can 过滤器
	struct ifreq ifr;										//can 设备名专用载体

	int sfd = socket (PF_CAN, SOCK_RAW, CAN_RAW);
	if (sfd == -1)
	{
		perror ("socket()");
		return -1;
	}

	//设置can 过滤器
	filter.can_id = my_canid;
	filter.can_mask = CAN_SFF_MASK;
	setsockopt (sfd, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof (struct can_filter));

	strncpy (ifr.ifr_name, if_can_name, IFNAMSIZ - 1);//设置can 接口名: vcan0
	if (ioctl (sfd, SIOCGIFINDEX, &ifr) == -1)				//绑定socket 到vcan0 设备上(send/recv 双端都需要通过ioctl() 把sfd 绑定到vcan0 设备上)
	{
		perror ("ioctl()");
		close (sfd);
		return -1;
	}

	//填充can socket info
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind (sfd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
	{																									//绑定socket 到vcan0 设备上(send/recv 双端都需要通过bind() 把sfd 绑定到vcan0 设备上)
		perror ("bind()");
		close (sfd);
		return -1;
	}

	//填充can 标准帧的发送数据
	frame.can_id = my_canid ;	//设置can_id 消息的标识符
	frame.len = 4;													//设置len 消息数据长度(每节data 固定为8bit, max=15节)
	frame.flags = 0 & CANFD_BRS;						//flags位: 0 & CANFD_BRS
	frame.__res0 = 0;												//填充/保留位: 填0
	frame.__res1 = 0;												//填充/保留位: 填0
	frame.data[0] = 0x01;										//帧数据(0-8 字节)
	frame.data[1] = 0x02;
	frame.data[2] = 0x03;
	frame.data[3] = 0x04;

	//执行单次can 数据发送
	if (write (sfd, &frame, sizeof (struct can_frame)) == -1)
	{
		perror ("write()");
		close (sfd);
		return -1;
	}

	close (sfd);
	return 0;
}
