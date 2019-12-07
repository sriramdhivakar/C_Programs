#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd,wr,sk;
	char buf[20] = "sriram and angusamy";

	fd = open("/dev/wait",O_RDWR);
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

	printf("write contains %d\n",wr);
	printf("write has data of %s\n",buf);

	sk = lseek(fd,0,SEEK_CUR);
	printf("SEEK_CUR is %d\n",sk);

	close(fd);
	_exit(0);

}
