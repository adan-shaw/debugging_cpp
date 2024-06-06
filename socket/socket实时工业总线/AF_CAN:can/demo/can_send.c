//编译:
//    gcc -g3 ./can_send.c -o s_can

#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

int main (void)
{
	const char *can_interface = "vcan0";	// 根据您的系统和硬件配置选择适当的接口名称

	int socket_fd = socket (PF_CAN, SOCK_RAW, CAN_RAW);
	if (socket_fd == -1)
	{
		perror ("Socket creation error");
		return 1;
	}

	struct ifreq ifr;
	strncpy (ifr.ifr_name, can_interface, IFNAMSIZ - 1);
	if (ioctl (socket_fd, SIOCGIFINDEX, &ifr) == -1)
	{
		perror ("ioctl error");
		close (socket_fd);
		return 1;
	}

	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind (socket_fd, (struct sockaddr *) &addr, sizeof (addr)) == -1)
	{
		perror ("Binding error");
		close (socket_fd);
		return 1;
	}

	struct can_frame frame;
	frame.can_id = 0x123;					// 设置消息的标识符
	frame.can_dlc = 4;						// 设置消息数据长度
	frame.data[0] = 0x01;
	frame.data[1] = 0x02;
	frame.data[2] = 0x03;
	frame.data[3] = 0x04;

	if (write (socket_fd, &frame, sizeof (struct can_frame)) == -1)
	{
		perror ("Write error");
		close (socket_fd);
		return 1;
	}

	close (socket_fd);
	return 0;
}
