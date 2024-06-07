//编译:
//		gcc -g3 ./can_recv.c -o r_can



#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>

//can_frame 帧详细定义
/*
	struct can_frame {
		canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags
		__u8    can_dlc; // frame payload length in byte (0 .. CAN_MAX_DLEN)
		__u8    __pad;   // padding
		__u8    __res0;  // reserved / padding
		__u8    __res1;  // reserved / padding
		__u8    data[CAN_MAX_DLEN] __attribute__((aligned(8)));
	};
*/

const char *if_can_name = "vcan0";		// can 网络空间接口名

int main (void)
{
	struct sockaddr_can addr;						//can 地址info 载体
	struct can_frame frame;							//can 数据帧载体
	struct ifreq ifr;										//can 设备名专用载体

	int i, sfd = socket (PF_CAN, SOCK_RAW, CAN_RAW);
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

	//can 数据读取循环
	while (1)
	{
		if (read (sfd, &frame, sizeof (struct can_frame)) == -1)
		{
			perror ("read()");
			close (sfd);
			return -1;
		}
		printf ("Received: can_id = 0x%X, len = %d\n", frame.can_id, frame.can_dlc);//打印can 数据的id + can_dlc 数据帧的数量(单帧0-8 字节, 8*8=64bit=max)

		for (i = 0; i < frame.can_dlc; ++i)//根据can_dlc 数据帧的数量(单帧0-8 字节, 8*8=64bit=max), 遍历can 数据
		{
			printf ("can data:\n\t0x%X \n", frame.data[i]);
		}
	}

	close (sfd);
	return 0;
}
