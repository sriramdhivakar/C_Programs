#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "ioctl_commands.h"

int main ()
{
  int fd, i;
  char my_buf[4000];

  unsigned int size;
  char c;


  printf ("[%d] - Opening device skg_cdrv\n", getpid ());

  fd = open ("/dev/sriram", O_RDWR);
  if (fd < 0)
    {
      printf ("\n\nDevice could not be opened\n\n");
      return 1;
    }
  printf ("Device opened with ID [%d]\n", fd);


  ioctl (fd, SKG_GET_SIZE, &size);

  printf ("Present size of the buffer is %d\n", size);
  printf ("Enter new size : ");
  scanf ("%d", &size);
  //size = 6000;
  printf ("Setting size of buffer to %d\n", size);

  if (ioctl (fd, SKG_SET_SIZE, &size) < 0)
    printf ("ioctl failed\n");

  ioctl (fd, SKG_GET_SIZE, &size);
  printf ("New size of the buffer is %d\n", size);


  /* lets fill the buffer with character '=' */
  c = 'p';
  ioctl (fd, SKG_FILL_CHAR, &c);

  /* test if it worked */
  read (fd, my_buf, 80);
  printf ("New contents\n%s\n", my_buf);

  close (fd);

  exit (0);
}
