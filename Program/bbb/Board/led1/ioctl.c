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
	int fd,low = 0,high = 1,dir = 1;
	int n,m,o,r;
	int gpio = 60;

	fd = open("/dev/gpio",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}
	ioctl(fd,set_gpio_value,&gpio);		
	ioctl(fd,get_gpio_value,&o);
	printf("GPIO value is %d\n",o);
        ioctl(fd,set_gpio_dir_out,&dir);
	ioctl(fd,get_gpio_dir_out,&r);
	printf("GPIO dir is %d\n",r);
	while(1)
	{
		ioctl(fd,set_gpio_pin,&high);
		ioctl(fd,get_gpio_pin,&n);
		printf("GPIO state is %d\n",n);
		sleep(1);
		ioctl(fd,set_gpio_pin,&low);
		ioctl(fd,get_gpio_pin,&m);
		printf("GPIO state is %d\n",m);
		sleep(1);
	}
	close(fd);
	return 0;
}
