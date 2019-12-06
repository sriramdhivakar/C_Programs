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
    struct i2c_rdwr_ioctl_data e2prom_data;
    unsigned int fd;
    unsigned int slave_address, reg_address, value; //slave_addressΪeeprom�ĵ�ַ��reg_addressΪeeprom�д洢��Ԫ�ĵ�ַ����Χ0x0--0xFFFFFFFF,valueΪ��Ҫд��eeprom��ֵ
    int ret;

    if (argc < 5) {
        printf("Usage:\n%s /dev/i2c/x start_addr reg_addr value\n", argv[0]);
        return 0;
    }

    fd = open(argv[1], O_RDWR);

    if (!fd) {
        printf("Error on opening the device file\n");
        return 0;
    }

    sscanf(argv[2], "%x", &slave_address);
    sscanf(argv[3], "%x", &reg_address);
    sscanf(argv[4], "%x", &value);

    e2prom_data.nmsgs = 2;//��Ϊ��ʱ��Ҫ���Σ�������Ϊ2
    e2prom_data.msgs = (struct i2c_msg*)malloc(e2prom_data.nmsgs * sizeof(struct i2c_msg));

    if (!e2prom_data.msgs) {
        printf("Memory alloc error\n");
        close(fd);
        return 0;
    }

    ioctl(fd, I2C_TIMEOUT, 2);//���ó�ʱʱ��
    ioctl(fd, I2C_RETRIES, 1);//�����ط�����

    /* write data to e2prom*/
    e2prom_data.nmsgs = 1;
    e2prom_data.msgs[0].len = 2;//��Ϣ����Ϊ2����дʱ��eeprom�ĵ�ַ����ģ���Ϊ������addr����len��ָbuf�е�ֵ�ĸ���
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//д����
    e2prom_data.msgs[0].buf = (unsigned char*)malloc(2);
    e2prom_data.msgs[0].buf[0] = reg_address;//��Ϣֵ1 eeprom�д洢��Ԫ�ĵ�ַ������Ҫ����д
    e2prom_data.msgs[0].buf[1] = value;//��Ϣֵ2,����Ҫдʲô

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); //���� ��д��ȥ��

    if (ret < 0) {
        printf("ioctl write error\n");
    }

    printf("you have write %02x into e2prom at %02x address\n", value, reg_address);

    sleep(1);
    /*read data from e2prom*/
    e2prom_data.nmsgs = 2;//��ʱ��Ҫ���ι��̣�Ҫ������I2C��Ϣ
    e2prom_data.msgs[0].len = 1;//��Ϣ����Ϊ1����һ��ֻдҪ����eeprom�д洢��Ԫ�ĵ�ַ
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//д�������ʱ�����
    e2prom_data.msgs[0].buf[0] = reg_address;//��Ϣֵ

    e2prom_data.msgs[1].len = 1;
    e2prom_data.msgs[1].addr = slave_address;
    e2prom_data.msgs[1].flags = I2C_M_RD;//������
    e2prom_data.msgs[1].buf = (unsigned char*)malloc(1);
    e2prom_data.msgs[1].buf[0] = 0;//�����Ҫ���Ļ�����
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); //���ˣ�����

    if (ret < 0) {
        printf("ioctl read error\n");
    }

    printf("read %02x from e2prom address %02x\n", e2prom_data.msgs[1].buf[0], reg_address);

    close(fd);
    return 0;
}