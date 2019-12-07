#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int rd,fd;
	char buf[20];

	fd = open("/dev/sriram",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}

	rd = read(fd,buf,19);
	if(rd < 0)
	{
		perror("read");
		_exit(1);
	}

	printf("Read contain %d amount of size\n",rd);
	printf("Read contain data of %s\n",buf);

	close(fd);
	_exit(0);
}
