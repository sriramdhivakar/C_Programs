#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define I2C_ADDR0 0x3b
#define I2C_ADDR1 0x38

int main(int argc,char *argv[])
{
	int fd,io,wr;
	unsigned int a,b;
	char ina,inv;

	if(argc < 3)
	{
		printf("Exceed the limit\n");
		_exit(1);
	}

	fd = open("/dev/i2c-2",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}
	
	a = argv[1];
	printf("%d\n",a);
	ioctl(fd,I2C_SLAVE,a);
	
	b = argv[2];
	printf("%d\n",b);
	wr = write(fd,&b,sizeof(b));
	if(wr < 0)
	{
		perror("write");
		_exit(1);
	}
	close(fd);
	_exit(0);
}
