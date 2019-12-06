#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>

#define AT24C08B_MAJOR 250

#define MAX_BUF_RW  256
static int at24c08b_major = AT24C08B_MAJOR;

static struct class* i2c_class;
static dev_t  devid;
struct at24c08b_dev {
    struct i2c_client* client;
    char name[30];
    struct cdev* cdev;
};

struct at24c08b_dev* at24c08b_devp;

static int at24c08b_open(struct inode* inode, struct file* file) {
    file->private_data = at24c08b_devp;
    return 0;
}


static ssize_t at24c08b_read(struct file* file, char* buf, size_t count, loff_t* ppos) {
    int i = 0;
    int transferred = 0;
    int offset;
    char bufRead[MAX_BUF_RW];
    struct at24c08b_dev* dev = (struct at24c08b_dev*)file->private_data;

    offset =  *ppos;

    if (i2c_check_functionality(dev->client->adapter, I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
        while (transferred < count) {
            bufRead[i++]  = i2c_smbus_read_byte_data(dev->client, offset + i);
            transferred += 1;
        }

        copy_to_user(buf, (void*)bufRead, transferred);
    }

    return transferred;
}



static ssize_t at24c08b_write(struct file* file, char* buf, size_t count, loff_t* ppos) {

    int transferred = 0;

    char bufWrite[MAX_BUF_RW];
    int offset;
    struct at24c08b_dev* dev = (struct at24c08b_dev*)file->private_data;
    offset = *ppos;

    if (i2c_check_functionality(dev->client->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {

        copy_from_user(bufWrite, buf, count);

        while (transferred < count) {
            i2c_smbus_write_byte_data(dev->client, offset + transferred, bufWrite[transferred]);
            transferred += 1;
        }
    }

    return transferred;
}

static int at24c08b_release(struct inode* inodep, struct file* file) {
    file->private_data = NULL;
    return 0;
}

static const struct file_operations at24c08b_fops = {
    .owner    = THIS_MODULE,
    .open    = at24c08b_open,
    .read    = at24c08b_read,
    .write    = at24c08b_write,
    .release = at24c08b_release,
};



static int __devinit at24c08b_probe(struct i2c_client* client, const struct i2c_device_id* id) {
    int ret;

    at24c08b_devp = kmalloc(sizeof(struct at24c08b_dev), GFP_KERNEL);

    if (!at24c08b_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    memset(at24c08b_devp, 0, sizeof(struct at24c08b_dev));


    at24c08b_devp->client = client;

    devid  = MKDEV(at24c08b_major, 0);

    alloc_chrdev_region(&devid, 0, 1, "at24c08b");

    at24c08b_devp->cdev = cdev_alloc();
    cdev_init(at24c08b_devp->cdev, &at24c08b_fops);
    at24c08b_devp->cdev->owner = THIS_MODULE;
    ret = cdev_add(at24c08b_devp->cdev, devid, 1);

    if (ret) {
        printk(KERN_INFO "I was assigned major number %d.\n", at24c08b_major);
        return -1;
    }

    device_create(i2c_class, NULL, devid, NULL, "at24c08b", MINOR(devid));

    return 0;

fail_malloc:
    unregister_chrdev_region(devid, 1);
    device_destroy(i2c_class,  devid);
    cdev_del(at24c08b_devp->cdev);
    device_destroy(i2c_class, MKDEV(at24c08b_major, 0));

    return ret;
}

static int __devexit at24c08b_remove(struct i2c_client* client) {

    printk("--------------->remove\n");
    unregister_chrdev_region(devid, 1);
    cdev_del(at24c08b_devp->cdev);
    kfree(at24c08b_devp);

    device_destroy(i2c_class, MKDEV(at24c08b_major, 0));


    return 0;
}


static const struct i2c_device_id at24c08b_id[] = {
    { "at24c08b", 0x50},
    { }
};
MODULE_DEVICE_TABLE(i2c, at24c08b_id);


static struct i2c_driver at24c08b_driver = {
    .driver =
    {
        .name = "at24c08b",
        .owner = THIS_MODULE,
    },
    .probe = at24c08b_probe,
    .remove = __devexit_p(at24c08b_remove),
    .id_table = at24c08b_id,
};

static int __init at24c08b_init(void) {
    printk(KERN_NOTICE"at24c08b is insmod\n");
    i2c_class = class_create(THIS_MODULE, "i2c");

    if (IS_ERR(i2c_class)) {
        printk(KERN_INFO "create class error\n");
        return -1;
    }

    return i2c_add_driver(&at24c08b_driver);
}

void at24c08b_exit(void) {
    printk(KERN_NOTICE"at24c08b is rmmod\n");
    i2c_del_driver(&at24c08b_driver);
    class_destroy(i2c_class);

}



MODULE_DESCRIPTION("at24c08b eeprom driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("lxlong");

module_init(at24c08b_init);
module_exit(at24c08b_exit);
