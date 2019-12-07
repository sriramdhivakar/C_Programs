#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
	int fd;
	struct pollfd pollfr[1];

	fd = open("/dev/poll",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		_exit(1);
	}

	pollfr[0].fd = fd;
	pollfr[0].events = POLLIN;

	if(poll(pollfr,1,20000) < 0)
	{
		printf("error in opening\n");
		_exit(1);
	}

	if((pollfr[0].revents & POLLIN) == 1)
	{
		printf("fd is readble\n");
	}
	close(fd);
	_exit(0);
}
