#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR 0x43

int main(void)
{
	int value=1;
	int salue=0;
	int fd;

	fd = open("/dev/i2c-2",O_RDWR);

	if(fd < 0)
	{
		perror("open");
		return 1;
	}

	if(ioctl(fd,I2C_SLAVE,I2C_ADDR) < 0)
	{
		printf("ioctl");
		return 1;
	}
	
	while(1)
	{
		write(fd,&value,sizeof(value));
		usleep(100000);
		write(fd,&salue,sizeof(salue));
		usleep(100000);
	}

	return 0;
}
