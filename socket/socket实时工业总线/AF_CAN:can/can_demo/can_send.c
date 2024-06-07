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

const char *if_can_name = "vcan0";		// can 网络空间接口名

int main (void)
{
	struct sockaddr_can addr;						//can 地址info 载体
	struct can_frame frame;							//can 数据帧载体
	struct ifreq ifr;										//can 设备名专用载体

	int sfd = socket (PF_CAN, SOCK_RAW, CAN_RAW);
	if (sfd == -1)
	{
		perror ("socket()");
		return -1;
	}

	strncpy (ifr.ifr_name, if_can_name, IFNAMSIZ - 1);// 设置can 接口名: vcan0
	if (ioctl (sfd, SIOCGIFINDEX, &ifr) == -1)				// 绑定socket 到vcan0 设备上(send/recv 双端都需要通过ioctl() 把sfd 绑定到vcan0 设备上)
	{
		perror ("ioctl()");
		close (sfd);
		return -1;
	}

	// 填充can socket info
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind (sfd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
	{																									// 绑定socket 到vcan0 设备上(send/recv 双端都需要通过bind() 把sfd 绑定到vcan0 设备上)
		perror ("bind()");
		close (sfd);
		return -1;
	}

	//填充can 发送数据
	frame.can_id = 0x123;					// 设置消息的标识符
	frame.can_dlc = 4;						// 设置消息数据长度
	frame.data[0] = 0x01;					// 单帧数据64bit (注意数据size 粒度)
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
