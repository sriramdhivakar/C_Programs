#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd,wr;
	char buf[30] = "sriram";
	fd = open("/dev/auto",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}
	wr = write(fd,buf,sizeof(buf));
	if(wr < 0)
	{
		perror("write");
		_exit(1);
	}
	close(fd);
	_exit(0);
}
