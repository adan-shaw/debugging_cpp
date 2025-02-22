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
const unsigned int my_canid = 0x123;

int main (void)
{
	struct sockaddr_can addr;						//can 地址info 载体
	struct canfd_frame frame;						//canfd 数据帧载体
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

	//开启canfd 模式
	i = 1;
	setsockopt(sfd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &i, sizeof(int));

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

	//canfd 帧的数据读取循环
	while (1)
	{
		if (read (sfd, &frame, sizeof (struct can_frame)) == -1)
		{
			perror ("read()");
			close (sfd);
			return -1;
		}

		//打印can 数据的id + can_dlc 数据帧的数量(单帧0-8 字节, 8*8=64bit=max)
		if (frame.can_id & CAN_EFF_MASK){
			//收到了can 扩展帧
			printf ("Receive Extended canfd frames CAN_ID: <0x%08x>, len = %d, data: ", frame.can_id & CAN_EFF_MASK, frame.len);
			//打印can 扩展帧数据(最大长度<=8)
			assert(frame.len <= CANFD_MAX_DLC);
			for (i = 0; i < frame.len; ++i)
			{
				printf (" 0x%X ", frame.data[i]);
			}
			printf ("\n");
			break;//只为了演示, 收一条数据就跳出while() 循环[非常不好的编程习惯, 应当拒绝这样做!! 这个break; 是个深坑, 坑死我了]
		}
	}

	close (sfd);
	return 0;
}
