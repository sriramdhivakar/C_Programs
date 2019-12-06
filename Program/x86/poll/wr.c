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
	char buf[20] = "hai Angusamy";

	fd = open("/dev/poll",O_RDWR);
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

	sk = lseek(fd,0,SEEK_CUR);
	printf("Seek_cur is %d\n",sk);

	sk = lseek(fd,0,SEEK_END);
	printf("Seek_end is %d\n",sk);

	sk = lseek(fd,5,SEEK_SET);
	printf("Seek_set is %d\n",sk);

	close(fd);
	return 0;
}
