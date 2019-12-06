/**
 * @file hello-world.c
 * @author Daniel Hilst Selli <danielhilst@gmail.com>
 * @brief Create a device that spits "Hello World"
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/string.h>

#define MODULE_NAME "hello-world"
#define _DEBUG(fmt, msg...)                                             \
        do {                                                            \
                pr_err("%s:%s@%d: "                                   \
                         fmt, MODULE_NAME, __func__, __LINE__, ##msg);  \
        } while (0);


static char *msg = "Hello world\n";
static int index = 0;
static dev_t dev;
static struct class *class;
static struct cdev *cdev;
static struct device *device;


static int dev_open(struct inode *ip, struct file *fp)
{
        return 0;
}

static int dev_release(struct inode *ip, struct file *fp)
{
        return 0;
}

static ssize_t dev_write(struct file *fp,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
        _DEBUG("");
        return -ENOSYS;
}

static ssize_t dev_read(struct file *fp,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
        int n;
        _DEBUG("");

        if (*off > 4096)
                return 0;

        n = copy_to_user(buf, &msg[index], 1);
        index++;
        if (index >= strlen(msg))
                index = 0;

        (*off)++;         /* supress compiler warning */

        return 1;
}

struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = dev_read,
        .write = dev_write,
        .open = dev_open,
        .release = dev_release,
};

static int __init dev_init(void)
{
        int error;
        int rval;

        _DEBUG("");

        error = alloc_chrdev_region(&dev, 0, 1, MODULE_NAME);
        if (error) {
                _DEBUG("alloc_chrdev_region() fails");
                return -1;
        }

        class = class_create(THIS_MODULE, "hello-world");
        if (IS_ERR(class)) {
                _DEBUG("class_create() fails");
                goto class_error;
        }

        cdev = cdev_alloc();
        if (!cdev) {
                _DEBUG("alloc_cdev() fails");
                goto cdev_error;
        }

        cdev_init(cdev, &fops);

        rval = cdev_add(cdev, dev, 1);
        if (rval < 0) {
                _DEBUG("cdev_add() fails");
                goto cdev_error;
        }
        
        device = device_create(class, NULL, dev, NULL, "hello-world");
        if (IS_ERR(device)) {
                _DEBUG("device_create() fails");
                return -1;
        }
                
        return 0;

cdev_error:
        cdev_del(cdev);
        unregister_chrdev_region(dev, 1);

class_error:
        class_unregister(class);
        class_destroy(class);

        return -1;
}

static void __exit dev_exit(void)
{
        _DEBUG("");
        device_destroy(class, dev);
        cdev_del(cdev);
        unregister_chrdev_region(dev, 1);
        class_unregister(class);
        class_destroy(class);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_AUTHOR("Daniel Hilst Selli <danielhilst@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CHANGE HERE");
