#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>



int main (void)
{
	char data[1];
	int count = 0, fd = open ("/dev/my_gpio", O_RDWR);
	if (fd < 0)
	{
		perror ("open()");
		return -1;
	}

	//循环改变BEEP & LED 的io 状态: ON / OFF
	while (1)
	{
		read (fd, data, sizeof (data));
		if (data[0] == '0')
		{
			printf("BEEP = OFF, LED = OFF, turn it all ON, test count = %d\n", count++);
			write (fd, "BEEP_ON", sizeof ("BEEP_ON"));
			write (fd, "LED_ON", sizeof ("LED_ON"));
		}
		else
		{
			printf("BEEP = ON, LED = ON, turn it all OFF, test count = %d\n", count++);
			write (fd, "BEEP_OFF", sizeof ("BEEP_OFF"));
			write (fd, "LED_OFF", sizeof ("LED_OFF"));
		}
		sleep(1);
	}

	close (fd);
	return 0;
}
