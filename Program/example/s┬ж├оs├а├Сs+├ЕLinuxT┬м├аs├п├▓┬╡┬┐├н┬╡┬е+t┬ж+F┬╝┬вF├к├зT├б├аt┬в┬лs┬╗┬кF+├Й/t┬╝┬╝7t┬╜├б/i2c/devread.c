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
    struct i2c_rdwr_ioctl_data dataRead;
    int fd;
    unsigned int slaveAddr, regAddr, value;
    int ret;

    if (argc < 3) {
        printf("input slaveAddr  regAddr \n");
        return 0;
    }

    fd = open("/dev/i2c-0", O_RDWR);

    if (!fd) {
        printf("open /dev/i2c-0 fail\n");
        return 0;
    }

    sscanf(argv[1], "%x", &slaveAddr);
    sscanf(argv[2], "%x", &regAddr);

    dataRead.nmsgs = 2;
    dataRead.msgs = (struct i2c_msg*)malloc(dataRead.nmsgs * sizeof(struct i2c_msg));

    ioctl(fd, I2C_TIMEOUT, 2);//���ó�ʱʱ��
    ioctl(fd, I2C_RETRIES, 1);//�����ط�����


    dataRead.nmsgs = 2;
    dataRead.msgs[0].len = 1;
    dataRead.msgs[0].addr = slaveAddr;
    dataRead.msgs[0].flags = 0;//д����
    dataRead.msgs[0].buf = (unsigned char*)malloc(1);
    dataRead.msgs[0].buf[0] = regAddr;

    dataRead.msgs[1].len = 1;
    dataRead.msgs[1].addr = slaveAddr;
    dataRead.msgs[1].flags = I2C_M_RD;//������
    dataRead.msgs[1].buf = (unsigned char*)malloc(1);
    dataRead.msgs[1].buf[0] = 0;
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&dataRead);

    if (ret < 0) {
        printf("ioctl read error\n");
    }

    printf("data read form addr:%x is 0x%x\n", slaveAddr, dataRead.msgs[1].buf[0]);
    close(fd);
    return 0;
}

