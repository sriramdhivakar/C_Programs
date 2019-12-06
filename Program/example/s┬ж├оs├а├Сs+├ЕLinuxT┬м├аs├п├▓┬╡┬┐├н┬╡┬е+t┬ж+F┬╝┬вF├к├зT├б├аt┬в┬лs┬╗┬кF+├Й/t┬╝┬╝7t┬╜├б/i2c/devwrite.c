#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int main(int argc, char** argv) {
    struct i2c_rdwr_ioctl_data dataWrite;
    int fd;
    unsigned int slaveAddr, regAddr, value;
    int ret;

    if (argc < 4) {
        printf("input slaveAddr  regAddr value\n");
        return 0;
    }

    fd = open("/dev/i2c-0", O_RDWR);

    if (!fd) {
        printf("open /dev/i2c-0 fail\n");
        return 0;
    }

    sscanf(argv[1], "%x", &slaveAddr);
    sscanf(argv[2], "%x", &regAddr);
    sscanf(argv[3], "%x", &value);

    dataWrite.nmsgs = 2;
    dataWrite.msgs = (struct i2c_msg*)malloc(dataWrite.nmsgs * sizeof(struct i2c_msg));

    ioctl(fd, I2C_TIMEOUT, 2);//���ó�ʱʱ��
    ioctl(fd, I2C_RETRIES, 1);//�����ط�����


    dataWrite.nmsgs = 1;
    dataWrite.msgs[0].len = 2;
    dataWrite.msgs[0].addr = slaveAddr;
    dataWrite.msgs[0].flags = 0;//д����
    dataWrite.msgs[0].buf = (unsigned char*)malloc(2);
    dataWrite.msgs[0].buf[0] = regAddr;
    dataWrite.msgs[0].buf[1] = value;

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&dataWrite);

    if (ret < 0) {
        printf("ioctl write error\n");
    }

    close(fd);
    return 0;
}