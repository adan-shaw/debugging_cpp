//编译:
//		gcc -g3 ./canfd_send.c -o s_canfd



#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

//canfd_frame 帧详细定义
/*
	struct canfd_frame {
		canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags
		__u8    len;     // frame payload length in byte
		__u8    flags;   // additional flags for CAN FD
		__u8    __res0;  // reserved / padding
		__u8    __res1;  // reserved / padding
		__u8    data[CANFD_MAX_DLEN] __attribute__((aligned(8)));
	};
*/

#define CAN_RAW_FD CAN_RAW						// 屏蔽平台差异(debian linux 中并没有CAN_RAW_FD 套接字, 详情请看: <linux/can.h>)
const char *if_can_name = "vcanfd0";	// canfd 网络空间接口名

int main (void)
{
	struct sockaddr_can addr;						//canfd 地址info 载体
	struct canfd_frame frame;						//canfd 数据帧载体
	struct ifreq ifr;										//canfd 设备名专用载体

	int sfd = socket (PF_CAN, SOCK_RAW, CAN_RAW_FD);
	if (sfd == -1)
	{
		perror ("socket()");
		return -1;
	}

	strncpy (ifr.ifr_name, if_can_name, IFNAMSIZ - 1);// 设置canfd 接口名: vcan0
	if (ioctl (sfd, SIOCGIFINDEX, &ifr) == -1)				// 绑定socket 到vcan0 设备上(send/recv 双端都需要通过ioctl() 把sfd 绑定到vcan0 设备上)
	{
		perror ("ioctl()");
		close (sfd);
		return -1;
	}

	// 填充canfd socket info
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind (sfd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
	{																									// 绑定socket 到vcan0 设备上(send/recv 双端都需要通过bind() 把sfd 绑定到vcan0 设备上)
		perror ("bind()");
		close (sfd);
		return -1;
	}

	//填充canfd 发送数据
	frame.can_id = 0x123;					// 设置消息的标识符
	frame.len = 4;								// 设置消息数据长度
	frame.data[0] = 0x01;					// 单帧数据64bit (注意数据size 粒度)
	frame.data[1] = 0x02;
	frame.data[2] = 0x03;
	frame.data[3] = 0x04;

	//执行单次canfd 数据发送
	if (write (sfd, &frame, sizeof (struct can_frame)) == -1)
	{
		perror ("write()");
		close (sfd);
		return -1;
	}

	close (sfd);
	return 0;
}
