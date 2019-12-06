#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpio_command.h"
int main(void)
{
	int fd;
	printf("[%d]The device opened in the driver",getpid());
	
	fd = open("/dev/gpio",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}
	while(1)
	{
		ioctl(fd,set_gpio_high);
		sleep(1);
		ioctl(fd,set_gpio_low);
		sleep(1);
	}

	close(fd);
	return 0;
}
