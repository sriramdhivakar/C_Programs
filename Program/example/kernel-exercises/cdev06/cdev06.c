#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MODULE_NAME "cdev06.c"
#define DEBUG(msg, args...) do {                                         \
                printk(KERN_DEBUG MODULE_NAME " %s()@%d " msg, __func__, __LINE__, ##args); \
                } while (0)

#define DEVICE_NAME "gdev"
#define FIRST 0

static int ndevs = 0;

struct gdev_s {
        dev_t dev;
        struct cdev *cdev;
        struct class *class;
        struct device **device;
};

struct gdev_buffer_s {
        char *buffer;
        int len;
};

static struct gdev_buffer_s gbuffer[] = {
        { "Hello World\n", 12 },
        { "How are you doing\n", 18 },
        { "You should play *skyrim* a little\n", 34 },
        { "I could be leveling right now\n", 30 },
        { "Don't play tibia, is too hard\n", 31 },
        { "Fuz ho dah!\n", 12 },
};

/* static char *hello = "Hello World\n"; */
/* static int hello_len = 12; */

static struct gdev_s gdev;

static int dev_open(struct inode *ip, struct file *fp)
{
        return 0;
}

static int dev_release(struct inode *ip, struct file *fp)
{
        return 0;
}


static ssize_t dev_read(struct file *fp,
                        char *buf,
                        size_t len,
                        loff_t *off)
{
        int tocopy;
        int copied;
        int i = MINOR(fp->f_inode->i_rdev) - FIRST;
        char *hello = gbuffer[i].buffer;
        int hello_len = gbuffer[i].len;
        
        DEBUG("dev_read() called with device %d:%d", 
              MAJOR(fp->f_inode->i_rdev), MINOR(fp->f_inode->i_rdev));
              
        if (*off > len)
                return 0;

        if (len + *off > hello_len)
                len = hello_len - *off;

        tocopy = copy_to_user(buf + *off,
                              hello,
                              len);

        copied = len - tocopy;
        *off += copied;
        return copied;
}
        

struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = dev_read,
        .open = dev_open,
        .release = dev_release,
};

int init_module(void)
{
        int error;
        int rval;
        int i = 0;
       
        DEBUG("module initialize");

        /* determine the number of devices to create */
        ndevs = sizeof(gbuffer) / sizeof (gbuffer[0]);

        error = alloc_chrdev_region(&gdev.dev, FIRST, ndevs, DEVICE_NAME);
        if (error) {
                DEBUG("alloc_chrdev_region() fails");
                return -1;
        }

        gdev.class = class_create(THIS_MODULE, "gko");
        if (IS_ERR(gdev.class)) {
                DEBUG("class_create() fails");
                goto class_error;
        }

        gdev.cdev = cdev_alloc();
        if (!gdev.cdev) {
                DEBUG("alloc_cdev() fails at %i", i);
                goto cdev_error;
        }

        cdev_init(gdev.cdev, &fops);

        rval = cdev_add(gdev.cdev, gdev.dev, ndevs);
        if (rval < 0) {
                DEBUG("cdev_add() fails");
                goto cdev_error;
        }

        gdev.device = kmalloc(sizeof (struct device *) * ndevs, GFP_KERNEL);
        if (!gdev.device) {
                DEBUG("kamalloc() fails");
                goto cdev_error;
        }

        DEBUG("ndevs = %d", ndevs);
        for (i = 0; i < ndevs; i++) {
                int minor = MINOR(gdev.dev) + i;
                int major = MAJOR(gdev.dev);

                gdev.device[i] = device_create(gdev.class, NULL, 
                                            MKDEV(major, minor), 
                                            NULL, "gdev%d", minor);

                if (IS_ERR(gdev.device[i])) {
                        DEBUG("device_create() fails");
                        goto device_error;
                } else 
                        DEBUG("device gdev%d created", minor);
                                
        }

        return 0;

device_error:
        do {
                int j;
                for (j = 0; j < i; j++)
                        device_destroy(gdev.class, MKDEV(MAJOR(gdev.dev),
                                                         MINOR(gdev.dev) + j));
        } while (0);
        kfree(gdev.device);

cdev_error:
        cdev_del(gdev.cdev);
        unregister_chrdev_region(gdev.dev, ndevs);

class_error:
        class_unregister(gdev.class);
        class_destroy(gdev.class);

        return -1;
}

void cleanup_module(void)
{
        int i;
        for (i = 0; i < ndevs; i++)
                device_destroy(gdev.class, MKDEV(MAJOR(gdev.dev),
                                                 MINOR(gdev.dev) + i));
        class_unregister(gdev.class);
        class_destroy(gdev.class);
        cdev_del(gdev.cdev);
        unregister_chrdev_region(gdev.dev, ndevs);
}


MODULE_LICENSE("GPL");
