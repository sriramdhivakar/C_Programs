#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include "header.h"

MODULE_LICENSE("Dual BSD/GPL");

static int major_no;

static int device_open(struct inode* inode, struct file* file)
{
    printk(KERN_EMERG "Node Opened\n");
    return 0;
}

int device_ioctl(struct file* filp,
                 unsigned int cmd,
                 unsigned long args)
{
    switch (cmd) {
    case IOCTL_CMD:
        printk(KERN_EMERG "%s \n", (char*)args);
        break;

    case IOCTL_SEND_PID:
        printk(KERN_EMERG "PID=%u \n", (unsigned int)args);
        break;

    default:
        printk(KERN_EMERG "Illegal ioctl command word\n");
        break;
    }

    return 0;
}

static int device_release(struct inode* inode, struct file* file)
{
    printk(KERN_EMERG "Module Released \n");
    return 0;
}

static struct class* my_class;

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

static int hello_init(void)
{
    major_no = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_EMERG "Module Major No : %d\n", major_no);

    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(my_class, NULL, MKDEV(major_no, 0), NULL, DEVICE_NAME);
    printk(KERN_EMERG "Module loaded in kernel\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_EMERG "Device is Released or closed \n");
    device_destroy(my_class, MKDEV(major_no, 0));
    class_unregister(my_class);
    class_destroy(my_class);
    unregister_chrdev(major_no, DEVICE_NAME);
}

module_init(hello_init);
module_exit(hello_exit);
