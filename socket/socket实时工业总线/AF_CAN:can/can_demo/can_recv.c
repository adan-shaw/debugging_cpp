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
#include <assert.h>



const char *if_can_name = "vcan0";		//can 网络空间接口名
const unsigned int my_canid = 0x122;

int main (void)
{
	struct sockaddr_can addr;						//can 地址info 载体
	struct can_frame frame;							//can 数据帧载体
	struct can_filter filter;						//can 过滤器
	struct ifreq ifr;										//can 设备名专用载体

	int i, sfd = socket (PF_CAN, SOCK_RAW, CAN_RAW);
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

	//can 标准帧的数据读取循环
	while (1)
	{
		if (read (sfd, &frame, sizeof (struct can_frame)) == -1)
		{
			perror ("read()");
			close (sfd);
			return -1;
		}

		//打印can 数据的id + can_dlc 数据帧的数量(单帧0-8 字节, 8*8=64bit=max)
		if (frame.can_id & CAN_EFF_FLAG){
			//收到了can 扩展帧
			printf ("Receive Extended frames CAN_ID: <0x%08x>, can_dlc = %d, data: ", frame.can_id & CAN_EFF_MASK, frame.can_dlc);
			//打印can 扩展帧数据(最大长度<=8)
			assert(frame.can_dlc <= CAN_MAX_DLC);
			for (i = 0; i < frame.can_dlc; ++i)
			{
				printf (" 0x%X ", frame.data[i]);
			}
			printf ("\n");
			break;//只为了演示, 收一条数据就跳出while() 循环
		}
		else{
			//收到了can 标准帧
			printf ("Receive Standard frames CAN_ID: <0x%03x>, can_dlc = %d\n", frame.can_id & CAN_SFF_MASK, frame.can_dlc);
			//打印can 标准帧数据(最大长度<=8)
			assert(frame.can_dlc <= CAN_MAX_DLC);
			for (i = 0; i < frame.can_dlc; ++i)
			{
				printf (" 0x%X ", frame.data[i]);
			}
			printf ("\n");
			break;//只为了演示, 收一条数据就跳出while() 循环
		}
	}

	close (sfd);
	return 0;
}
