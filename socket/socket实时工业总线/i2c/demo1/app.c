#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main (void)
{
	int fd;
	char data[1];

	fd = open ("/dev/my_gpio", O_RDWR);
	if (fd < 0)
	{
		perror ("open error");
		return -1;
	}

	while (1)
	{
		read (fd, data, sizeof (data));
		if (data[0] == '0')
		{
			write (fd, "BEEP_ON", sizeof ("BEEP_ON"));
			write (fd, "LED_ON", sizeof ("LED_ON"));
		}
		else
		{
			write (fd, "BEEP_OFF", sizeof ("BEEP_OFF"));
			write (fd, "LED_OFF", sizeof ("LED_OFF"));
		}
	}

	close (fd);
	return 0;
}
