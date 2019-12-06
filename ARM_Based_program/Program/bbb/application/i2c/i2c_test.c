#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR_1 0x38
#define I2C_ADDR_2 0x3b

int main(void)
{
	int value=0;
	int salue=1;
	int fd;

	fd = open("/dev/i2c-2",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	ioctl(fd,I2C_SLAVE,I2C_ADDR_1);
	write(fd,&value,sizeof(value));

	return 0;
}
