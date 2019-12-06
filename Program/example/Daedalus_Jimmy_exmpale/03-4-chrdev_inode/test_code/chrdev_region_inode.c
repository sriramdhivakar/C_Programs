#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define DEVFILE "/dev/chrdev_inode"



int main(void) {
    int fd;

    fd = open(DEVFILE, O_RDWR);

    if (fd == -1) {
        perror("open");
    }

    sleep(5);

    if (close(fd) != 0) {
        perror("close");
    }


    return 0;
}

