/****************************************************************************\
* Program name	:  memread.c						     *
* Modification  :							     *
* Notes         : This program will read physical memory ( RAM ) and dump    *
*                 contents using hexdisp() and the /dev/mem device interface *
*                 Can only be run as root                                    *
\****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

static int fd;
static unsigned long PA;
static unsigned long off;
static char * buf;
static int nbr;

int main(int argc, char **argv)
{
	if ( argc != 3 ) {
	   printf("Usage : memread <phys_address(hex)> <numbytes(dec)>\n");
	   return -1;
	}
	fd = open("/dev/mem", O_RDONLY);
	if ( fd < 0 ) {
	   printf("Error opening file /dev/mem\n");
	   return -2;
	}
	PA = strtoul(argv[1], NULL, 16);
	off = lseek(fd, PA, SEEK_SET);
	if ( off == -1 ) {
	   printf("Error in lseek()\n");
	   return -3;
	}
	nbr = atoi(argv[2]);
	buf = malloc(nbr);
	nbr = read(fd, buf, nbr);
	write(1,buf , nbr);
	free(buf);
	close(fd);
	return 0;
}
