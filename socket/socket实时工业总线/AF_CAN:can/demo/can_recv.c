//编译:
//    gcc -g3 ./can_recv.c -o r_can

#include <stdio.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>

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

	while (1)
	{
		if (read (socket_fd, &frame, sizeof (struct can_frame)) == -1)
		{
			perror ("Read error");
			close (socket_fd);
			return 1;
		}
		printf ("Received: can_id = 0x%X, len = %d\n", frame.can_id, frame.can_dlc);

		for (int i = 0; i < frame.can_dlc; ++i)
		{
			printf ("can data:\n\t0x%X \n", frame.data[i]);
		}
	}

	close (socket_fd);
	return 0;
}
