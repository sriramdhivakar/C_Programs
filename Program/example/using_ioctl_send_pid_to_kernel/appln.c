#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "header.h"

int main()
{
    int fd;
    pid_t pid;
    char* msg = "Hello\n";

    fd = open(DEVICE_PATH, O_RDWR);
    printf("%ld\n", IOCTL_CMD);
    ioctl(fd, IOCTL_CMD, msg);
    pid = getpid();
    ioctl(fd, IOCTL_SEND_PID, pid);

    printf("PID=%u\n", pid);
    printf("Ioctl executed\n");
    close(fd);
    return 0;
}
